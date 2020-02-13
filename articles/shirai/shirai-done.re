
== VTuberライブエンタメ技術に求められる研究開発

GREE VR Studio Labは2018年からグリーグループに設置されたVTuber（バーチャルユーチューバー）やVRエンターテインメントシステムにおける研究開発、企業間連携、研究交流発信事業を担当する研究開発組織です。
現在、グリーが注力している分野のひとつであるVTuber事業は、アニメーションやゲーム、ライブ配信など、多岐にわたるエンターテインメント技術が必要とされる分野です。

それらを構成する技術要素はゲーム開発におけるリアルタイム3Dグラフィックスや、モーションキャプチャー、サーバー技術などがあり、加えて、ライブ・エンターテインメントに適したモーション生成や、演者支援、ディスプレイ技術、インタラクション技術、人工知能応用、キャラクター設計、エモーション分析、視聴者文化の調査分析など多岐にわたっています。
（もちろんエンジニアの皆さんは日々このような開発を専門的に取り組んでいることは間違いないのですが）
このような新しい複合横断分野の研究開発は、既存のミッションクリティカルな単一企業内R&D手法だけでは、個々の課題は個々の技術やソリューションで解決できても、そもそもの問題設定や異なるユーザーを巻き込んだ社会としてのゴールが明確ではない課題が多くあります。
「社会実装」と呼ばれる技術のデプロイメントがそこにあり、産学連携やスタートアップ企業との連携などを通して、最先端の技術や手法を実装していくことが世界的なスタンダードとなりつつあります。

グリーグループでライブエンターテインメントを推進するWright Flyer Live Entertainment（以下、WFLE）には、主にプロダクション事業、プラットフォーム事業、ソリューション事業があります。プラットフォーム事業の中にもスマホアプリやインフラだけでなく、Presence Technology Group（以下PT）という、VTuber番組のための製作技術、スタジオ技術を中心に開発する組織もあります。
WFLEの組織図について、カジュアルな公式資料としては事業代表者の荒木英士（DJ RIO）によるブログをご参考にいただけると幸いです@<bib>{Wantedly}。

//image[WFLEPT-GVRS][WFLE PTとGREE VR Studio Laboratoryの位置づけ]

このようなまさに新しい産業を掘り起こしていく体制の横で、弊ラボ「VR Studio Laboratory」は、VRゲームなどを企画、開発、配信してきたGREE VR Studioと併設され、
（1） VTuber関連技術のR&D促進、（2） 異業種R&D連携強化、（3） 業界振興・イノベーション型人材の支援発掘や育成、
の3つを基本方針として活動しています。
WFLEが開発するVTuber専用ライブ配信プラットフォーム「REALITY」開発チームのもつ知見や経験を、日本国内外の大学・企業・研究機関などの研究者や学生等とのコミュニケーションを通じて深め、共有し、知財開発およびスピード感のある社会実装、品質向上、技術発信に貢献しています。


