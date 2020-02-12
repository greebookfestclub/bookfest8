= Virtual CastとHapbeatを使った国際双方向アバター触覚ライブの開発
#@# である調→ですます調のほうがいい感じがするので統一
#@# 参考文献が本文中でうまく参照できていない感じだけど、本文が冗長になることを避けたいのでできるだけ引用で済ます方針。

== VTuberライブエンタメ技術に求められる研究開発

GREE VR Studio Labは2018年からグリーグループに設置されたVTuber（バーチャルユーチューバー）やVRエンターテインメントシステムにおける研究開発、企業間連携、研究交流発信事業を担当する研究開発組織です。
現在、グリーが注力している分野のひとつであるVTuber事業は、アニメーションやゲーム、ライブ配信など、多岐にわたるエンターテインメント技術が必要とされる分野です。

それらを構成する技術要素はゲーム開発におけるリアルタイム3Dグラフィックスや、モーションキャプチャー、サーバー技術などがあり、加えて、ライブ・エンターテインメントに適したモーション生成や、演者支援、ディスプレイ技術、インタラクション技術、人工知能応用、キャラクター設計、エモーション分析、視聴者文化の調査分析など多岐にわたっています。このような新しい分野の研究開発は既存の企業内R&D手法だけでは問題分析や課題解決が難しく、産学連携やスタートアップ企業との連携などを通して、最先端の技術や手法を実装していくことが世界的なスタンダードとなりつつあります。

グリーグループでライブエンターテインメントを推進するWright Flyer Live Entertainment（以下、WFLE）には、主にプロダクション事業、プラットフォーム事業、ソリューション事業があり、そのプラットフォーム事業の中にもPresence Technology Group（以下PT）が、スタジオ技術を中心にVTuberコンテンツ制作やプラットフォーム開発を実施しています。

そうした中、VR Studio Labは、VRゲームなどを企画、開発、配信してきたGREE VR Studioと併設され、
（1） VTuber関連技術のR&D促進、（2） 異業種R&D連携強化、（3） 業界振興・イノベーション型人材の支援発掘や育成、
の3つを基本方針として活動しています。
WFLEが開発するVTuber専用ライブ配信プラットフォーム「REALITY」開発チームのもつ知見や経験を、日本国内の大学・企業・研究機関などの研究者や学生等とのコミュニケーションを通じて共有し、知財開発およびスピード感のある社会実装、品質向上、技術発信に貢献しています。

//image[WFLEPT-GVRS][WFLE PTとGREE VR Studioの位置づけ]

