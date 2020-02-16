
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

===パフォーマンスの設計
//image[TokyoBrisbane][SA19RTLティザー動画より。左側の2人は東京、右側はブリスベンにいるという設定。1ショットの画像でわかりやすくするために本番前の実験を繰り返し、絵コンテ・レイアウトレベルで調整を重ねる。]{
//}

まずは当初の提案レベルの暫定シナリオです。原作は英語ですが「7分にどう詰め込むか」という視点で詰め込んでいきます。

//tsize[10,90]
//table[scenario1][当初提案シナリオ（7分）]{
時間	内容
-------------------------------------------------------------
0-1分	[はじめに]各言語で挨拶と自己紹介。メインMCは会場（ブリスベン）にあり、他のキャラクターとスタジオは東京にあることを伝える。
1-2分	[私たちは離れている]ビデオ会議風のビジュアルで、スマートフォンで現地時間とGPS（Googleマップ）を表示。ストリーミングのラグを表示（予測では片側で20〜40秒の遅延がある）。
2-3分	[双方向リモート触覚ライブ]オーディオチャネルを使用したHapbeatエクスペリエンスをデモ。メインMC、審査員など一部の観客がHapbeatデバイスを装着しYouTubeライブストリーミング視聴。じゃんけんをします。
3-4分	[安定性、互換性、および利便性] YouTube Liveパイプラインとの適合性と互換性を示す。
4-5分	[多言語での感情との相互作用]リアルタイムの翻訳で観客の顔、拍手、歓声を分析できるリアルタイムの感情分析を使って、視覚化されたデータとリアルタイム翻訳で会場の観客全員と交流。
5-6分	[ダンスプレイ]フィナーレ。ダンスプレイで観客を次世代のライブエンターテイメントに連れていく。
//}

その後、採択時に審査員からのフィードバックがありますが
「この提案はまさにRTL向きだね！」という感じで、おおむね好評価。
その後は、各要素のフィージビリティや意味合い、最終的な持ち時間（各チーム10分）にあわせて、
パフォーマンスの構成要素を再設計していきます。
採択からチーム編成までの期間（本番10週間前）で、VCを活用することが決定しましたので、
9月10日ぐらいの段階でこんな感じのシナリオになりました。
ちなみにこのような進行の構成要素と時間配分を記載した台本を「構成台本」と呼びます。

//tsize[10,90]
//table[scenario2][本番10週間前時点のシナリオ（10分）]{
時間	内容
-------------------------------------------------------------
0-1分	あいさつ
1-2分	システムについて（豪州側バーチャルキャストに日本から凸する）
2-3分	ミルキークイーンが英語で通訳しながら直感アルゴリズムKirinが日本語、Xiが中国語（計3人）がVTuber文化とファンとのコミュニケーションについて語る
3-6分	パオズゲーム：白井が「会場で拍手をすると画面端のメーターがあがります」と紹介。右ウイングと左ウイングで拍手がなるたびにパオズが落ちてくる。「多いほうが勝ち！」その後「笑いでも何か落ちてきます」。
5-6分	バーチャル・ウィリアムテル：日→豪で頭上のリンゴを弓で射る。
6-7分	六本木で開催されたTIFFのようす（VR-HMD-5Gライブの記録ビデオ）を紹介、楽曲「520」を踊りエンディング。
//}

このあと複数回の技術検証、出演者どうしのリハーサル、ゲネプロ（最終リハーサル）を通して、英語版で完成させていきます。

//image[scenario][シナリオ決定稿。画像化してVC内でカンペとして表示するため1枚に凝縮してある。]{
//}

VTuberのシナリオを技術書で解説する機会はなかなかないのですが、最終的な台本は（アニメ声優と違って）紙の本ではなく、画像やスプレッドシートで扱われることは多いようです。

=== VCI を使ったVirtual Castの拡張

VCはそのバーチャル空間に独自の背景や3Dオブジェクト、またそれらにインタラクティブ性を持たせるために独自のスクリプトシステムを有しており「VCI」（Virtual Cast Interactive）と呼ばれています。
VCIはバーチャルキャスト社公式が配布しているUnityPacageを導入してUnity上で3Dオブジェクトをセットアップし、スクリプトはLua言語を使って書くことができます。
#@# https://virtualcast.jp/wiki/doku.php?id=vci:script:luatutorial

