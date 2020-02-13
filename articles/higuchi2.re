
= Kotlin MPPで作るドメインモデル実装
2020年5月15日（金）にアカデミーヒルズにて技術者向けのイベント GREE Tech Conference 2020 を開催します。
グリーグループの技術を社外に向けて発表する場として、グリーグループメンバーの登壇で構成されたメインセッション12本と、
公募セッション12本の合計24枠を用意しています。
そこで、公式アプリを開発することとなりました。
せっかく作るなら新技術にチャレンジしたい人達を集め、成果をGREE Tech Conferenceで発表出来れば最高です。
Kotlin MPPを使ってドメインモデルを実装し、Server/Android/iOSアプリでドメインモデル実装を共通化しました。
ここでは、その設計と実装について紹介します。

== 要件
プロジェクトを開始した10月上旬にいただいた要件は、次のものでした。

 * セッション情報がみれる
 * スケジュールがみれる
 * ニュースがみれる

セッションとは、１コマの発表のことです。
スケジュールとは、セッションを時系列で一覧表示したものです。
ニュースとは、参加者へ伝えたい情報を書いた記事のことです。

== ドメインモデル
要件を受けて、次のようなドメインモデルを作りました。

まず、セッションです。
セッションには、発表者、開始終了時刻、タイトル、説明文が必要です。
ドメインエキスパートと話していると発表資料と登壇動画を共有したいという要望が明らかになりました。
また、タグを付けて検索したいとの追加要望がありました。
それらをSessionドメインモデルとして定義しました。

//table[SessionTable][Session]{
属性名  型  説明
--------------------
Id	Int .
Speaker	List<Speaker>	登壇者情報
StartTime	Timestamp	開始時刻
EndTime	Timestamp	終了時刻
Title	String	タイトル
Description	String	説明文
SlideUrl	String	発表後に追加されるSlideShareのURL
MovieUrl	String	発表後に追加される登壇動画のURL
TagList	List<Tag>	タグ一覧。
//}

次にスピーカーです。
他のカンファレンスを参考にして、氏名、肩書き、経歴、GitHub/Twitterを表示することとなりました。

//table[SpeakerTable][Speaker]{
属性名  型  説明
--------------------
Id	Int	.
Name	String	登壇者表示名
Title	String	肩書き
GitHubId	String	登壇者情報に掲載するGitHubId
TwitterId	String	登壇者情報に掲載するTwitterId
Description	String	登壇者の説明文
//}

当初タグは、LocalEntityとしてSessionの中にList<String>として埋め込んでいました。
しかし、タグで検索したいとの追加要望を受け、切り出しました。

//table[TagTable][Tag]{
属性名  型  説明
--------------------
Id	Int	.
Name	String	タグの名前
//}

ニュース記事は、Articleとしました。
公開予約をしたいとの追加要望をいただいたため、属性として保持していたPublishedAtを使って公開制御することとしました。

//table[ArticleTable][Article]{
属性名  型  説明
--------------------
Id	Int	.
Title	String	タイトル
Description	String	説明文
PublishedAt	Timestamp	公開日時。現在時刻より過去の記事のみ表示する。
//}

===[column] ドメインモデルとは
ドメイン駆動設計(DDD)で定義されている言葉です。
システムの対象領域をドメインと呼びます。
ドメインの本質的な部分を表現するモデルをドメインモデルと呼びます。
ドメインモデルは、ドメインのエキスパートとプログラマの双方が理解できるものでなければなりません。
今回はTech Conferenceドメインのエキスパートである運営責任者の方からお話を伺い、ドメインモデルを作成しました。

===[/column]

== 実装
ドメインモデルは、以下の場所にファイルを配置しました。

//cmd{
├── common
    └── src
        └── commonMain
            └── kotlin
                └── jp.gree.techcon.common.model
                                    ├── Article.kt
                                    ├── Session.kt
                                    ├── Speaker.kt
                                    └── Tag.kt
//}

ドメインモデルは、シンプルにdata classとして実装します。

//listnum[SessionModel][SessionModel][kotlin]{
package jp.gree.techcon.common.model

import kotlinx.serialization.Serializable

@Serializable
data class Session(
    val id: Long,
    val name: List<Speaker>,
    val startTime: Long,
    val endTime: Long,
    val title: String,
    val description: String,
    val tagList: List<Tag>,
    val slideUrl: String? = null,
    val movieUrl: String? = null
)
//}

slideUrlとmovieUrlは発表後に追加されるため、Optionalとしています。
他も同様に定義をそのままdata classにすることで実装しています。
明らかなため、ここでは省略させていただきます。

