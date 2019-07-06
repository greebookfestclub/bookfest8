技術書典7へ向けた執筆用repositoryです。
TechBoosterさんのTemplateを元に作成しています。
TechBoosterさん、ありがとうございます。

https://github.com/TechBooster/ReVIEW-Template

# 記事の追加方法
articlesの下に"名前.re"というファイルを作り、review形式で記載します。
作ったファイル名をarticles/catalog.ymlに追加します。

* review形式説明: https://github.com/kmuto/review/blob/master/doc/format.ja.md


# build方法
build-in-docker.shを実行すると、articles/ReVIEW-Template.pdfができます。
時間ができ次第、CircleCI対応します。