上記のシナリオに現れる「パオズゲーム」や「バーチャル・ウイリアムテル」は、「直感アルゴリズム」番組中ではVCIを使って実装されています。

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

コード的にはLuaなのでこんなかんじです（例として紙吹雪を降らせるコードになります）。

なお、VC自体の設計思想は「VR」というよりは
「HMDを使って演じるバーチャルな3Dキャラクターによる放送局のためのシステム」ですので、
VCIは主にグラフィックスについてのインタラクション、具体的にはシーングラフと衝突、ユーザーによるグラブアクションなどが取得できますが、
外部システムやデバイスとの通信はできません。
この先は必要に応じて色々な魔改造?を施していくことになります。

== 実装と解説

シナリオと技術的な実装要素を図にするとこんなかんじになります。
メインMCのハードウェア装備は、（1）背中にOMEN X、（2）HMDはOculus S（会場の設営コストと安定性を配慮して）、（3）肩の左右に触覚提示をわかりやすくするインジゲーター、（4）胸にHapbeat-Duo（HMDのオーディオ出力からベルトにアンプを接続）、（5）電飾を制御するM5Stack、という構成になります。

//image[SA19-scenario][（左）シナリオと技術的な実装要素／（右）筆者・メインMCのハードウェア装備(SA19RTLeq.png)]{
//}

本会場でのPC構成は主に3台。メインMC用のOMEN、会場音響を分析するMicPC、そして会場音響からOBSを使って盛り上がり分析画像をグラフィックスとして付加するMain、さらに東京側には各出演者に1台づつVC端末が用意されています。
続いて、個々の技術についての実装紹介になります。

=== 会場音声分析からのギフト送信
まず、会場音声分析からのギフト送信を解説します。
会場の歓声や拍手、笑いなどによって、VCIのアイテム（パオズ、紙吹雪など）をバーチャルギフトとして降らせることがゴールです。

会場の音響を分析する音響分析用PC（GPD Pocket2、以下MicPC）を用意し、Python環境で開発した音声分析システムを使い、その分析結果をWebSocketを扱うことができるNode.jsのライブラリ「Socket.IO」を使ってJSON形式にて送受信させます。

会場からの音声分析系はラボのインターン、栢之間さん（東工大2年生）が中心になってPythonによる音響分析を実装しています（感謝）。
いろんなノウハウがあるのですが、シンプルにコードで表現するとこんな感じです。

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
STFTは short-time Fourier transform 、短時間フーリエ変換です。
MFCCはメル周波数ケプストラム係数（Mel-Frequency Cepstrum Coefficients）という
聴覚フィルタに基づく音響分析手法で、人間の聴覚特性にあわせたフィルタバンクを使って高速に音響の特徴を抽出できます。
結果として applause, laugh, excitement つまり、拍手のような喝采、笑いと興奮を抽出します。単純に拍手だけ判定することも可能です。

上記のアルゴリズムを共通にして、各システムの通信は Node.js を使って行います。

//list[MicPC-index.js][index.js（MicPC）]{
const { PythonShell } = require('python-shell'); //pythonをnodeから起動する
const socketio = require('socket.io')
app.use(express.static('public'));
const expressServer = app.listen(3000); 
const io = socketio(expressServer); 
var python_script_path = '../../EmotionAnalysis/'; //実行するアルゴリズムの場所

let pyEmoAnalysis = new PythonShell(
    'main.py', { mode: 'text', pythonOptions: ['-u'],
    scriptPath: python_script_path });

//EmoAnalysisからデータが飛んで来たら実行
pyEmoAnalysis.on('message', data => {
    obj = JSON.parse(data)
    console.log(obj)
    // client側にデータを送信
    io.emit('send_EmoAna_Result', obj) 
})
//}

Socket接続をし、上記index.jsから送られてきた解析結果を同Socketに存在する別のclientに送信します。
何らかのブラウザで指定のURL（http://localhost:3000）を開き、localのindex.jsと接続するためのSocketを作っています。

//list[client.js][client.js（MicPCで走るNode.jsスクリプト）]{
const LocalSocket = io.connect('http://localhost:3000');
const MainSocket = io.connect('https://main.jp.ngrok.io/');
const OMENSocket = io.connect('https://omen.jp.ngrok.io/');

// index.jsから送られてきた解析結果をそのまま接続されているsocketのclientに送る
LocalSocket.on('send_EmoAna_Result', (obj) => {
    console.log(obj)
    MainSocket.emit('send_EmoAna_Result', obj)
    OMENSocket.emit('send_EmoAna_Result', obj)
})
//}