===[column] Kotlin MPPとは
Kotlin MPPとは、Kotlin Multi Platformの略称です。
Kotlinで書かれたコードをAndroidだけでなく、iOS/Web/Serverなど様々なPlatformで実行することができます。
例えば、共通コードに次のようなinterfaceを作ります。

//listnum[WriteLog][WriteLog][kotlin]{
internal expect fun writeLogMessage(message: String, logLevel: LogLevel)
//}

javaとjavascript用の実装を次のように書きます。

//listnum[KotlinWriteLog][KotlinWriteLog][java]{
internal actual fun writeLogMessage(message: String, logLevel: LogLevel) {
    println("[$logLevel]: $message")
}
//}

//listnum[JSWriteLog][JSWriteLog][javascript]{
internal actual fun writeLogMessage(message: String, logLevel: LogLevel) {
    when (logLevel) {
        LogLevel.DEBUG -> console.log(message)
        LogLevel.WARN -> console.warn(message)
        LogLevel.ERROR -> console.error(message)
    }
}
//}

このようにして、javaとjavascript双方で動くコードを実装することができます。
裏側を共通化し、UIに関わる部分をSwiftUIなどPlatform固有の実装を行うことができます。
全Platformでの共通処理であるドメインモデルの実装に最適と考えました。

===[/column]

== Serverからの利用
DatabaseはMySQLを利用しました。
複数のスピーカーで1つのセッションを行うことがあります。
また、1名のスピーカーが複数のセッションを行うことがあります。
つまり、スピーカーとセッションは多対多の関係です。
そこで、関係を保持するテーブルを作成しました。
同様にタグも多対多ですので、関係テーブルを作成しました。