具体的には長期スパンのR&D、知財創出、アライアンス、新技術の調査選定、国際展開、普及啓蒙事業、社会実装を通した実験などがあります。
（本稿で扱いませんが）千葉大教育学部付属小学校でのVTuberを使った実験授業 @<bib>{CSR20190322}や、大学でのプログラミング講義向けアバターの教育向け応用（引用@<bib>{RexIVA}）、また新技術の開発系では無料ボイチェンサービス「転声こえうらない」（@<href>{https://vr.gree.net/lab/vc/}）のリリースや、ライブエンターテインメントの研究所らしくTwitter@<href>{https://twitter.com/vrstudiolab,@VRStudioLab}やYouTubeチャンネル @<href>{http://j.mp/VRSYT} でもライブイベントや資料映像を発信しております、SlideShareでも資料を公開しておりますので（@<href>{https://www.slideshare.net/vrstudiolab/}）、ご興味ありましたらご購読いただけると幸いです。

=== SIGGRAPH Real-Time Live!について

本稿ではVTuber技術、特に次世代型のライブエンターテインメントで使えそうな技術や裏話について紹介していきたいと思います。
特に2019年11月にオーストラリアと日本を接続して実施した「SIGGRAPH Real-Time Live!」について、構成する技術について紹介していきます。

SIGGRAPH Real-Time Live!とは、SIGGRAPH内で開催されるリアルタイムのライブデモセッションです。
そもそも「SIGGRAPH」は ACM（Association for Computing Machinery, ニューヨークに本部のある計算機科学の国際学会）の分科会
「Special Interest Group on Computer GRAPHics」の略で、発足は1967年、カンファレンスとしては1974年から続く長い歴史を持ちます。
参加者規模も2万人～4万人といったも大きな国際会議なので「世界最大のコンピュータグラフィックスとインタラクション技術における国際会議」と呼んでよいと思います（@<href>{https://www.siggraph.org/}）。

SIGGRAPH年に一度の大会は、夏は北米で7月ごろ開催されます。特に2年に一度はロサンゼルスで開催しています。
ハリウッドの映画産業を擁するロサンゼルスでの開催が参加者にとっても便利ですし、カルフォルニア州による誘致の働きかけもあるそうです。
SIGGRAPH自体の参加者数は近年20,000人規模で推移していますが、やはりロサンゼルスでの開催がもっとも参加者が多いです。
かつてのピークから半減した理由のひとつとして、2008年からは冬場にSIGGRAPH Asia（以下、SA）として、アジアでの開催がされております。
もともとSIGGRAPH自体は中国・韓国・日本からの参加者・発表者も多く、SAは北米版と異なり、実施会社がドイツ系のケルンメッセ、そのシンガポール支社が中心に運営していることもありシンガポールを拠点に巡回しています。
すでにSAだけでも12年の歴史をもち、日本では過去、横浜（2009年）、神戸（2015年）、東京（2018年）に開催されており、直近では2019年に初のAPAC地域としてオーストラリア・ブリスベン、さらに2021年には東京での開催も予定されています。

さて、SIGGRAPHは映画産業と研究者のためだけのイベントではありません。
ゲームやライブエンタテインメント、最先端のインタラクティブ技術なども数多く扱われています。
筆者は1997年の初参加から、SIGGRAPHにはほぼ毎年参加しているのですが、この23年でゲーム産業は大きく成長し、マーケット向けにはE3、開発者向けにはGDCとエキスポの住み分けが進みました。
また関連分野のエキスポとしては、ラスベガスで1月に開催される「CES」は家電とIoT、テキサス州オースティンで3月に開催される「SXSW」は広告メディアとゲーム、音楽イベントとサービスのフィールド実験が特徴です。
GDCで扱ういわゆる「ゲーム技術」も、かつてのようにリアルタイムグラフィックス技術だけが重視されるのではなく、ネットワーキングや設計、インフラ、大規模なチームマネジメントといった技術に広がってきています。

一方でSIGGRAPH内では学術論文発表や見本市はそのままに、従来の主力であったアニメーション・フェスティバルといった映像作品の上映会は年々規模が縮小し、新技術のデモ、特にVRのコンテンツや技術デモは大きな規模で、かつ人気の展示となっています。
その中でもリアルタイムグラフィックスのデモとしては一番大きなセッションとして「Real Time Live!」（以下、RTL）が2010年に誕生しています。
これはリアルタイムグラフィックスを中心とするライブデモによるショーケースイベントです。
NVIDIAのようなGPUメーカーのR&D成果の発表や研究者による技術デモ以外に、ゲーム産業における具体的なタイトルの開発手法や、映画産業におけるPreVizといったライブプレビュー技術など、
各社がアクターを使ったり、熱のこもったプレゼンなどを行い、
いわゆる「技術デモ」というよりは派手さも求められるショーになります。
会場も数千人が入る超巨大会場で、1発表あたりの持ち時間は10分以下。
一方で、来場者が歓声をあげたり拍手したり、といった反応もライブで感じられる注目のセッションです。

=== GREE VR Studio Lab の挑戦（SA18RTL）
2018年の東京、SA18RTLは東京国際フォーラム「Hall C」という客席1,500人のホールで開催されました。この回ははじめてのSAにおけるRTLでしたので、上演プログラムは公募ベースではなくチェア（長谷川勇 氏，Luminous Productions／スクウェア・エニックス）による推薦ベースで実施されました。
日本からはスクエニやバンナムさんがR&D成果として、リアルタイムグラフィックスやAIによる対話的アニメーションのデモをする中、
GREE VR Studio Labは、はじめてのリアルタイムライブデモ「“REALITY: Be yourself you want to be” VTuber and presence technologies in live entertainment which can make interact between smartphone and virtual live characters（REALITY：「なりたい自分で生きていく」スマートフォンとバーチャルライブキャラクター間でやり取りできるVTuberとライブエンターテイメントのプレゼンス技術）」を発表しました @<bib>{SA18RTL}。
この発表はVR Studio LabとWFLEに加えて、ニュージーランドでモーションセンサーに仕える新素材を開発しているStretchSense社と、イギリスでソフトウェアによるモーションキャプチャー補完ミドルウェアを開発しているIKINEMA社と共同で実施しました。シナリオはすべて日本語＋英語バイリンガルで構築し、ラボディレクターである白井が今居＝リアリテ＝レアというキャラクターを演じ、iPhoneXによる表情モーションキャプチャーと特殊表情の可能性@<bib>{CCSE2019}、StretchSense社によるグローブ、UnrealEngine4による統合されたリアルタイムキャラクターアニメーションと、IKINEMA社による女性アクターと、リアルタイム・アニメーション・クリーニング・パイプライン、さらに新規でキャラクター「今居レア（IMAI Lea）」と「IKINEMA Aya」もデザインし、ダンスシーンもあり、けっこう大変でしたが得るものも大きな挑戦でした。
海外メディアからも取材や事後レポートがあり、現在もVR Studio LabのYouTubeでは人気のコンテンツになっています（@<href>{https://www.youtube.com/watch?v=zj2fe4A87GA}）。
なお、IKINEMA社は本件に関してプレスリリースを発行しています@<bib>{SA18RTL-IKINEMA}
（しかしその半年後にはAppleに買収されてしまいましたので、世の中なかなかうまくいかない）。

#@# //image[SA18-2355][SIGGRAPH ASIA 2018 TokyoでのReal-Time Live!の様子（SA公式提供）][scale=0.5]{
//image[SA18RTL][SIGGRAPH ASIA 2018 TokyoでのReal-Time Live!の様子（SIGGRAPH ASIA 公式提供）][scale=1.0]{
//}
//image[SA18RTL2][左の緑（IMAI Lea）が筆者です。ライブのダンスとiPhoneXによるライブの表情や特殊表情などをリアルタイムで披露しました。Studio LabのYouTubeでは人気のコンテンツになっています（@<href>{https://www.youtube.com/watch?v=zj2fe4A87GA}）][scale=1.0]{
//}


SA18RTLに続き、夏のSIGGRAPH2019においては、世界でのVTuber業界を再定義するべく「Virtual Beings World」というイベントを主催しました。
この中でGREE VR Studio Labは「World VTuber Showcase」というティザー動画コンテスト（@<href>{http://j.mp/WVS19}）を開催し、世界中から集まった22作の中からNTTドコモ「直感×アルゴリズム♪」をグランプリとして選出、VTuber文化のアンバサダーになれる権利として任命し、2019年11月にオーストラリアで開催されるSA19RTLにおけるコラボレーションのきっかけになりました@<bib>{VRON20190809}。

//image[S18VBW][SIGGRAPH 2018 で開催した「Virtual Beings World」の様子]{
//}


=={a-label} Virtual CastとHapbeatを使った国際双方向アバター触覚ライブの開発

以上のような経緯から、SA19RTLにおいては、NTTドコモ副島義貴Pと、ラボの発信活動でご協力いただいている産業用VRのスタートアップ企業・イクスアール株式会社COO小池健輔氏、そしてVR Studio Labのインターンであり、東京工業大学発スタートアップ企業「Hapbeat合同会社」の代表でもある山崎勇祐さんと共著する形で企画提案をエントリーし、採択されました（@<href>{https://sa2019.siggraph.org/attend/real-time-live/session/111}）@<bib>{VRON20191119}。

タイトルは「Global Bidirectional Remote Haptic Live Entertainment by Virtual Beings」で、VTuberによるグローバルな双方向触覚ライブです。
メインキャストは「生アニメ」として人気の「直感アルゴリズム」とコラボすることになりました。
最終的なキャストとしては「直感アルゴリズム」のKilin（リンリン）さん（法元明菜さん）、Xi（シー）さん（岩井映美里さん）が日本語と中国語を担当、そして英語担当として先述のWorld VTuber Showcaseでご縁のあった「お米アイドルMaiプリンセス」からミルキークイーンさん（尾車Rいのりさん）が英国系英語のナビゲータとして参加することとなりました（マネジメントとスタジオを協力いただいたジャストプロさんありがとうございました！）。


//image[SA19RTLconcept2][NTTドコモ副島Pのご協力によって華やかになったキービジュアル（左）と、ミルキークイーンさんの参加で一層華やかになったコンセプト画像の一部（右）][scale=1.0]{
//}


=== 実現したい機能
まず、この技術デモの目的として、複数人のVTuberのパフォーマンスを国際的なクラウドネットワークでつなぎ、触覚ディスプレイを使用して次世代の双方向バーチャルライブエンターテイメントの可能性を示すというところに目標を設定しました。

出演者は東京で3Dのリアルタイムキャラクターをライブで演じ、オーストラリアのブリスベンでSIGGRAPH ASIAの視聴者とスクリーン越しに交流します。
また東京側の出演者は触覚ディスプレイを介して会場の拍手と歓声を可視化しつつ感じることができるようにします。

そしてVTuberライブとしてぜひとも挑戦したかった要素が「エモーションの共有」です。
先述のとおりSIGGRAPHは、コンピューターグラフィックスとインタラクティブなテクニックの専門会議です。
多くの聴衆はグラフィックスで素晴らしい経験をするはずですが、このデモは次世代のエンタメとしてグラフィックスに加えて、魅力的な要素が必要です。
国際イベントにおける多様な聴衆のために、エモーション情報を分析して、出演者・参加者全員で共有したい。
ネットワーク越しのグローバルな双方向インタラクションとVTubers用の触覚ディスプレイは技術的にも新しく、（NTTドコモさんが関わる上でも）5Gの次世代ネットワークライブエンターテイメントとして魅力的な挑戦になるはずです。
理想をいえば、拡張された人間のライブの演技と触覚エンタメによって、距離、言語、文化、オーディオ、視覚障害の障壁も解決していきたい。

//image[REMOPREconcept][REMOPRE - Vibeshare初期の概要設計。VCはVirtualCastの略。]{
//}

とはいえ全部盛りにはできませんので、以上のような構想から、
今回のシナリオでは、触覚フィードバック、感情分析からのリアルタイムグラフィックスとインタラクションを中心に、
欧米圏を中心とするSIGGRAPH参加者に向けて、バーチャルビーイングの将来の可能性について、
楽しい雰囲気で感情に訴えていきたいという方向になりました。

まとめると、
（1）VTuberによる
（2）多言語かつ世界規模でグローバルな
（3）会場参加で双方向インタラクション
（4）触覚が有効なライブ体験
（5）ダンスや歌はしっかり楽しい
という構成です。
システム的にはコードネーム REMOPRE、後に「Viveshare」と名付けたシステムの開発が始まりました。


=== チーム編成と技術構成

まずは今回のデモを成立させる上で、また「直感アルゴリズム」の出演と双方向触覚を実装する上で、技術的な取捨選択を行いました。
昨年のようにWFLE PTグループで開発しているUnreal Engine 4（UE4）ベースで開発されているスタジオのエンジンを使用することも魅力的なのですが、
今回は挑戦として、また今後の技術の展開可能性を考えて、グラフィックエンジンは「VirtualCast」（バーチャルキャスト、以下VC）を採用することとしました。
ドコモさんとVirtualCast社さんのご協力もあり、エンタープライズ版を中心に開発を進行させていただきました（コンシューマ版でもほとんどの機能は動くように開発しています）。

この時点での座組は、NTTドコモ、バーチャルキャスト社、ジャストプロ、EXR、Hapbeat、そしてGREE VRStudio Labというスーパーチームです。
昨年は日本語と英語のバイリンガルですすめるコストがありましたが、今回はシナリオと実演以外は日本語だけで進行できそうです（これが後で別の問題を生んだりします）。

//image[SA19RTLteam][SA19RTLチームフォト、2019年10月8日・六本木ヒルズにて撮影。][scale=0.7]{
//}

=== Hapbeatを使用した国際双方向触覚ライブ

まずは触覚のハードウェアです。近年、触覚デバイスはVRエンタメの世界を中心にさまざま登場していますが、
「Hapbeat」は糸の張力を使った触覚デバイスで、音響信号から多様な触覚刺激を生起できる、かつ3万円程度で手軽に利用できるデバイスです。
Hapbeat社による最新モデル「Hapbeat-Duo」は左右の紐を別々に制御できるモデルです。
制御は音声信号のLRを使用して行い、ステレオ音源の左右の音量比がそのまま振動の出力比となります。
非対称な振動を提示することで方向や距離など、表現できる情報が非常に広く、効果音のような短い振動用の波形を挿入することで、
左右交互にトントン、とした打感を提示することも簡単にできますし、
ちょっとしたアルゴリズムを開発することで、HMDを装着したユーザーに距離や方向を提示することもできます。
（このあたりもラボの研究成果でもあります
参考: https://dl.acm.org/citation.cfm?id=3338562 
動画: https://www.youtube.com/watch?v=AgmFWBu5ZnM
）

今回はこのHapbeat-Duoを使い、オーストラリアと日本に分散して存在する出演者と、聴衆をつなぐ挑戦をします。

//image[hapbeatduo][使用した触覚デバイス「Hapbeat-Duo」（ @<href>{https://booth.pm/ja/items/1549687} より購入できます）][scale=0.5]{
//}




=== Virtual Castと距離への挑戦

東京と今回の開催地であるオーストラリア・ブリスベンは直線距離で 7,154km 離れています。地球の半径よりも遠いです。

//image[SA19RTL-map][東京と開催地のオーストラリア・ブリスベンは7,154km離れている。そして Virtual Cast の推奨環境は4K動画並み。][scale=0.8]{
//}

そして VC の推奨環境は「上り下りとも 50Mbps以上 、レイテンシ 10ms以下、パケットロスなし」という情報がバーチャルキャスト社より公式情報として伝えられました。
これは動画ストリーミングだと4K動画並みが必要ということになります。
なおVC内部の通信はモノビットエンジンを使っているようで（@<href>{https://monobit.co.jp/vtuver/}）、これについてはさすがゲーム用エンジン。安定していました。
一方で、VC内部の音声エンジンも、新しい仕組みを試しているところでした（@<href>{https://virtualcast.jp/blog/2019/10/virtualcastroadmap/}）。
詳しい話は割愛しますが、最終的な判断には音声の同時発声と音声品質、帯域、そして装置構成や技術的安定など複雑な要素が絡み合い、難度が高い要素でした。

=== （書きかけ）パフォーマンスの設計
//image[TokyoBrisbane][TokyoBrisbane]{
//}

まずは当初提案されていた7分の暫定シナリオを紹介します。

0-1分[はじめに] いくつかの言語で挨拶と自己紹介をします。メインMCは会場（ブリスベン）にありますが、2番目のMC、キャラクター、スタジオは東京にあります。

1〜2分[私たちは遠隔地にいます] 2番目のMCは、ビデオ会議スタイルのビジュアルを通じて、スマートフォンで現地時間とGPS（Googleマップ）を表示します。また、ストリーミングの時間差と待ち時間もわかります。各サイトは2つのYouTubeLiveストリーミングを介して接続され、安定していますが、片側で20〜40秒の遅延があります。

2-3分[双方向リモートハプティックライブ]オーディオチャネルを使用したHapbeatエクスペリエンスを示します。メインMC、審査員、および聴覚障害のある一部の視聴者は、Hapbeatデバイスに一般的なスマートフォン（YouTubeライブストリーミング）を装備します。パフォーマーは、遠くで面白い話をしながら、じゃんけんをします。

3〜4分[安定性、互換性、および利便性]技術的には、ハプティクスと特殊なデバイスは、リモート環境で実行するのが非常に困難です。しかし、このデモンストレーションは、現在のYouTube Liveパイプラインとの適合性と互換性を示しています。また、音声/視覚障害の合理的な解決策も示しています。

4〜5分[多言語での感情との相互作用]また、リアルタイムの翻訳で観客の顔、拍手、歓声を分析できるリアルタイムの感情アナライザーもあります。もともとは、パフォーマーの触覚フィードバック用ですが、視覚化されたデータとリアルタイムの翻訳は、会場のほとんどの聴衆にとっても魅力的です。会場の観客全員と面白い交流をします。

5-6分[ダンスプレイ]それはフィナーレです。私たちは、ダンスプレイで観客を次世代のライブエンターテイメントに連れて行きます。


VCを活用した暫定シナリオとして、9月10日ぐらいの段階でこんな感じになりました。

0-1: あいさつ

1-2: システムについて（豪州側バーチャルキャストに日本から凸する）

2-3: ミルキークイーンが英語で通訳しながら直感アルゴリズムKirinが日本語、Xiが中国語（計3人）がVTuber文化とファンとのコミュニケーションについて語る

3-6: パオズゲーム：白井が「会場で拍手をすると画面端のメーターがあがります」と紹介。右ウイングと左ウイングで拍手がなるたびにパオズが落ちてくる。「多いほうが勝ち！」その後「笑いでも何か落ちてきます」

5-6: バーチャル・ウィリアムテル：日豪で頭上のリンゴを弓で射る

6-7: 六本木で開催されたTIFFのようす（VR-HMD-5Gライブ）を紹介

=== VCI を使ったVirtual Castの拡張

VCはそのバーチャル空間にインタラクティブ性を持たせるために独自のスクリプトシステムを有しており「VCI」と呼ばれています。
VCIはUnity上で3Dオブジェクトをセットアップし、スクリプトはLua言語を使って書くことができます。
#@# https://virtualcast.jp/wiki/doku.php?id=vci:script:luatutorial

上記のシナリオに現れる「パオズゲーム」や「バーチャル・ウイリアムテル」は、「直感アルゴリズム」番組中ではVCIを使って実装されています。

コード的にはLuaなのでこんなかんじです。

//list[Confetti.lua][Confetti.lua 紙吹雪を降らせる]{
#@mapfile(shirai/Confetti.lua)
math.randomseed(os.time())
ConfettiTarget = vci.assets.GetSubItem("ConfettiTarget")
ConfettiTarget.SetLocalPosition(Vector3.__new(0, 1.5, 0))
vci.assets._ALL_SetMaterialColorFromName("Transparent",
 Color.__new(1.0, 1.0, 1.0, 0.5))
EEConfetti = vci.assets.GetSubItem("EEconfetti")
gen_height = 0
size = 1
ToumeiFlag = 0

function fallConfetti()
    local rand_xpos = math.random(0, 100) / 100
    local rand_zpos = math.random(0, 100) / 100
    local rand_xvel = math.random(-100, 100) / 100
    local rand_zvel = math.random(-100, 100) / 100
    local ConfettiTarget_pos = ConfettiTarget.GetLocalPosition()
    print(ConfettiTarget_pos)
    local pos = Vector3.__new(ConfettiTarget_pos.x,
     ConfettiTarget_pos.y + gen_height, ConfettiTarget_pos.z)
    local scale = Vector3.__new(size, size, size)
    EEConfetti.SetLocalPosition(pos)
    EEConfetti.SetLocalScale(scale)
    vci.assets.GetEffekseerEmitter("EEconfetti")._ALL_Play()
end

function update()
    if vci.me.GetButtonInput(1) then
        fallConfetti()
    end
    if vci.me.GetAxisInput().y == 1 then
        print("enableLEDmeter")
    end
    if vci.me.GetAxisInput().y == -1 then
        print("disableLEDmeter")
    end
end

function onUse(use)
    if use == "ConfettiTarget" then
        if ToumeiFlag == 0 then
            vci.assets._ALL_SetMaterialColorFromName(
                "Transparent", Color.__new(1.0, 0, 1.0, 0.0))
            ToumeiFlag = 1
        end
    end
end
#@end
//}

なお、VC自体の設計思想は「VR」というよりは
「HMDを使って演じるバーチャルな3Dキャラクターによる放送局のためのシステム」ですので、
VCIは主にグラフィックスについてのインタラクション、具体的にはシーングラフと衝突、ユーザーによるグラブアクションなどが取得できますが、
外部システムやデバイスとの通信はできません。
この先は色々な魔改造を施していくことになります。


== 実装と解説


//image[SA19RTLeq][筆者の機材的な装備（解説を入れる予定）][scale=0.5]{
//}

シナリオと技術的な実装要素を図にするとこんなかんじです。
//image[SA19-scenario][シナリオと技術的な実装要素（ほぼ最終版資料より）]{
//}


#@# https://docs.google.com/presentation/d/1IosX_76FQ8Fi4ewnXgXh5uGrb83492zZMgIc7kjINuc/edit#slide=id.p6
#@# SA19RTL https://docs.google.com/presentation/d/1MnfQwyAcq4JnD1lZKzVyksAS9ieWsNxJvo6msbVW-KQ/edit#slide=id.g78f914e51a_1_0

続いて、個々の技術についての実装紹介になります。

=== 会場音声分析からのギフト送信
まず、会場音声分析からのギフト送信を解説します。
会場の歓声や拍手、笑いなどによって、VCIのアイテムをバーチャルギフトとして降らせることがゴールです。

会場の音響を分析するPCを用意し、python環境で開発した音声分析システムを使い、その分析結果を
WebSocketを扱うことができるNode.jsのライブラリ「Socket.IO」を使ってJSON形式にて送受信させます。
このあたりはラボのインターン、栢之間さん（東工大2年生）が中心になってPythonによる音響分析を実装しています。

//list[MicPC-index.js][index.js（音響分析PC）]{
const { PythonShell } = require('python-shell'); //pythonをnodeから起動する
const express = require('express');
const app = express();
const socketio = require('socket.io')
app.use(express.static('public'));
const expressServer = app.listen(3000); //clientのポート番号（localhost:3000)
const io = socketio(expressServer); 
var python_script_path = '../../EmotionAnalysis/'; //実行するpython script
let pyEmoAnalysis = new PythonShell(
    'main.py', { mode: 'text', pythonOptions: ['-u'],
    scriptPath: python_script_path });

//EmoAnalysisからデータが飛んで来たら実行
pyEmoAnalysis.on('message', data => {
    obj = JSON.parse(data)
    console.log(obj)
    io.emit('send_EmoAna_Result', obj) 
})
//}

上記の main.py で呼ばれる音声分析系はこんなかんじです。

//list[judgeSet.py][judgeSet.py（main.pyに呼び出される）]{
def realtime_sub(signal, fs):
    mfcc = an.mfcc(signal, fs)
    stft = an.stft(signal)
    applause = an.find_largeapplause(mfcc, threshold[0])
    laugh = an.find_laugh(mfcc, threshold[1])
    excitement = an.excitement_base(stft)
    return [applause, laugh, excitement]*coefficients
//}

ライブラリは PyAudioと librosa が主に使われています。
STFTは short-time Fourier transform 短時間フーリエ変換です。
MFCCはメル周波数ケプストラム係数（Mel-Frequency Cepstrum Coefficients）という
聴覚フィルタに基づく音響分析手法で、人間の聴覚特性にあわせたフィルタバンクを使って高速に音響の特徴を抽出できます。
結果として applause, laugh, excitement つまり、拍手のような喝采、笑いと興奮を抽出します。単純に拍手だけ判定することも可能です。

#@# 冗長なのでとりあえず使用しない方向で
#@# //list[judgeSet.py][judgeSet.py main.pyに呼び出される]{
#@# def realtime(project):

=== VCI と node.js を使ったVirtual Castの拡張

前述のとおり、VCおよびVCIには外部システムとの通信機能はありませんが、唯一、デバッグ情報をWebSocketで受け取ることができます。
oocytanbさんの vci-logcat というプロジェクト（
@<href>{https://github.com/oocytanb/vci-logcat}
）が詳しいのでここでの詳説は割愛します。

VCI→他のシステムへの通信はWebSocket通信を使って実装します。
逆に他のシステム→VCIへのイベントは、キーボード信号を使って実装することにします。
具体的には pyAutokey という Pythonライブラリを使って、VCを使っているPCのキーイベントを発生させて、VC側ではそのキーイベントを拾います。

//list[MainPCindex.js][自動マウス＆キー操作用のpythonプログラムのセットアップ]{
let pyAutoKey = new PythonShell('autokey.py',
 { mode: 'text', pythonOptions: ['-u'], scriptPath: './' });
// 自動キー入力を行う閾値
var Hi_Threshold = 0.7
var Lo_Threshold = 0.4

//io.on→socketが接続されたとき起動
io.on('connection', (socket) => {
    console.log('made socket connection', socket.id);
    socket.on('echo', (arg) => {
        console.log(arg)
    })
    socket.on('disconnect', () => {
        console.log('disconnect');
    });

    // mapするときの最大値を決定。最大値との比率で値を決める
    socket.on('send_EmoAna_Result', (obj) => {
        var Applause_fromMax = 0.13
        var Laugh_fromMax = 0.02
        // まず得られた解析結果を0 ~ 指定した最大値の範囲（範囲A）に限定する。
        // その後、範囲Aを0~1の範囲にマッピングする
        obj.L_L = map(value_limit((obj.L_L - 0.001), 0,
         Laugh_fromMax), 0, Laugh_fromMax, 0, 1)
        obj.L_A = map(value_limit((obj.L_A - 0.04), 0,
         Applause_fromMax), 0, Applause_fromMax, 0, 1)
        io.emit('tc2client', obj)
    });
});
//}

マッピング後の解析結果が閾値を超えたらマウスを (100,100）に動かし、指定のキー入力を動作させます。
ただしこのスクリプトは起動後にマウスとキー入力を奪われるので、本番以外はコメントアウト推奨です。

=== M5Stackによる触覚の可視化
//image[REMOPRE1117][REMOPRE1117]{
//}


筆者が背負っているPC（HP OMENX）の中で動いている nodeサーバのコード（index.js）はこんなかんじです。
音声分析PCから送られてきた解析結果をマッピングして、VCのデバッグ情報をWebsocketで受け取ります。
それらの値をトリガーとして、M5Stackにシリアル通信でコマンドを送りLEDを光らせます。

なお、すべてクラウド環境で動作したい設計なので、ngrok（エングロック）を使ってトンネル化し、
ローカル環境上で実行しているアプリケーションの通信をインターネット経由で動作するようにしています。
これは会場のネットワーク、特にWifiが数千人の国際イベントではまともに動作しないことを想定し、LTE等の公衆回線を使うことを想定した設計です。

//list[OMEN-index.js1][VCからのメッセージを送ってマイコンでLED光らせる index.js（前半）]{
const express = require('express');
const socket = require('socket.io');
const { PythonShell } = require('python-shell'); // varを{}で囲むのが重要らしい

// サーバー立ち上げ
var app = express();
const PORT = process.env.PORT || 4000 //環境変数orPort4000
var server = app.listen(PORT, () => {
    console.log('listening to requests on port ' + PORT)
});

//ngrokで起動したいとき（port80)
const ngrok = require('ngrok')

connectNgrok().then(url => {
    console.log('URL : ' + url);
});
async function connectNgrok() {
    let url = await ngrok.connect({
        addr: 4000, // port or network address, defaults to 80
        subdomain: 'omen', // reserved tunnel name https://alex.ngrok.io
        authtoken: '***', // your authtoken from ngrok.com
        region: 'jp', // one of ngrok regions (us, eu, au, ap), defaults to us
    });
    return url;
}
//}


M5Stackは中々優秀なのですが、通信を文字で行うと流石に重たく、24個のフルカラーLEDが美しく光ってくれないので、intをうまく使って3種類の点灯モードと強度を詰め込んでいきます。

//list[OMEN-index.js2][VCからのメッセージを送ってマイコンでLED光らせる index.js（後半）]{
// Socket setup
var io = socket(server);
var L_Top2Bot = 200 //数字の説明：xyz, y = 左右（左＝0、右＝1）
R_Top2Bot = 210
L_Bot2Top = 201
R_Bot2Top = 211
L_Center2Edge = 202
R_Center2Edge = 212
L_Edge2Center = 203
R_Edge2Center = 213
L_MeterLED = 204
R_MeterLED = 214
Random_Rainbow = 205
HandShaked = 206
hit2body = 207
Start_HB = 250
Stop_HB = 251

var LED_Mode = 1; //LEDが光るモード。0にすると拍手・笑いをdisable
let pySendSerial = new PythonShell('sendserial.py',
 { mode: 'text', pythonOptions: ['-u'], scriptPath: './' });
pySendSerial.on('message', data => {
    console.log(data)
})

// 値の範囲を変換する関数。例：(5,0,10,0,100) => return 50
const map = (value, fromMin, fromMax, toMin, toMax) => {
    let result = 0;
    result = (value <= fromMin)
        ? toMin : (value >= fromMax)
            ? toMax : (() => {
                let ratio = (toMax - toMin) / (fromMax - fromMin);
                return (value - fromMin) * ratio + toMin;
            })();
    return result;
};

// 値の範囲をmin~maxの間で制限する関数。例：(5,0,4) => return 4
function value_limit(val, min, max) {
    return val < min ? min : (val > max ? max : val);
}
//}

会場で笑いや拍手が起きると、両脇のカラーLEDが3種類のパターンでカラフルに明滅します。
実際、演者はHMDを装着しており、会場のリアクションに反応することは難しいのですが、これによって触覚とLEDが双方向性を与えてくれます。
なお、ステージや会場の遠くからの視認性を確認するために、ハロウィン前夜の六本木ヒルズで実験したりしました。

== 当日に起きたこと ＆ 今後に向けて

その後、国際リモートライブのためのリハーサル、特に日豪同時リハーサルやReal-Time Live!特有の公式練習、ひたすらダンスの練習（オリジナル振付）、機材がクソ重い、音声品質と遠のくゴール、不測のトラブル、すごすぎる他の発表者…などなどいろいろありましたが、そろそろ長すぎるような気もするのでこのへんで筆を置きたいと思います。

#@# Matt AI - Speech Driven Digital Human with Emotions http://sa2019.conference-program.com/presentation/?id=real_106&sess=sess230 Jingxiang Li, et al. Tencent Technology Company Limited

結局のところ、シナリオのおもしろさをレビューするのも大事だし、設計のレビューも、音響周りのレビューも、技術のライブデモはほとんど技術なのでした。　

次回のSIGGRAPHはワシントンです。
でもオリンピックも直撃の日程なので、なかなか無理はできなさそう。

2021年のSAは東京なので、ここに向けてどんどんパワーアップして、世界中を驚きと笑いでひっくり返したいですね！

== 謝辞
 SA19RTL ふりかえり ミルキークイーンさん/尾車Roseいのりさん
いつも元気で知的で英語シナリオの監修までやっていただいたミルキーさん。
本当にありがとうございました。

我々がなぜバーチャルビーイング（virtual being）なのか?という本質的な問 いについての深いシナリオまでできていたのですが、 残念ながら本番は声が元気すぎてマイクが死んでしまったものと推測しま す。今後、再挑戦する機会を是非とも作りたい! ぜひご一緒できましたら幸いです。
リンリンさん/法元明菜さん。
いつもよく気がつく努力家なリンリン/法元明菜さん。作って頂いた振り付け がとてもよく、徹夜で練習しまくりました。本番では音が飛んでいたのに踊 り続けられたのはこの特訓のおかげ!そして自己紹介パートのステキなイラ ストありがとうございました!
シーさん/岩井映美里さん いろんなご都合で本番直前のみの絡みとなり「いつもマイペースで何を考え ているか分からないところがあるよ~」という情報だけは頂いていたので実 はちょっぴり不安があったのですが、そんなことは杞憂でした。本当に声もキレイだし、芯がしっかりした人物なのだと感じました。特に最後のダンスシ ーン!会場のミュージックがトラブルで切れており、しかもバーチャル内で はそのトラブルがわからない状態だったのに、最後まで歌い続けて踊りもし っかり...会場は何が起きているのか、これがライブなのか!とわかり手拍子 から拍手の渦...今思い出しても泣きそうになります。
スタッフの皆様/おわりに
チェアの皆さんや他のReal-Time Live!発表者にもインタビューしてみたので すが、やっぱり会場をもっとも沸かせたのはウチの発表!新開発の会場熱気分析 システムもみごとにメーター振り切っていました。会場投票が無かったのは

 残念!トロフィー持って帰りたかった!くやしい! でも音声トラブルがあってシナリオがしっかり聞いてもらえなかったのも事実だし、審査員賞を取ったパフォーマンスもホント凄かった!これは是非と も学びたいやつです。 というわけで次はの挑戦は夏のSIGGRAPH（ワシントンD.C. 8/19-23）で会場 投票一位を目指して頑張るか、中国ライブかな...同じことは二度とやらない のがラボのポリシーでもあるので、技術的チャレンジも盛り込んでがんばっていきたいと思います!


====[column] Re:view環境で技術書を書く

本稿を書くにあたり、Re:view環境初体験となりました。

普段はOverLeafなどのクラウドLaTeX環境で執筆することが多いです。
（日本語の論文ではない書物、技術書は久しぶりに書きました！）

時々VSCodeのブラウザ版で書いてみたりもしましたので、ちょっとだけ紹介しておきます。
@<href>{https://online.visualstudio.com/}
最初はWindowsにDocker環境を構築しようとしてジタバタいたのですが、執筆のためだけにあれこれインストールするのに疲れました…Azureのアカウントが必要ですが、無料枠で十分な感じです。
このブラウザ版VSCode（正式名称はVisual Studio Online）にはターミナルもbash環境もあり、Docker環境も作れるようなのですが、あれこれインストールするならいっそ
Visual Studio Code環境だけでも作れそうな気がします。

そもそも本書は樋口さんらにより Github上にRe:VIEW で執筆環境を構築されておりますので、Github上で自分のブランチのpublishをしてプルリクエストさえ出せれば、あとはcircleciがRe:VIEW→TeX→PDFまでコンパイルしてくれちゃいます。

画像ファイルの追加なども Visual Studio Onlineなら簡単にアップロードしてリポジトリに追加できてしまいますので、学生さんの卒論等にも便利なのではないかと思いました。
自分の研究室ではかつて、卒論集を Cloud LaTeX や Github で管理していたのと、自分の書籍はXMLで書いていた時期もあったので技術書典部をきっかけにいろいろ勉強になりました！

えっ、GithubのURLから直接編集できるんですか？便利すぎる！
CI環境なども素敵です。
iPadのブラウザでも行けそうです！最近のiPadOSはZIPファイルもスンナリ解凍できるし…これは結構快適な技術書ライフが送れそうです。

とはいえDocker環境はMacOSが圧倒的に楽ですね。
樋口さん 環境構築ありがとうございました！！

====[/column]