具体的には長期スパンのR&D、知財創出、アライアンス、新技術の調査選定、国際展開、普及啓蒙事業、社会実装を通した実験などがあります。
（本稿で扱いませんが）千葉大教育学部付属小学校でのVTuberを使った実験授業 @<bib>{CSR20190322}や、大学でのプログラミング講義向けアバターの教育向け応用（引用@<bib>{RexIVA}）、またエンタメサービスを通した実験としては無料ボイチェンサービス「転声こえうらない」（@<href>{https://vr.gree.net/lab/vc/}）のリリースがあります。
もちろんライブエンターテインメントの研究所らしくTwitter @<href>{https://twitter.com/vrstudiolab,@VRStudioLab}や、YouTubeチャンネル（@<href>{http://j.mp/VRSYT} ）でも
VRエンタメ技術についてのビデオレポートやライブイベント、資料映像を発信しております。
またSlideShareでも資料を公開しておりますので（@<href>{https://www.slideshare.net/vrstudiolab/}）、ご興味ありましたらご笑納ご購読いただけると幸いです。

本稿では上記のようなWebメディアでは解説が難しいVTuber技術、特に次世代型のライブエンターテインメントで使えそうな話の中でも、特にエンタメコンテンツや一般的な読みものとしては``読者を選ぶであろう''技術的な裏話をしていきたいと思います。
とはいえ前述のとおり、
ラボで扱っている技術は膨大で範囲も広い一方、
WFLEのエンジニアさんたちは、インフラやスマホアプリの開発については非常に尖った技術を持った優秀な方々がおり、
私が筆を執るような立場ではありません。
興味がある人はこちらに採用サイトがありますので、まずはエントリーして中の人と対話してみたほうがよいと思います（@<href>{https://www.wantedly.com/companies/wrightflyer/}）。

「なりたい自分で、生きていく」が社是なので、波長が合う人にはきっと居心地がいいでしょう。

さてそんなわけで、本稿ではあれこれ考えた挙げ句、特に2019年11月に実施した「SIGGRAPH Real-Time Live!」について、技術的なチャレンジといくつかの実装を紹介していきたいと思いました。
あまりに扱っている技術が膨大で、また経験としても、非常に稀有な開発であり、かつ今後のVTuber業界や5G開発においてちょっとした知見になるかも？と感じたからです。

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

一方でSIGGRAPH内では学術論文発表や見本市といったベース構成は大きく変わらずに、従来の主力であったアニメーション・フェスティバルといった映像作品の上映会は年々規模縮小しつつあり、新技術のデモ、特にVRのコンテンツや技術デモは大きな規模に、かつ人気の展示となっています。
その中で、リアルタイムグラフィックスのデモとしては一番大きなセッションとして「Real Time Live!」（以下、RTL）が2010年に誕生しています。
これはリアルタイムグラフィックスを中心とするライブデモによるショーイベントです。
NVIDIAのようなGPUメーカーのR&D成果の発表や研究者による技術デモ以外に、ゲーム産業における具体的な開発手法のライブデモや、映画産業におけるPreVizといったライブプレビュー技術など、
各社がアクターを使ったり、熱のこもったプレゼンなどを行い、
いわゆる「技術デモ」というよりは「技術と派手さが求められるショー」を披露します。
会場も数千人が入る超巨大会場で、1発表あたりの持ち時間は10分以下という短さ。
一方で、来場者が歓声をあげたり拍手したり、といった反応もライブで感じられる注目のセッションです。

=== GREE VR Studio Lab の挑戦「SA18RTL」
振り返ること2018年12月の東京。SIGGRAPH ASIA Real Time Live!（SA18RTL）は東京国際フォーラム「Hall C」という客席1,500人のホールで開催されました。
この回ははじめてのSAにおけるRTLでしたので、上演プログラムは公募ベースではなくチェア（長谷川勇 氏，Luminous Productions／スクウェア・エニックス）による推薦ベースで実施されました。
日本からはスクエニやバンナムさんがR&D成果として、リアルタイムグラフィックスやAIによる対話的アニメーションや素晴らしいモーションキャプチャシステムのデモをする中、
GREE VR Studio Labは、はじめてのリアルタイムライブデモ
「“REALITY: Be yourself you want to be” VTuber and presence technologies in live entertainment which can make interact between smartphone and virtual live characters（REALITY：『なりたい自分で生きていく』スマートフォンとバーチャルライブキャラクター間でやり取りできるVTuberとライブエンターテイメントのプレゼンス技術）」
を発表しました @<bib>{SA18RTL}。
この発表はVR Studio LabとWFLEに加えて、ニュージーランドでモーションセンサーに使える新素材を開発しているStretchSense社と、イギリスでソフトウェアによるモーションキャプチャー補完ミドルウェアを開発しているIKINEMA社と共同で実施しました。シナリオはすべて日本語と英語のバイリンガルで構築し、
ラボディレクターである筆者が自らVTuberキャラクターを設計して演じ、iPhoneXによる表情モーションキャプチャーと特殊表情@<bib>{CCSE2019}の可能性、StretchSense社によるグローブ、UnrealEngine4による統合されたリアルタイムキャラクターアニメーションとバーチャルギフトの可能性とおもしろさを伝えました。

挑戦としてはIKINEMA社による女性アクターと、リアルタイム・アニメーション・クリーニング・パイプラインによるライブデモ、さらに新規でデモ用キャラクター「今居＝リアリテ＝レア（IMAI Lea）」と「IKINEMA Aya」もデザインし、ダンスシーンもあり、けっこう大変でしたが得るものも大きな挑戦でした。
海外メディアからも取材や事後レポートがあり@<bib>{FXGUIDE2018}、現在もVR Studio LabのYouTubeでは人気のコンテンツになっています（@<href>{https://www.youtube.com/watch?v=zj2fe4A87GA}）。
なお、IKINEMA社は本件に関してプレスリリースを発行しています@<bib>{SA18RTL-IKINEMA}
（しかしその半年後にはAppleに買収されてしまいましたので、世の中なかなかうまくいかない）。

#@# //image[SA18-2355][SIGGRAPH ASIA 2018 TokyoでのReal-Time Live!の様子（SA公式提供）][scale=0.5]{
//image[SA18RTL][SIGGRAPH ASIA 2018 TokyoでのReal-Time Live!の様子（SIGGRAPH ASIA 公式提供）][scale=1.0]{
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

「直感アルゴリズム」の出演と双方向触覚を実装する、
そして今回のデモを成立させる上で、``全部盛り''の状態から戦略的な取捨選択を行いました。

2018年のようにWFLE PTグループで開発しているUnreal Engine 4（UE4）ベースで開発されているスタジオシステム「vLiveStreamer」を続けてRTLのデモに使用することも十分に魅力的なのですが、
今回は挑戦として、また今後の技術の展開可能性を考えて、スタジオシステムはUnityベースで開発されている「VirtualCast」（バーチャルキャスト、以下VC）を採用することとしました。
この判断はドコモさんとVirtualCast社さんのご協力によるところが大きいです。
外部のVTuberを活躍させるという意味で、外部のエンジンを使う挑戦、そしてVRMやVCIといったアバターを活用する技術をどこまで使えるのか？が経験になるはずです。
なお、VCはエンタープライズ版を中心に開発を進行させていただきました（コンシューマ版でも機能するように開発しています）。

この時点での座組は、NTTドコモ、バーチャルキャスト社、ジャストプロ、EXR、Hapbeat、そしてWFLEとGREE VRStudio Labという7社横断のスーパーチームです。
昨年は日本語と英語のバイリンガルですすめるコストがありましたが、今回はシナリオと実演以外は日本語だけで進行できそうです（これが後で別の音声系の問題を生んだりもしました）。

これはフォーミュラカーのレースに例えれば、ドライバーとエンジン選択とチーム編成が決まった状態です。これでエンジニアリングが始められそうです。

//image[SA19RTLteam][SA19RTLチームフォト、2019年10月8日・六本木ヒルズにて撮影。][scale=0.7]{
//}