//listnum[Database][Database][sql]{
CREATE TABLE `Sessions` (
      `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
      `start_time` int(10) unsigned NOT NULL,
      `end_time` int(10) unsigned NOT NULL,
      `title` varchar(128) CHARACTER SET utf8mb4 DEFAULT NULL,
      `description` varchar(4096) CHARACTER SET utf8mb4 DEFAULT NULL,
      `slide_url` varchar(1024) CHARACTER SET utf8mb4 DEFAULT NULL,
      `movie_url` varchar(1024) CHARACTER SET utf8mb4 DEFAULT NULL,
      PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=1 DEFAULT CHARSET=latin1 COLLATE=latin1_general_ci;

CREATE TABLE `Speakers` (
      `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
      `name` varchar(128) CHARACTER SET utf8mb4 DEFAULT NULL,
      `title` varchar(128) CHARACTER SET utf8mb4 DEFAULT NULL,
      `github_id` varchar(128) CHARACTER SET utf8mb4 DEFAULT NULL,
      `twitter_id` varchar(128) CHARACTER SET utf8mb4 DEFAULT NULL,
      `description` varchar(4096) CHARACTER SET utf8mb4 DEFAULT NULL,
      PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=1 DEFAULT CHARSET=latin1 COLLATE=latin1_general_ci;

CREATE TABLE `SpeakerRelations` (
      `session_id` int(10) unsigned NOT NULL,
      `speaker_id` int(10) unsigned NOT NULL,
      PRIMARY KEY (`session_id`,`speaker_id`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1 COLLATE=latin1_general_ci;

CREATE TABLE `Tags` (
      `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
      `name` varchar(128) CHARACTER SET utf8mb4 DEFAULT NULL,
      PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=1 DEFAULT CHARSET=latin1 COLLATE=latin1_general_ci;

CREATE TABLE `TagRelations` (
      `session_id` int(10) unsigned NOT NULL,
      `tag_id` int(10) unsigned NOT NULL,
      PRIMARY KEY (`session_id`,`tag_id`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1 COLLATE=latin1_general_ci;
//}

次にDaoを作成します。
データベースアクセスには、exposedを利用します。

//listnum[SessionDao][SessionDao][kotlin]{
package jp.gree.techcon.server.dao

import org.jetbrains.exposed.dao.IntIdTable

object Sessions : IntIdTable() {
    val startTime = integer("start_time")
    val endTime = integer("end_time")
    val title = varchar("title", 128)
    val description = varchar("description", 4096)
    val slideUrl = varchar("slide_url", 1024)
    val movieUrl = varchar("movie_url", 1024)
}

import org.jetbrains.exposed.sql.Table

object SpeakerRelations : Table() {
    val session = reference("session_id", Sessions).primaryKey(0)
    val speaker = reference("speaker_id", Speakers).primaryKey(1)
}

object TagRelations : Table() {
    val session = reference("session_id", Sessions).primaryKey(0)
    val tag = reference("tag_id", Tags).primaryKey(1)
}
//}

次にEntityを定義します。
exposedではvia Daoと書くと、関係テーブルを使って取得したオブジェクトを格納してくれます。

//listnum[SessionEntity][SessionEntity][kotlin]{
package jp.gree.techcon.server.entity

import jp.gree.techcon.server.dao.Sessions
import jp.gree.techcon.server.dao.SpeakerRelations
import jp.gree.techcon.server.dao.TagRelations
import org.jetbrains.exposed.dao.EntityID
import org.jetbrains.exposed.dao.IntEntity
import org.jetbrains.exposed.dao.IntEntityClass

class Session(id: EntityID<Int>) : IntEntity(id) {
    companion object: IntEntityClass<Session>(Sessions)

    var startTime by Sessions.startTime
    var endTime by Sessions.endTime
    var title by Sessions.title
    var description by Sessions.description
    var slideUrl by Sessions.slideUrl
    var movieUrl by Sessions.movieUrl
    var speakers by Speaker via SpeakerRelations
    var tags by Tag via TagRelations
}
//}

DBから取得したデータを共通のドメインモデルに詰め替えてクライアント側に送信します。
Kotlin MPPを使って作られていたKotlinFest公式アプリではセッション、スピーカーの全データを応答し、Client側でjoinしていました。
しかし、このアプリではデータ整形をサーバ側責務と考え、サーバ側でjoinして応答しました。

//listnum[SessionService][SessionService][kotlin]{
package jp.gree.techcon.server.service

import jp.gree.techcon.server.entity.*
import jp.gree.techcon.server.service.DatabaseFactory.dbQuery
import jp.gree.techcon.common.model.Session as SessionModel
import jp.gree.techcon.common.model.Speaker as SpeakerModel
import jp.gree.techcon.common.model.Tag as TagModel

class SessionService {
    suspend fun getAllSessions(): List<SessionModel> {
        var sessions: List<Session> = listOf()
        var result: List<SessionModel> = listOf()

        // get data from database
        dbQuery {
            sessions = Session.all().toList()
            // format data
            result = sessions.map { session ->
                val names: List<SpeakerModel> = session.speakers.map { 
                speaker ->
                    SpeakerModel(
                        speaker.name,
                        speaker.title,
                        speaker.githubId,
                        speaker.twitterId,
                        speaker.description
                    )
                }
                val tagNames: List<TagModel> = session.tags.map { 
                tag ->
                    TagModel(tag.name)
                }

                SessionModel(
                    session.id.value.toLong(),
                    names,
                    session.startTime.toLong(),
                    session.endTime.toLong(),
                    session.title,
                    session.description,
                    tagNames,
                    session.slideUrl,
                    session.movieUrl
                )
            }
        }
        return result
    }
}
//}

16行目でsessionの全データを取得しています。
34行目でcommonで定義している共通のドメインモデルに詰め替えて応答しています。
同様に19行目でspeakerを、29行目でtagを詰め替えています。
共通モデルに詰め替えることにより、Client側も共通モデルを使うことができます。


== Clientからの利用
iOS及びAndroidからは、共通処理としてKtor HttpClientを使ってドメインモデルにマッピングする処理を実装しました。
これをそれぞれのViewModelに詰め替えて表示を行います。

//listnum[Api][Api][kotlin]{
package jp.gree.techcon.common.datasource.network

import io.ktor.client.HttpClient
import io.ktor.client.features.json.JsonFeature
import io.ktor.client.features.json.serializer.KotlinxSerializer
import io.ktor.client.request.get
import jp.gree.techcon.common.Platform
import jp.gree.techcon.common.model.*
import kotlinx.serialization.list
import kotlin.native.concurrent.ThreadLocal

@ThreadLocal
internal object Api {
    private val client = HttpClient {
        install(JsonFeature) {
            serializer = KotlinxSerializer()
        }
    }

    // Read
    suspend fun getSessions(): List<Session> 
        = client.get<SessionList>("$endpoint/sessions").value
}
//}

== まとめ
Kotlin MPPを使うことで、Kotlinで共通コードを実装することができます。
Kotlinにはdata classが存在するため、ドメインモデルの実装に適しています。
今回はドメインモデルとその通信部分の実装を共通化することができました。

これまで携わったプロジェクトでは、ClientとServerで細部にズレが生じていました。
特にOptionalかどうかという部分は、実装を行った人の考えに依存するケースが多いと感じていました。
実装自体が共通化されることで、このようなブレが生じなくなることが期待できます。

今回は紙面の都合上、ドメインモデルとその周辺のみ紹介させていただきました。
今回のプロジェクト全体を説明すると1冊の書籍になりそうなので、できればチャレンジしてみたいと思います。

本アプリは現在開発中であり、3月中旬のストア公開時にソースコードも公開予定です。
GREE Tech Conferenceを含むGREEグループのEngineering関連情報は、 
https://twitter.com/gree_tech で随時発信しています。
興味のある方は、ぜひfollowをお願いします。
