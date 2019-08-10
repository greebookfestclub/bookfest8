
= この本の作り方
== Build環境
印刷所へはPDFで納品します。
まずは、ここにあるテキストを手元のPCでBuildして、PDF形式で出力します。

=== install docker desktop for mac
build用のdocker環境が用意されています。
これを使ってbuildするのが簡単です。
もし、"docker desktop for mac"がなければ、インストールします。
"docker desktop for mac"は、以下のサイトからダウンロードできます。

https://hub.docker.com/

=== build pdf using docker
この書籍のデータは、githubのopen repositoryで管理しています。
まず、このデータを取得します。
build用のdocker imageを取得し、それを使ってPDFを作成します。
すると、articles/GREE-BookFest7.pdf というファイル名でbuildしたPDFファイルが出力されます。

//list[BuildTemplate][BuildTemplate][shell]{
% git clone git@github.com:greebookfestclub/bookfest7.git
% cd bookfest7
% docker pull vvakame/review:3.1
% ./build-in-docker.sh
% ls articles/GREE-BookFest7.pdf
//}

== 執筆環境
執筆はRe:view形式で行います。
Re:viewフォーマットは、技術書典で配賦される多くの書籍で使われているフォーマットです。
そのため、執筆環境も整備されています。
ここでは、手元のPCのテキストエディタ環境を構築します。

=== atom環境構築
atomにredpenを入れることにより、問題のある部分は赤い表示がされるようになります。
もし、テキストエディタatomがなければ、インストールします。
atomは、以下のサイトからダウンロードできます。

https://atom.io/

atomを起動して、installメニューから次の2つのplugin packageをインストールします。

* linter-redpen
* language-review

redpenがcliに依存しているらしいので、それもインストールします。

//list[InstallRedpen][InstallRedpen][shell]{
% brew cask install adoptopenjdk
% brew install redpen
//}

=== re:view形式について
htmlやmarkdownのようなmarkup言語です。
詳細につきましては、以下のサイトを参照ください。

https://github.com/kmuto/review/blob/master/doc/format.ja.md

=== 執筆
articles/ 以下に"名前.re"というファイルを用意しました。
ここに書くのが簡単です。
抵抗がある場合は、ファイル名を変更し、articles/catalog.ymlのファイル名も変更してください。
このファイルをatomで開き、編集してbuildします。
ある程度まで書けたら、PullRequestをください。
githubにgreebookfestclubというorganizationを作りましたので、
githubアカウントを教えていただければownerに追加します。
一人でレビューするのは辛いので、みなさんぜひorganizationに参加していただければと思います。

== 表紙の作り方
まだ調べていません。これから調べて書きます。

== CI/CD環境
Circle CIを利用しています。
まず、1文字でも変更して、Pull Requestを作ってください。
すると、自動でCircleCIのBuildが走ります。
下記画面のDetailを押下し、遷移したCircleCIの画面で、"Artifacts"タブを押下します。
すると、GREE-BookFest7.pdfというリンクが表示されるため、
それを押下するとPullRequestの内容を含むPDFを確認できます。
確認して問題なければ、mergeしてください。

//indepimage[circleci]

= RecommendEngineの作り方
outlineだけです。これからちゃんと書きます。

== はじめに
推薦には、次の3つの手法がある。([1])
* Search based method: 同じ著者やキーワードなどを推薦する既存手法
* Cluster Model: ユーザを分類して、それぞれに最適な推薦を行う。
* 協調フィルタ: 類似ユーザが選んだものを推薦する。

== 検証方法
検証方法には、OfflineとOnlineがある。
Offlineはログを使って手元で検証、OnlineはA/Bテストのように実際のユーザで試す方法。
Google/Amazon/Netflixなど多くの企業では、Offlineテストでパラメータを検証する。([1],[2],[4])
チューニング済みのモデル同士をOnlineでA/Bテストする。
Google/Netflixでは長期継続率を確認するため、A/Bテストは数ヶ月行なっている。([3],[4])

== ClusterModel
記事をVectorにして、そこからUserをVectorにして、UserVectorを分類している。

== 記事Modeling
数学的に推薦するには、記事をVectorで表現する必要がある。
記事を単語(形態素)に分割し、単語ごとのVectorを辞書(Word2VecのModel)を引いて調べて合算する。
記事を単語に分割することを形態素解析、単語をVectorに表現することをWordEmbeddingと呼ぶ。

=== 提案:LIMIA辞書作成
形態素解析用の辞書とWordEmbedding用の辞書をLIMIAに最適化することで、記事が正しく分類されるようにする。

=== Offline検証方法
LIMIAでは運営側でMainKeywordが設定されており、記事の特徴を最もよく表している。
ランダムにいくつかの記事をピックアップし、近傍のN記事のMainKeyword一致率を評価指標とする。

=== UserModeling
現状、読んだ記事の平均を取っているんだけど、これで良いのか。
改善案が見つかっていないが、もし見つけたら、ここも改善していった方が良さそう。

=== 分類
k-means++で行なっている。
ここは変えなくても良い気がするが、やろうと思えば別の分類手法を試すことは可能。

=== 推薦

== 協調フィルタ
User-to-Itemの協調フィルタを作る。

=== 協調フィルタロジック
10年前のGoogleNewsではPLSIという手法を使っていた。([2])
LIMIAではMatrixFactorizationというNetflixの手法を使っている。([5])
他にも手法はありそうなので、調べて良さそうな方法を使う。

=== Offline検証方法
GoogleNewsでは、以下の方法で行なっていた。([2])
過去ログから一部ユーザデータを抽出し、それをテストデータとする。
工夫をしていない協調フィルタのロジックで推薦リストを作り、それとの差分を評価値とする。
これが使えそう。

== 参考資料

* [1] Amazon事例: https://scrapbox.io/recommend-study/Amazon.com_Recommendation_Item-to-Item_Collaborative_Filtering
* [2] Google事例1: https://scrapbox.io/recommend-study/Google_News_Personalization:_Scalable_Online_Collaborative_Filter
* [3] Google事例2: https://scrapbox.io/recommend-study/Personalized_News_Recommendation_Based_on_Click_Behavior
* [4] Netflix事例: https://scrapbox.io/recommend-study/The_Netflix_Recommender_System:_Algorithms,_Business_Value,_and_Innovation
* [5] MatrixFactorization: https://datajobs.com/data-science-repo/Recommender-Systems-[Netflix].pdf
* [6] Word2Vec: https://papers.nips.cc/paper/5021-distributed-representations-of-words-and-phrases-and-their-compositionality.pdf
