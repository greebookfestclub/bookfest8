
= WFSの共通基盤と海外展開

== WFSとは
WFS@<fn>{endofn01}とはグリーの子会社で、ゲームアプリを制作・運営しています。WFSでは設立当初から、既存のグリープラットフォームから独立した、いわゆる普通のスマートフォン向けゲームアプリを作ろうとして設立されました。既存のグリープラットフォームというのは、いわゆるガラケー時代から続いているものです。これはグリーのアカウントを作ることがほぼ前提となっているため、今のスマホアプリに対しては合わない部分もありました。しかしこちらを使わないこととなると、既に存在していた便利なものを全て自前で作ることになります。そこで新たな共通の基盤が必要となり、開発がスタートしました。

== WFSアプリの共通基盤「gamelib」
共通基盤として必要なことは主にこの2点です。
* 複数のゲームに対応できること
* ゲーム以外を全て担当することで、ゲーム実装に集中してもらうこと
これらを実現すべ作られたのがgamelibという基盤およびチームです。機能としては
* アカウントの管理(認証基盤とアカウントの発行)
* 課金
* ゲーム内通貨を維持管理
* 経理処理
* キーワードフィルタ
などを担当しています。逆にアプリ毎に異なる部分は担当していません。キャラ管理などゲームに近い部分はそれぞれのアプリに担当してもらっています。
具体的に提供しているものは組み込みのためのSDKです。その中で外部システムとのやりとりも全て巻き取っています。さらに、各種ルールを守ってもらうためのコードレビュー活動もここで行なっています。こうして機能の提供と安全性を提供しています。

=== 補足
各機能の詳細について補足していきます。

==== アカウントの管理
アカウント管理において必要なことは安全なアカウント管理と発行はもちろんですが、引継ぎ機能が重要となります。パスワード発行方式やSNS連携方式などいくつかありますが、時代の変化により需要のある方法が変わるため、継続して開発する必要がある機能です。

==== アプリ内通貨管理
ゲームアプリにて販売されたゲーム内通貨は、一般的に有償分と無償分が区別して表示されているかと思います。これは、有償分の消費分が売上となるからです。また有償分の残高は法律上必要な供託金の計算の元となります。つまり販売された通貨は、消費分(売上へ)・残高(供託金などへ)を区別して管理する必要があり、これを管理します。

== 構成
gamelibがどのような構成になっているのかお話します。ざっくり表現するとこのような関係性です。

//indepimage[endo_01]

アプリはゲームサーバーとの通信の他、gamelibのSDKを通じてgamelibと通信を行います。gamelibサーバーを使うのは、ログイン、引き継ぎ、ゲーム内通貨の参照、課金などの時です。
ゲームサーバーも、ゲーム内通貨を更新するときにはgamelibへとアクセスします。それらの変動の結果をgamelibは集計し、経理やログシステムへと値を送ります。
クライアントからも、gamelibでは、cocos, unityのSDKをプロダクトに提供することで、簡単にgamelibを呼び出せるようにしています。

//indepimage[endo_02]

また、 gamelibはAWS上に構成されています。アプリから上から見ますと、ロードバランサを通ってEC2で処理してMySQLをDBに使うというシンプルな形です。この上で各所に冗長構成を入れています。最初はオンプレからはじまり中身は入れ替わり続けており、何度かのバージョンアップをへて今の形になりました。このあたりは社内のインフラチームと協力してやっています。

== 海外展開
これまでの内容で日本向けの展開ができました。このアプリを海外に展開するにあたり、gamelibでも対応が必要になりました。
課金の国・通貨が増えることへの対応は、SDKが実装を吸収してプロダクトでは気にしないで良いようにします。しかしインフラ、例えばヨーロッパから日本へ通信してしまうと非常に時間がかかってしまいます。

=== 海外での構成
そこで、海外にも国内gamelibと同じセットを構築することで、レイテンシが厳しい問題にも対応しました。ゲームサーバーも基本的には同じリージョンにサーバーを置いています。それぞれのgamelibのソースコードは同一で設定を変えているだけのものを配布しています。現地にサーバーがあるので高速で通信できます。ログだけ日本に送信することで、ログの集約も実現します。

=== 事後処理
そうして集まった課金データは、国と通貨がバラバラのものです。それらを現地通貨で集めた上で、適切に為替変換することで日本円を得ます。(※最終的な販売金額の確定値はapple/google側が知らせてくれる金額ですが、速報的に金額を出すためにこのようなことをやっています。)
また他にも対応が必要です。
* 経理処理
** 例えば消費税への対応も必要となります。国ごとにルールが異なるため、ある程度は調べていく必要があるため経理と連携していきましょう。
* 法律面
** これも例えばGDPR的にはEU内にサーバーがある必要は必ずしもありません。地理的なことよりも適切なログ管理のための仕組みが必要となります。また、課金キャンセルが認められている国もあるためクーリングオフ制度の詳細も調べて起きましょう。
これらのことをすることで、gamelibではプロダクトに負担をかけずに海外課金を実施することができました。またログも集約したままですので、国内外問わずに売り上げを把握することができるようになりました。

== まとめ
WFSの共通基盤を支えているのは一般的な技術です。それを更新しつつ提供しています。そこからの延長で海外課金も、地味な積み重ねで実現しています。
今後必要な場面が増えていくことで、組織と共に技術的な方向転換が必要になることもあるでしょう。その時にまたどこかでまとめを作らせて頂きます。

//footnote[endofn01][旧Wright Flyer Studios。 https://www.wfs.games/]

