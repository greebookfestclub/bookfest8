
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


== ドメインモデル
要件を受けて、次のようなドメインモデルを作りました。

//table[SessionTable][Session]{
属性名  型  説明
--------------------
Id	Int .
Speaker	List<Speaker>	登壇者情報
StartTime	Timestamp	開始時刻
EndTime	Timestamp	終了時刻
Title	String	タイトル
Description	String	説明文
SlideUrl	String	発表後に追加されるSlideShareのURL(Webのみ表示? 要確認)
MovieUrl	String	発表後に追加される登壇動画のURL(Webのみ表示? 要確認)
TagList	List<Tag>	タグ一覧。
//}

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

//table[TagTable][Tag]{
属性名  型  説明
--------------------
Id	Int	.
Name	String	タグの名前
//}

//table[ArticleTable][Article]{
属性名  型  説明
--------------------
Id	Int	.
Title	String	タイトル
Description	String	説明文
PublishedAt	Timestamp	公開日時。現在時刻より過去の記事のみ表示する。
//}

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

//listnum[SessionModel][kotlin]{
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

== Serverからの利用
DBから取得したデータを共通のドメインモデルに詰め替えてクライアント側に送信します。
Kotlin MPPを使って作られていたKotlinFest公式アプリではセッション、スピーカーの全データを応答し、Client側でjoinしていました。
しかし、このアプリではデータ整形をサーバ側責務と考え、サーバ側でjoinして応答しました。

//listnum[SessionService][kotlin]{
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
                val names: List<SpeakerModel> = session.speakers.map { speaker ->
                    SpeakerModel(
                        speaker.name,
                        speaker.title,
                        speaker.githubId,
                        speaker.twitterId,
                        speaker.description
                    )
                }
                val tagNames: List<TagModel> = session.tags.map { tag ->
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

== Clientからの利用
iOS及びAndroidからは、共通処理としてKtor HttpClientを使ってドメインモデルにマッピングする処理を実装しました。
これをそれぞれのViewModelに詰め替えて表示を行います。

//listnum[Api][kotlin]{
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
    suspend fun getSessions(): List<Session> = client.get<SessionList>("$endpoint/sessions").value
}
//}

== まとめ
本アプリは現在開発中であり、3月中旬のストア公開時にソースコードも公開予定です。
GREE Tech Conferenceを含むGREEグループのEngineering関連情報は、 https://twitter.com/gree_tech で随時発信しています。
興味のある方は、ぜひfollowをお願いします。
