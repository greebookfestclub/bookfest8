#@# ToDo: ファイルを分割して終わった箇所と書きかけの箇所を分離する
#@# である調→ですます調のほうがいい感じがするので統一
#@# https://docs.google.com/presentation/d/1IosX_76FQ8Fi4ewnXgXh5uGrb83492zZMgIc7kjINuc/edit#slide=id.p6
#@# SA19RTL https://docs.google.com/presentation/d/1MnfQwyAcq4JnD1lZKzVyksAS9ieWsNxJvo6msbVW-KQ/edit#slide=id.g78f914e51a_1_0
#@# ファイルの分割について https://blog.kakeragames.com/2019/12/02/split-re-view-file.html
#@# shirai-done.re: 書き終わり、校正中。
#@# shirai-working.re: 書きかけ、校正前。
= Virtual CastとHapbeatを使った国際双方向アバター触覚ライブの開発
#@mapfile(shirai/shirai-done.re)

== VTuberライブエンタメ技術に求められる研究開発

筆者がDirectorをつとめる「GREE VR Studio Laboratory」は2018年からグリーグループに設置されたVTuber（バーチャルユーチューバー）やVRエンターテインメントシステムにおける研究開発、企業間連携、研究交流発信事業を担当する研究開発組織です。
現在、グリーが注力している分野のひとつであるVTuber事業は、アニメーションやゲーム、ライブ配信など、多岐にわたるエンターテインメント技術が必要とされる分野です。

それらを構成する技術要素はゲーム開発におけるリアルタイム3Dグラフィックスや、モーションキャプチャー、サーバー技術などがあり、加えて、ライブ・エンターテインメントに適したモーション生成や、演者支援、ディスプレイ技術、インタラクション技術、人工知能応用、キャラクター設計、エモーション分析、視聴者文化の調査分析など多岐にわたっています。
（もちろんエンジニアの皆さんは日々このような開発を専門的に取り組んでいることは間違いないのですが）
このような新しい複合横断分野の研究開発は、既存のミッションクリティカルな単一企業内R&D手法だけでは、個々の課題は個々の技術やソリューションで解決できても、そもそもの問題設定や異なるユーザーを巻き込んだ社会としてのゴールが明確ではない課題が多くあります。
「社会実装」と呼ばれる技術のデプロイメントがそこにあり、産学連携やスタートアップ企業との連携などを通して、最先端の技術や手法を実装していくことが世界的なスタンダードとなりつつあります。

グリーグループでライブエンターテインメントを推進するWright Flyer Live Entertainment（以下、WFLE）には、主にプロダクション事業、プラットフォーム事業、ソリューション事業があります。プラットフォーム事業の中にもスマホアプリやインフラだけでなく、Presence Technology Group（以下PT）という、VTuber番組のための製作技術、スタジオ技術を中心に開発する組織もあります。
WFLEの組織図について、カジュアルな公式資料としては事業代表者の荒木英士（DJ RIO）によるブログをご参考にいただけると幸いです@<bib>{Wantedly}。

//image[WFLEPT-GVRS][WFLE PTとGREE VR Studio Laboratoryの位置づけ]

このように、まさに新しい産業を掘り起こしていく体制の横で、弊ラボ「VR Studio Laboratory」は、VRゲームなどを企画、開発、配信してきたGREE VR Studioと併設され、
（1） VTuber関連技術のR&D促進、（2） 異業種R&D連携強化、（3） 業界振興・イノベーション型人材の支援発掘や育成、
の3つを基本方針として活動しています。
WFLEが開発するVTuber専用ライブ配信プラットフォーム「REALITY」開発チームのもつ知見や経験を、日本国内外の大学・企業・研究機関などの研究者や学生等とのコミュニケーションを通じて深め、共有し、知財開発およびスピード感のある社会実装、品質向上、技術発信に貢献しています。


