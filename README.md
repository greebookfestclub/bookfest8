技術書典8へ向けた執筆用repositoryです。
TechBoosterさんのTemplateを元に作成しています。
TechBoosterさん、ありがとうございます。

https://github.com/TechBooster/ReVIEW-Template

# スケジュール
2020/01/17: タイトル締め切り
2020/01/24: アウトライン締め切り
2020/02/07: 本文及び表紙締め切り
2020/02/14: 広報確認締め切り、印刷所入稿
2020/03/01: 技術書典8当日

# 用語解説
* タイトル締め切り: タイトルを書いたPullRequestを作ってください。例: https://github.com/greebookfestclub/bookfest7/pull/13
* アウトライン締め切り: 各章のタイトルを書いたPullRequestを作ってください。例: https://github.com/greebookfestclub/bookfest7/pull/4
* 本文締め切り: 本文を書いたPullRequestを作ってください。例: https://github.com/greebookfestclub/bookfest7/pull/21

# 作業環境
原稿管理は、GitHubで行います。
上記の例を参考にPullRequest作成をお願いします。
分かりづらいようでしたら、樋口にmentionいただければサポートします。
PullRequestを作るとCircleCIでPDFが自動的に作成されます。
PDFを確認するためには、GitHubのgreebookfestclub organizationに参加する必要があります。
捨てアカウントで構いませんので、GitHub(github.com)のアカウントを教えてください。
* 技術書典8原稿: https://github.com/greebookfestclub/bookfest8

# 記事の追加方法
articlesの下に"名前.re"というファイルを作り、review形式で記載します。
作ったファイル名をarticles/catalog.ymlに追加します。

* review形式説明: https://github.com/kmuto/review/blob/master/doc/format.ja.md

# build方法
build-in-docker.shを実行すると、articles/GREE-BookFest8.pdfができます。
また、PullRequestを作るとCircleCIでBuildされます。

# review形式が苦手な方へ
review形式が苦手な方や環境構築が難しい方がいましたら、articles/名前.md というファイル名でMarkDownで書いて連絡いただければ、こちらで変換します。
