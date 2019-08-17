
= この本の作り方

== Build環境

印刷所へはPDFで納品します。
まずは、ここにあるテキストを手元のPCでBuildして、PDF形式で出力します。

=== install docker desktop for mac

build用のdocker環境が用意されています。
これを使ってbuildするのが簡単です。
もし、"docker desktop for mac"がなければ、インストールします。
"docker desktop for mac"は、以下のサイトからダウンロードできます。

@<href>{https://hub.docker.com/}

=== build pdf using docker

この書籍のデータは、githubのopen repositoryで管理しています。
まず、このデータを取得します。
build用のdocker imageを取得し、それを使ってPDFを作成します。
すると、articles/GREE-BookFest7.pdf というファイル名でbuildしたPDFファイルが出力されます。

//cmd{
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

@<href>{https://atom.io/}

atomを起動して、installメニューから次の2つのplugin packageをインストールします。

 * linter-redpen
 * language-review

redpenがcliに依存しているらしいので、それもインストールします。

//cmd{
% brew cask install adoptopenjdk
% brew install redpen
//}

=== re:view形式について

htmlやmarkdownのようなmarkup言語です。
詳細につきましては、以下のサイトを参照ください。

 * 公式ドキュメント: @<href>{https://github.com/kmuto/review/blob/master/doc/format.ja.md}
 * チートシート: @<href>{https://gist.github.com/erukiti/c4e3189dda179a0f0b73299fb5787838}

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