#@# //cmd{node index.js//}

Main,OMENそれぞれのPCで node index.js を起動し（おススメは VS Code のコンソールで投入します）、ブラウザでURLを入力することで、各PC上で必要となる可視化や通信が行われる仕組みになっています。

=== VCI と node.js を使ったVirtual Castの拡張

前述のとおり、VCおよびVCIには外部システムとの通信機能はありませんが、唯一、デバッグ情報をWebSocketで受け取ることができます。
oocytanbさんの vci-logcat というプロジェクト（
@<href>{https://github.com/oocytanb/vci-logcat}
）が詳しいのでここでの詳説は割愛します。
こちらを使ってVCI→他のシステムへの通信はWebSocket通信を使って実装します。

逆に他のシステム→VCIへのイベントは、キーボード信号を使って実装することにします。
具体的には pyAutokey という Pythonライブラリを使って、VCを使っているMainPCにキーイベントを発生させて、MainPC上のVC側ではそのキーイベントを拾います。

//list[MainPCindex.js-key][自動マウス＆キー操作（node.js）]{
let pyAutoKey = new PythonShell('autokey.py',
 { mode: 'text', pythonOptions: ['-u'], scriptPath: './' });
// 自動キー入力を行う閾値
var Hi_Threshold = 0.7
var Lo_Threshold = 0.4

//io.on→socketが接続されたとき起動
io.on('connection', (socket) => {
    // 解析結果が飛んで来たら実行
    socket.on('send_EmoAna_Result', (obj) => {
        var Applause_fromMax = 0.13
        var Laugh_fromMax = 0.02
        // まず得られた解析結果を0 ~ 指定した最大値の範囲（範囲A）に限定する。
        // その後、範囲Aを0~1の範囲にマッピングする
        obj.L_L = map(value_limit((obj.L_L - 0.001), 0, Laugh_fromMax), 0, Laugh_fromMax, 0, 1)
        obj.L_A = map(value_limit((obj.L_A - 0.04), 0, Applause_fromMax), 0, Applause_fromMax, 0, 1)
        // マッピング後の解析結果が閾値を超えたらマウスを(100,100)に動かし、指定のキー入力を動作させる
        if (obj.L_L > Hi_Threshold) {
            pyAutoKey.send(String(2))
        } else if (Lo_Threshold < obj.L_L && obj.L_L < Hi_Threshold) {
            pyAutoKey.send(String(2))
        } else {
            pyAutoKey.send(String(4))
        }
        if (obj.L_A > Hi_Threshold) {
            pyAutoKey.send(String(1))
        } else if (Lo_Threshold < obj.L_A && obj.L_A < Hi_Threshold) {
            pyAutoKey.send(String(1))
        } else {
            pyAutoKey.send(String(3))
        }
     });
});
//}

キーボード入力の[1]が紙吹雪、[2]がパオズ、[3]、[4]がVCI内のHapbeat（に似せた3Dオブジェクト）の帯の色を変更します。
なおこのpyAutokeyを起動後はマウスとキー入力を奪われるので、本番以外はコメントアウト推奨です。
止めたいときは、Alt + Ctrl + Delete を押せば止まりますが、知らない人が触るとパニックになります。

//list[MainPCindex.js][自動マウス＆キー操作（python）]{
import pyautogui as pgui

def testKey(key):
    pgui.click(100, 100)
    pgui.typewrite(key)

def main():
    while True:
        key = input()
        if key == "1" or key == "2" or key == "3" or key == "4":
            testKey(key)

if __name__ == "__main__":
    main()
//}

以上のようなキーボードをハックする処理は、複数のシステムに横断する処理をどうしてもネットワークで実装できない場合（他には共有ファイルの書き込みなどでも実装できない場合）の最終手段としての実装になりますが、
意外とVRエンタメシステムやVTuberなどのスタジオ技術の実装ノウハウとしては便利なので覚えておくといいかもしれません。
なお、AmazonではUSBキー入力を発呼できるフットスイッチなどもありますのでシステム化するときにも便利です。
なおデバッグするときもキーボード入力で試せますが、本番ではチートになりますので注意です。
一方で、充分なテストをしておかないと、本番では「人間では押せないぐらいの連打で信号が来た！」なんていうことも起きますので、そちらも注意が必要です。