具体的には長期スパンのR&D、知財創出、アライアンス、新技術の調査選定、国際展開、普及啓蒙事業、社会実装を通した実験などがあります。
（本稿で扱いませんが）千葉大教育学部付属小学校でのVTuberを使った実験授業 @<bib>{CSR20190322}や、大学でのプログラミング講義向けアバターの教育向け応用（引用@<bib>{RexIVA}）、またエンタメサービスを通した実験としては無料ボイチェンサービス「転声こえうらない」（@<href>{https://vr.gree.net/lab/vc/}）のリリースがあります。
もちろんライブエンターテインメントの研究所らしくTwitter @<href>{https://twitter.com/vrstudiolab,@VRStudioLab}や、YouTubeチャンネル（@<href>{http://j.mp/VRSYT}）でも
VRエンタメ技術についてのビデオレポートやライブイベント、資料映像を発信しております。
またSlideShareでも資料を公開しておりますので（@<href>{https://www.slideshare.net/vrstudiolab/}）、ご興味ありましたらご笑納ご購読いただけると幸いです。

本稿では上記のようなWebメディアでは解説が難しいVTuber技術、特に次世代型のライブエンターテインメントで使えそうな話の中でも、特にエンタメコンテンツや一般的な読みものとしては``読者を選ぶであろう''技術的な裏話をしていきたいと思います。
とはいえ前述のとおり、
ラボで扱っている技術は膨大で範囲も広い一方、
WFLEのエンジニアさんたちは、インフラやスマホアプリの開発については非常に尖った技術を持った優秀な方々がおり、
私が筆を執るような立場ではありません。
興味がある人はこちらに採用サイトがありますので、まずはエントリーして中の人と対話してみたほうがよいと思います（@<href>{https://www.wantedly.com/companies/wrightflyer/}）。
「なりたい自分で、生きていく」が社是なので、波長が合う人にはきっと居心地がいいでしょう。

さて今回は、特に2019年11月に実施した「SIGGRAPH Real-Time Live!」について、技術的なチャレンジといくつかの実装を紹介していきたいと思います。
あまりに扱っている技術が膨大で、また経験としても、非常に稀有な開発であり、かつ今後のVTuber業界や5G開発においてちょっとした知見になれば幸いです。

=== SIGGRAPH と Real-Time Live!について
SIGGRAPH Real-Time Live!とは、SIGGRAPH内で開催されるリアルタイムのライブデモセッションです。
そもそも「SIGGRAPH」は ACM（Association for Computing Machinery, ニューヨークに本部のある計算機科学の国際学会）のCG分科会
「Special Interest Group on Computer GRAPHics」の略で、発足は1967年、カンファレンスとしては1974年と長い歴史を持ちます。
参加者規模も2万人～4万人と大きな国際会議なので「世界最大のコンピュータグラフィックスとインタラクション技術における国際会議」と呼んでよいと思います（@<href>{https://www.siggraph.org/}）。

SIGGRAPHコミュニティの年に一度の大会は、夏に北米で7月ごろ開催されます。特に2年に一度はロサンゼルスで開催しています。
ハリウッドの映画産業を擁するロサンゼルスでの開催が参加者にとっても便利ですし、カルフォルニア州による誘致の働きかけもあるそうです。
SIGGRAPH自体の参加者数は近年20,000人規模で推移していますが、やはりロサンゼルスでの開催がもっとも参加者が多いです。

2008年からは冬場にSIGGRAPH Asia（以下、SA）として、アジアでの開催がされています。
もともとSIGGRAPH自体は中国・韓国・日本からの参加者・発表者も多く、SAは北米版と異なり運営会社はドイツ系のケルンメッセで異なります。
またシンガポール支社が中心に運営していることもありシンガポールを中心に巡回しています。
すでにSAだけでも12年の歴史をもち、日本では過去、横浜（2009年）、神戸（2015年）、東京（2018年）にて開催されており、さらに2021年には東京での開催も予定されています。
直近では2019年11月末に初のAPAC地域の南半球、オーストラリア・ブリスベンで開催されました。

さて、SIGGRAPHは映画産業と研究者のためだけのイベントではありません。
ゲームやライブエンタテインメント、最先端のインタラクティブ技術なども数多く扱われています。
筆者は1997年の初参加から、SIGGRAPHにはほぼ毎年参加しているのですが、この23年でゲーム産業は大きく成長し、マーケット向けには「E3」、開発者向けには「GDC」とエキスポの棲み分けが進みました。
また関連分野のエキスポとしては、ラスベガスで1月に開催される「CES」は家電とIoT、テキサス州オースティンで3月に開催される「SXSW」は広告メディアとゲーム、音楽イベントとITサービスのフィールド実験が特徴です。
GDCで扱ういわゆる「ゲーム技術」も、かつてのようにリアルタイムグラフィックス技術だけが重視されるのではなく、ネットワーキングや設計、インフラ、大規模なチームマネジメントといった技術に広がってきています。

一方でSIGGRAPH内では学術論文の口頭発表や見本市といったベース構成は大きく変わらずも、過去の特徴であったアニメーション・フェスティバルといった映像作品の上映会は年々形を変え縮小しつつあります。新技術のデモ、特にVRのコンテンツや技術デモは大きな規模に、かつ人気の展示となっています。
その中で、数あるリアルタイムグラフィックスのデモの中で、最も大きなセッションとして「Real Time Live!」（以下、RTL）が2010年に誕生しています。
これはリアルタイムグラフィックスを中心とするライブデモによるショーイベントです。
NVIDIAのようなGPUメーカーのR&D成果の発表や研究者による技術デモ以外に、ゲーム産業における具体的な開発手法のライブデモや、映画産業におけるPreVizといったライブプレビュー技術など、
各社がアクターを使ったり、熱のこもったプレゼンなどを行い、
いわゆる「技術デモ」というよりは「技術と派手さが求められるショー」を披露します。
会場も数千人が入る超巨大会場で、1発表あたりの持ち時間は10分以下という短さ。
一方で、来場者が歓声をあげたり拍手したり、といった反応もライブで感じられる注目のセッションです。

=== GREE VR Studio Lab の挑戦「SA18RTL」
振り返ること2018年12月の東京。SIGGRAPH ASIA Real Time Live!（SA18RTL）は東京国際フォーラム「Hall C」という客席1,500人のホールで開催されました。
この回ははじめてのSAにおけるRTLでしたので、上演プログラムは公募ベースではなくチェア（長谷川勇 氏，Luminous Productions／スクウェア・エニックス）による推薦ベースで実施されました。
日本からはスクエニやバンナムさんがR&D成果として、リアルタイムグラフィックスやAIによる対話的アニメーションや素晴らしいモーションキャプチャシステムのデモを発表しました。GREE VR Studio Labは、はじめてのリアルタイムライブデモ
「“REALITY: Be yourself you want to be” VTuber and presence technologies in live entertainment which can make interact between smartphone and virtual live characters（REALITY：『なりたい自分で生きていく』スマートフォンとバーチャルライブキャラクター間でやり取りできるVTuberとライブエンターテイメントのプレゼンス技術）」
を発表しました @<bib>{SA18RTL}。
この発表はVR Studio LabとWFLEに加えて、ニュージーランドでモーションセンサーに使える新素材を開発しているStretchSense社と、イギリスでソフトウェアによるモーションキャプチャー補完ミドルウェアを開発しているIKINEMA社と共同で実施しました。シナリオはすべて日本語と英語のバイリンガルで構築し、
ラボディレクターである筆者が自らVTuberキャラクターを設計して演じ、iPhoneXによる表情モーションキャプチャーと特殊表情@<bib>{CCSE2019}の可能性、StretchSense社によるグローブ、UnrealEngine4による統合されたリアルタイムキャラクターアニメーションとバーチャルギフトの可能性とおもしろさを伝えました。

挑戦としてはIKINEMA社による女性アクターと、リアルタイム・アニメーション・クリーニング・パイプラインによるライブデモ、さらに新規でデモ用キャラクター「今居＝リアリテ＝レア（IMAI Lea）」と「IKINEMA Aya」もデザインし、ダンスシーンもあり、けっこう大変でしたが得るものも大きな挑戦でした。
海外メディアからも取材や事後レポートがあり@<bib>{FXGUIDE2018}、現在もVR Studio LabのYouTubeでは人気のコンテンツになっています（@<href>{https://www.youtube.com/watch?v=zj2fe4A87GA}）。
なお、IKINEMA社は本件に関してプレスリリースを発行しています@<bib>{SA18RTL-IKINEMA}
（しかしその半年後にはAppleに買収されてしまいましたので、世の中なかなかうまくいかないですね）。

#@# //image[SA18-2355][SIGGRAPH ASIA 2018 TokyoでのReal-Time Live!の様子（SA公式提供）][scale=0.5]{
//image[SA18RTL][SA18RTLでのライブデモの様子（SIGGRAPH ASIA 公式提供）。東京国際フォーラムのHall Cを埋め尽くす観衆（と大量の野良Wifiアクセスポイント）。][scale=1.0]{
//}
//image[SA18RTL2][左の緑（Lea）が筆者です。ライブのダンスとiPhoneXによるライブの表情や特殊表情などをリアルタイムで披露しました。Studio LabのYouTubeでは人気のコンテンツになっています（@<href>{https://www.youtube.com/watch?v=zj2fe4A87GA}）][scale=1.0]{
//}


SA18RTLに続き、夏のSIGGRAPH2019においては、世界でのVTuber業界を再定義するべく「Virtual Beings World」という国際ミートアップイベントを主催し（@<href>{http://j.mp/VBW19Slides}）、世界13社によるVTuber／Virtual Beings関係業界によるピッチと、「World VTuber Showcase」というティザー動画コンテスト（@<href>{http://j.mp/WVS19}）を主催しました。
そこで世界中から集まった22作のVTuber中からのベスト作品として、
NTTドコモ「直感×アルゴリズム♪」がベストインタラクティブ賞＆グランプリとして選出されました。
副賞は「VTuber文化のアンバサダーになれる権利」、これが2019年11月にオーストラリアで開催されるSA19RTLにおけるコラボレーションのプロジェクトの始まりでした@<bib>{VRON20190809}。

//image[S18VBW][SIGGRAPH 2018 で開催した「Virtual Beings World」の様子。大入り満員でした。]{
//}

=={a-label} Virtual CastとHapbeatを使った国際双方向アバター触覚ライブの開発

以上のような経緯から、SA19RTLにおいては、「Virtual Beings World」で共有された「VTuberによる世界への挑戦」をさらに固めていくべく、
NTTドコモ副島義貴さんと、ラボの発信活動でご協力いただいている産業用VRのスタートアップ企業・イクスアール株式会社COO小池健輔さん、そしてVR Studio Labのインターンであり、東京工業大学発スタートアップ企業「Hapbeat合同会社」の代表でもある山崎勇祐さんと共著する形で企画提案をエントリーし、無事に採択されました（@<href>{https://sa2019.siggraph.org/attend/real-time-live/session/111}）@<bib>{VRON20191119}。

タイトルは「Global Bidirectional Remote Haptic Live Entertainment by Virtual Beings」で、VTuberによるグローバルな双方向触覚ライブです。

//image[SA19RTLconcept2][NTTドコモ副島Pのご協力によって華やかになったキービジュアル（左）と、ミルキークイーンさんの参加で一層華やかになったコンセプト画像の一部（右）][scale=1.0]{
//}

メインキャストは「生アニメ」として人気の「直感アルゴリズム」から、
最終的なキャストとしては「直感アルゴリズム」のKilin（リンリン）さん（法元明菜さん）、Xi（シー）さん（岩井映美里さん）が日本語と中国語を担当、そして英語担当として先述のWorld VTuber Showcaseでご縁のあった「お米アイドルMaiプリンセス」からミルキークイーンさん（尾車Rいのりさん）が英国系英語のナビゲータとして参加することとなりました（マネジメントとスタジオを協力いただいたジャストプロ・見野歩さんありがとうございました！）。




=== 目的設定・コンセプト・実現したい機能
まず、この技術デモの目的設定です。
複数人のVTuberのパフォーマンスを国際的なクラウドネットワークでつなぎ、
触覚ディスプレイを使用して「次世代の双方向バーチャルライブエンターテイメントの可能性を示す」というところに目標を設定しました。

コンセプトとして「5G時代に期待されるようなMEC（Mobile Edge Computing）」を意識して、出演者は東京で3Dのリアルタイムキャラクターをライブで演じ、オーストラリアのブリスベンでSIGGRAPH ASIAの視聴者とスクリーン越しに交流します。
また東京側の出演者は触覚ディスプレイを介して会場の拍手と歓声を可視化しつつ感じることができるようにします。

そしてVTuberライブとしてぜひとも挑戦したかった要素が「エモーションの共有」です。
先述のとおりSIGGRAPHは、コンピューターグラフィックスとインタラクティブなテクニックの専門会議です。
多くの聴衆はグラフィックスで素晴らしい経験をするはずですが、このデモは次世代のエンタメとしてグラフィックスに加えて、
さらに一段高い魅力的な要素が必要です。
国際イベントにおける多様な聴衆のために、エモーションを分析して、出演者・参加者全員で共有したい。
ネットワーク越しのグローバルな双方向インタラクションとVTubers用の触覚ディスプレイは技術的にも、
インターネットの歴史の上でも新しく、
（NTTドコモさんが関わる上でも）「5G時代のネットワークライブエンターテイメント」として魅力的な挑戦になるはずです。

理想をいえば、拡張された人間のライブの演技と触覚エンタメによって、距離、言語、文化、オーディオ、視覚障害の障壁も解決していきたいところです。

//image[REMOPREconcept][REMOPRE - Vibeshare初期の概要設計。VCはVirtualCastの略。]{
//}

とはいえ機能は``全部盛り''にはできませんので、以上のような構想から、
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
これを実現するシステムはコードネーム REMOPRE、後に「Viveshare」と名付けました。
次世代ライブエンタテイメントのための実験的な技術開発が始まりました。


=== エンジン選択とチーム編成

「直感アルゴリズム」を出演させる。
そして双方向触覚を実装する。
さらに今回のデモを成立させる。
``全部盛り''の状態から戦略的な取捨選択を行いました。

まずはエンジン選定です。
2018年のようにWFLE PTグループで開発しているUnreal Engine 4（UE4）ベースで開発されているスタジオシステム「vLiveStreamer」を続けてRTLのデモに使用することは一見合理的な判断です。
しかし今回は挑戦として、また今後の技術の展開可能性を考えて、スタジオシステムはUnityベースで開発されている「VirtualCast」（バーチャルキャスト、以下VC）を採用することとしました。
この判断はドコモさんとVirtualCast社さんのご協力によるところが大きいです。
外部のVTuberを活躍させるという意味で、外部のエンジンを使う挑戦、そしてVRMやVCIといったアバターを活用する技術をどこまで使えるのか？が経験になるはずです。
なお、VCはエンタープライズ版を中心に開発を進行させていただきました（コンシューマ版でも機能するように開発しています）。

この時点での座組は、NTTドコモ、バーチャルキャスト社、ジャストプロ、EXR、Hapbeat、そしてWFLEとGREE VRStudio Labという7社横断のスーパーチームです。
昨年は日本語と英語のバイリンガルですすめるコストがありましたが、今回はシナリオと実演以外の開発作業は日本語で進行できます。

これをフォーミュラカーのチームに例えれば、スポンサーとドライバーとエンジン選択とチーム編成が決まった状態です。
磨くべき技術も見えてきたので、これでやっとR&Dとエンジニアリングが始められそうです。

//image[SA19RTLteam][SA19RTLチームフォト、2019年10月8日・六本木ヒルズにて撮影。][scale=0.7]{
//}

=== Hapbeatを使用した国際双方向触覚ライブ

次に触覚のハードウェアとアルゴリズム選択です。
近年、触覚デバイスはVRエンタメの世界を中心にさまざま登場していますが、
「Hapbeat」は糸の張力を使った触覚デバイスで、音響信号から多様な触覚刺激を生起できる、かつ3万円程度で手軽に利用できる国産デバイスです。

//image[hapbeatduo][今回採用した触覚デバイス「Hapbeat-Duo」（ @<href>{https://booth.pm/ja/items/1549687} より購入可能）][scale=0.5]{
//}

写真@<img>{hapbeatduo}は、Hapbeat社による最新モデル「Hapbeat-Duo」で、左右の紐を別々に制御できるモデルです。
制御は音声信号のLRを使用して行い、ステレオ音源の左右の音量比がそのまま振動の出力比となります。
非対称な振動を提示することで方向や距離など、表現できる情報が非常に広く、効果音のような短い振動用の波形を挿入することで、
左右交互にトントン、とした打感を提示することも簡単にできます。


//image[HapticMinesweeper][目に見えない物体の方向を感じられる「Haptic Minesweeper」体験の様子（@<href>{https://www.youtube.com/watch?v=AgmFWBu5ZnM} より）][scale=0.5]{
//}

VR Studio LabではこのHapbeat-Duoをベースに、VRゲームやライブエンタテイメントなどに利用できる触覚アルゴリズム多数開発しています。
たとえばHMDを装着したユーザーに距離や方向を触覚で提示することもできます。
「Haptic Minesweeper」として、IEEE World Haptics 2019で発表しており@<bib>{SIG19Yamazaki}、UE4とOculus Questで動作するデモを動画公開しています（@<href>{https://www.youtube.com/watch?v=AgmFWBu5ZnM}）。
触覚についての基盤技術は開発済みですので今回は新しい挑戦として「国際双方向触覚ライブ」に挑戦します。
このHapbeat-Duoを使い、オーストラリアと日本に分散して存在する出演者と、聴衆をつなぐ挑戦をします。


=== Virtual Castと距離への挑戦

東京と今回の開催地であるオーストラリア・ブリスベンは直線距離で 7,154km 離れています。地球の半径よりも遠いです。

//image[SA19RTL-map][東京と開催地のオーストラリア・ブリスベンは7,154km離れている。そして Virtual Cast の推奨環境はYouTube動画ストリーミングだと4K動画相当。][scale=0.8]{
//}

そして VC の推奨環境は「上り下りとも 50Mbps以上 、レイテンシ 10ms以下、パケットロスなし」という情報がバーチャルキャスト社より公式情報として伝えられました。
YouTube動画ストリーミングだと4K動画に推奨される帯域が20Mbpsなので、それ以上の帯域が必要ということになります（@<href>{https://support.google.com/youtube/answer/78358?hl=ja}）。
なおVC内部の通信はモノビットエンジンを使っているようで（@<href>{https://monobit.co.jp/vtuver/}）、これについてはさすがゲーム用エンジン。さまざまな環境においても安定していました。
パケットロスなし、という推奨環境ですが、おそらくモーションについてはパケットロスしたときは補完されるようです。

一方で、VC内部で使用している音声エンジンは課題が多いと感じます。
VRChatほどではないですが、音声品質と同時性保証の面では、ライブエンタメというよりは、ニコニコ動画のような完全にネット視聴型の配信プラットフォームにおける「バーチャルステージとしての統合」という設計思想を感じます。
なおSA19RTLと並列の時間進行で、VC側も新しい仕組みを試しているところでした（@<href>{https://virtualcast.jp/blog/2019/10/virtualcastroadmap/}）。
詳しい話は割愛しますが、最終的な判断には音声の同時発声と音声品質、通信帯域、そして装置構成やスタッフの手の数も含めた技術的安定など複雑な要素が絡み合い、難度が高い要素でした。
#@end




== 本番当日に起きたこと ＆ 今後に向けて
//image[REMOPRE1117][最終的なシステム構成（2019年11月17日版）]{
//}

さて、システムは完成しました。
SA19公式のトレイラーにも収録されたりもしました（@<href>{https://www.youtube.com/watch?v=IW_ycfh80P0}）が、
その後もけっこう大変な進行で、
特に日豪同時公開リハーサルを11月17日現地時間12～16時（日本11～15時）に実施し、日本での取材対応などこなしながら
ひたすらダンスの練習（オリジナル振付）、
そのまま羽田空港からオーストラリアのフライトへ。
機材がクソ重い、さらに真冬のコートで移動して、オーストラリアについてみれば現地は真夏のクリスマス。体力が削られるだけでなく、ホリデーな雰囲気に精神も削られます。

現地ではリハを受け取ってくれた先発隊と合流し、11月20日11～13時が最終リハーサル、その後13～15時が別の上映があり会場閉め切り、
さらに15～16時が本番準備、そして16～18h時が本番ライブ（日本時間15-17時）という流れでした。

とはいえ当日の朝まで寝ずの作業をしてたように思います。
まず17日のリハーサルの段階で明らかになったのが演者の音声の問題。
直前まで複数の方法で構成を検討していたのですが、2つの問題が出ました。
「会場側のエンジニアから会場音と、出演者のマイクが拾ってしまう環境音のハウリングやローカルループが消せない」、
「リモート（東京）側の音声が何をしゃべっているのかよくわからない」。

シナリオ自体は英語で作りこんでいたのですが、どうも我々の聞こえる英語の周波数と、オーストラリア現地の方々のチューニングがあっていない感じです。最終的には演者間のコミュニケーションはDiscord Nitroを選択し、こちらはオーストラリアサーバーで設定していたので、最高音質ではあったのですが、どうしても国際イベントには国際英語でゆっくり明瞭に話す必要があります。北米英語でもイギリス英語でもオージー英語でもなく、国際英語。これに会場の音響環境まで含めた「プロの音響エンジニアの耳」で、すべての系を含めたマイクの調整などを行っていく必要がありました。
音声はVCを使わずに、Discordを使う？楽曲だけ別の音源を再生する？
音声品質を追い込むばかりに、構成を変更せねばならず、どんどん遠のくゴール。

音声問題を引きずりながら（一方では会場が使える時間は限られており）、かつ他のチームもトラブっていて非常に難しい状況でした。
そして、直前の直前。OMENが謎のCPU高負荷を起こして再起動もできないような状態に陥りました。

3,000人の観衆。本番直前の不測のトラブル。すごすぎる他の発表者……。
さすがに「もうだめかも…」という気持ちで本番を迎えました。
実際には、ほとんど動作していないOMENのHMDを捨てて飾りにして、バーチャルキャスト内カメラマンが使うMainPCからHMDのケーブルを伸ばし、浄瑠璃というより二人場織のような状態で演技をすることになりました。
耳には緊急用のDiscord（iPhoneで接続）、本番中で他の発表者が発表中なので、うかつな会話もできない状態です。

そして本番が始まりました……！

//image[wtg2][（左）見事に成功した歓声と拍手メーター（画面左右）、（右）矢が刺さった瞬間のノケゾリ（@<href>{https://www.youtube.com/watch?v=yTrDRKazksM} より）]{
//}

当日の状態は再編集してYouTube動画として公開してありますので是非ご参照ください（@<href>{https://www.youtube.com/watch?v=yTrDRKazksM}）。
見事に成功した歓声と拍手メーター（画面左右）、そしてウィリアムテル・ゲームの矢が刺さった瞬間のノケゾリ（演技ではなく触覚による反射）、そしてラストの歌「520」とダンス…。

音声トラブルは発声し、会場のMAさんによって楽曲が途中でミュートされてしまいました。
会場の聴衆には気づいていたかどうかわかりません。
HMD越しでしたが、会場の笑いや拍手からも、観客との一体感は間違いなく高いものでした。
最後の「520」の楽曲が鳴っているとき、何百回と練習した私の体は見事に他の3名の演者とピッタリ合っていました。
東京とブリスベンの距離があっても、ディレイがあっても、不可能は可能になりました。
しかし、音声はどのような状態になっているか分かりませんでした。
おそらく日本側の音源と会場側の音源が二重に鳴っており、会場の音響さんにミュートをかけられてしまったようです。

会場の音声が切られたとき、私は『しまった』と心底思いました。

しかし、VC内では他の出演者、直感アルゴリズムのシー（声：岩井映美里）さんの歌声が聞こえてきました。
『え……この声は……（楽曲と被るから）会場に聞こえてはいけないのでは…』という考えも頭をよぎりましたが、
『もう、踊るしかない！』と私はHMDの中で泣きそうになりながら無言で踊り続けました。
そのシーさんのアカペラとダンスにあわせて、なんと、会場から手拍子が……！

……いろいろありました。残念ながらベストな状態でのパフォーマンスができなかったことが心のこりですが、
そろそろ長すぎるような気もするのでこのへんで筆を置きたいと思います。

#@# Matt AI - Speech Driven Digital Human with Emotions http://sa2019.conference-program.com/presentation/?id=real_106&sess=sess230 Jingxiang Li, et al. Tencent Technology Company Limited

== あとがきにかえて
結局のところ、シナリオのおもしろさをレビューするのも大事だし、設計のレビューも、音響周りのレビューも、技術のライブデモはほとんど技術なのでした。　

残念！トロフィー持って帰りたかった！くやしい！
でも複合した技術をうまくデモできたのは事実ですが、音声トラブルがあってシナリオがしっかり聞いてもらえなかったのも事実だし、審査員賞を取ったパフォーマンスもホント凄かった！
これは是非とも次回につなぎたい学びでした。

#@# というわけで次はの挑戦は夏のSIGGRAPH（ワシントンD.C. 8/19-23）で会場投票一位を目指して頑張るか、中国ライブかな……とはいえ「同じことは二度とやらない」のがラボのポリシーでもあるので、技術的チャレンジも盛り込んでがんばっていきたいと思います!

次回のSIGGRAPHはワシントンです。
でもオリンピックも直撃の日程なので、なかなか無理はできなさそうです。
その次の2021年のSAは東京なので、ここに向けてどんどんパワーアップして、世界中を驚きと笑いでひっくり返したいですね！

以下、共演者さんたちへのメッセージです。
ミルキークイーン（声：尾車Roseいのり）さん。
いつも元気で知的で英語シナリオの監修までやっていただいたミルキーさん。本当にありがとうございました。
我々がなぜバーチャルビーイング（virtual being）なのか？という本質的な問いについての深いシナリオまでできていたのですが、残念ながら本番は声が元気すぎてマイクが死んでしまったものと推測します。
今後、再挑戦する機会を是非とも作りたい！ぜひご一緒できましたら幸いです。

リンリンさん／法元明菜さん。
いつもよく気がつく努力家なリンリンさん。作って頂いた振り付け がとてもよく、徹夜で練習しまくりました。本番では音が飛んでいたのに踊り続けられたのはこの特訓のおかげ！
そして自己紹介パートのステキなライブイラストありがとうございました！

シーさん／岩井映美里さん。いろんなご都合で本番直前のみの絡みとなり「いつもマイペースで何を考えているか分からないところがあるよ～」という情報だけは頂いていたので実はちょっぴり不安があったのですが、そんなことは杞憂でした。
本当に声もキレイだし、芯がしっかりした人物なのだと感じました。特に最後のダンスシーン！会場の音響がトラブルで切られており、しかもバーチャル内ではそのトラブルがわからない状態だったのに、最後まで歌い続けて踊りもしっかり...。会場は何が起きているのか、これがライブなのか！とわかり手拍子から拍手の渦...マイペースって大事だな！と尊敬すると同時に、やさしい観衆に今思い出しても泣きそうになります。

スタッフの皆様。
チェアの皆さんや他のReal-Time Live!発表者にもインタビューしてみたのですが、やっぱり会場をもっとも沸かせたのはウチの発表でした。会場熱気分析Vibeshareシステムもみごとにメーター振り切っていました。会場投票が無かったのは残念ですが、やはり他の発表者もすごかった。
これに懲りず、これからもVTuber産業を世界に発信していきましょう。


#@mapfile(shirai/shirai-working.re)
#@end

#@mapfile(shirai/column.re)
#@end
