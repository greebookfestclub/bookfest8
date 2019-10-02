技術書典8へ向けた執筆用repositoryです。
TechBoosterさんのTemplateを元に作成しています。
TechBoosterさん、ありがとうございます。

https://github.com/TechBooster/ReVIEW-Template

# 記事の追加方法
articlesの下に"名前.re"というファイルを作り、review形式で記載します。
作ったファイル名をarticles/catalog.ymlに追加します。

* review形式説明: https://github.com/kmuto/review/blob/master/doc/format.ja.md

# build方法
build-in-docker.shを実行すると、articles/GREE-BookFest8.pdfができます。
また、PullRequestを作るとCircleCIでBuildされます。

# review形式が苦手な方へ
review形式が苦手な方や環境構築が難しい方がいましたら、articles/名前.md というファイル名でMarkDownで書いて連絡いただければ、こちらで変換します。
