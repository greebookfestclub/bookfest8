

= TerraformとJenkinsによる外界監視管理の自動化

== はじめに
グリーインフラ部に所属して李と申します。
普段はRDBMSとMonitoring関連の仕事をやっていますが、興味でGOの開発もやっています。
最近の趣味はMicroServicesと分散システムで、JWTの認証サーバを作ったり、gRPCを触ったりしています。
本ドキュメントは、私が担当している外形監視導入の作業をベースで、いくつのインフラ関連の自動化ツールと使い方をご紹介します。

== モニタリングとは
モニタリングとは監視・観察することを意味します。
エンジニアの世界では、モニタリングはサービスの正常性を監視して問題を特定するシステムとなります。
モニタリングの種類は、監視の視点から、メインに２つの種類があります。

=== サービス管理者視点
インフラの運用者はサーバのCPU負荷、メモリーの使用量を監視したり、サービスの開発者はアプリのエラーログを監視して修正を行いたいなどの要件を満たす監視は、
内部監視・もしくはホワイトボックス監視とよく呼ばれています。
よく使われてるツールはPrometheus（メトリクス監視）、fluentd（ログ監視）などがあります。

=== サービス利用者視点
ゲームやアプリのユーザーは、ゲームが遊べるか、アプリが使えるかしか関心持ってないので、ユーザーからみるとサービスの正常性を監視するのは外形監視となります。
次の章から外形監視について説明いたします。

== 外形監視とは
外形監視はユーザーアクセスをシミュレートして、定期的に、外部公開しているURLに対してリクエストを送り、レスポンスタイムとステータスコードをチェックします。
サービスは外形監視に対してブラックボックスのようなこととなります。
外形監視が必要となる理由は、仮に内部監視がすべて問題なくても、外部からアクセスできないケースが存在しています。
たとえば、ネットワークの障害とか、DNS周りの設定間違いとかの原因でユーザーがサービスにアクセスできなくなります。
この場合に外形監視があると、すぐ障害に気付くことができ、問題を特定して、ユーザーへの影響を最低限に抑えることができます。

== 外製の外形監視サービス
グリーではもともと内製の外形監視サービスを使ってましたが、メンテの手間や検知誤差の原因で、外製のサービスに移行しました。
外形監視業界では、代表的なサービスはPingdomがあります。
Pingdomはたくさんの機能を搭載しているプラットホームです。監視以外アラート通知やサイトパフォーマンス分析など豊かな機能があります。
AWSやGCP（stackdriver）などパブリッククラウドにも外形監視のサービスがあります。
いろいろな外形監視サービスがあると思いますが、基本的にhttp/https/ping監視のベース機能が入っていますので、
自分の要件を合わせて適切なサービスを選択するのがいいと思います。

=== UptimeRobot
今回特に紹介したいのはUptimeRobotというサービスになります。
UptimeRobotはhttp/ping/port/keyword４つの監視タイプをサポートしています。（https監視もhttpタイプとなります）
UptimeRobotはAPI@<fn>{lifn01}から監視の設定することができるので、いろんなカスタマイズな運用ができます。
//footnote[lifn01][https://uptimerobot.com/api]

さらに、UptimeRobotは無料で50モニタを設定することができるので、検証したい・すぐ使いたい方にはすごく便利なサービスだと思います。
監視の設定に関しては詳しく説明をおりませんが、公式サイト@<fn>{lifn02}で実際に手を動かしてみるといいと思います。。
//footnote[lifn02][https://uptimerobot.com/]

== インフラでの外形監視構築と運用
上記でモニタリングや外形監視についてお話しました。
実際UptimeRobotを利用したサンプルを紹介する前に、まずIaCをご紹介したいと思います、まずIaCをご紹介したいと思います。

=== IaC
Infrastructure as Code.
文字のとおり、インフラの設定をコードとして書くことです。
GUIに通らず、コードを書く・更新でインフラの構築や修正ができるメリットは、
1. インフラ設定の記録はコードの記録として残されるため監査しやすい。
2. インフラ環境の複製が簡単にできます。
3. コードがあれば、CI/CDツールを使って自動化することができる。
となります。

また、APIでインフラプラットホームと通信できることは、IaCツールが使える前提となります。
インフラプラットホームといえば、先にAWSやGCPと思う方が多いかもしれませんが、
上記で紹介したUptimeRobotもある意味ではインフラプラットホームです。

この後、IaCツールを使ってUptimeRobotに監視モニターを設定するサンプルを紹介します。

== Terraform
いろんなIaCツールがありますが、たとえばAWSのCloudFormation、ここではTerraformを使います。
TerraformはHashiCorp社で開発したIaCツールです。
一番の特徴はクロスプラットホームで、公式的にAWS,GCPなど主流なクラウドをサポートしていますが、
APIがあれば、Terraformのproviderを書けば、Terraformを使えることができます。

=== Terraformのベースコンポーネント
 * 設定ファイル
 ** HCLとJSONのフォマットをサポートしています。
 ** HCLはHashiCorp社独自なformat languageとなります@<fn>{lifn03}。

//footnote[lifn03][https://www.terraform.io/docs/configuration-0-11/syntax.html]

 * Provider
 ** 各プラットフォームと実際に通信するパーツです。
 ** APIがあればカスタマイズで開発できます。

 * tf.state
 ** Terraformとインフラをシンクするための状態ファイルです。

=== Terraformのワークフロー
 1. write: コードを書きます
 2. plan: terraform planを実行して変更点を確認します
 3. apply: terraform applyを実行して実際に設定を反映させます

=== UptimeRobot + Terraform

まずTerraformをダウンロードします。（Providerの原因で0.11.xxを使います）
//cmd{
~$ wget https://releases.hashicorp.com/terraform/0.11.14/terraform_0.11.14_darwin_amd64.zip
~$ tar xvf terraform_0.11.14_darwin_amd64.zip
~$ mv terraform /usr/bin/terraform
//}

terraform-provider-uptimerobot@<fn>{lifn04}をダウンロードします。
//footnote[lifn04][https://github.com/louy/terraform-provider-uptimerobot]
次のリンクからバイナリをダウンロードして、~/.terraform.d/pluginsの下に置きます。
//cmd{
~$ ls ~/.terraform.d/plugins
terraform-provider-uptimerobot
//}

環境は整えましたので、実際に設定ファイルを書いてみましょう。
//listnum[main.tf][main.tf][hcl]{
variable "api_key" {}

provider "uptimerobot" {
  api_key = "${var.api_key}"
}

resource "uptimerobot_monitor" "test_monitor" {
  friendly_name = "Test Monitor"
  type          = "http"
  url           = "http://test.com"
  interval      = 300
}
//}
これでhttp://test.comを監視するモニタが設定できます。
api_keyはUptimeRobotのGUIから確認することができます。

terraformコマンドを実行します。
//cmd{
~$ TF_VAR_api_key=<api_key> terraform plan
~$ TF_VAR_api_key=<api_key> terraform apply
//}

うまく実行できたら、uptimerobotのguiでTest Monitorが作られたことを確認できます。

== Jenkinsの設定
Terraformを使ってUptimeRobotの設定をコード化にできました。コードがあれば自動化ができます！
TerraformをベースでさらにCI/CDツールを使ってコードだけ書けば自動で適用してくれる仕組みを作っています。
今回のDemoはJenkinsを利用します。

Jenkinsをbuildしやすいため、DockerでbuildできるようにDockerfileを作成しています。
//listnum[Dockerfile][Dockerfile][hcl]{
FROM ubuntu:18.04

ENV API_KEY xxx
ARG TERRAFORM_VERSION

RUN apt-get update -qq && apt-get -y install unzip wget

RUN set -eux; \
	\
	dpkgArch="$(dpkg --print-architecture)"; \
	case "${dpkgArch##*-}" in \
		amd64) url="https://releases.hashicorp.com/terraform/${TERRAFORM_VERSION}/terraform_${TERRAFORM_VERSION}_linux_386.zip" ;; \
	esac; \
	\
  wget -q ${url} && unzip terraform_${TERRAFORM_VERSION}_linux_386.zip && mv terraform /usr/bin/terraform

WORKDIR /uptimerobot

CMD terraform init && \
	TF_VAR_api_key=$API_KEY terraform validate && \
	TF_VAR_api_key=$API_KEY terraform plan && \
	TF_VAR_api_key=$API_KEY terraform apply -auto-approve -parallelism=1
//}

Dockerfileをbuild/runするためのbuild.sh
//listnum[build.sh][build.sh][hcl]{
set -e
API_KEY="xxxx"
TERRAFORM_VERSION="0.11.14"
docker build -t uptimerobot-terraform --build-arg TERRAFORM_VERSION=$TERRAFORM_VERSION .
docker run -v ${PWD}:/uptimerobot -e API_KEY=$API_KEY uptimerobot-terraform 
//}
詳細は次のリポジトリを参考してください。
https://github.com/leeif/terraform-uptimerobot-jenkins-demo

Githubの設定手順
 * Settings -> Webhooks -> Add Webhook
  ** https://<jenkins url>/github-webhook/
  *** Jenkinsのgithub-webhookを追加します。
  ** 「Just the push event」をチェックします。


Jenkinsの設定手順

前提：JenkinsサーバにDockerが使える

 * 新規ジョブ作成 : フリースタイル・プロジェクトのビルドを選択します
 * ジョブ設定 -> General -> GitHub project : Project urlにgithubリポジトリのurlを入れます
 * ジョブ設定 -> ソースコード管理 -> Git
  ** リポジトリURL：cloneのurlを入れます。
  ** ブランチ指定子：*/master
  ** プライベートRepoの場合、認証情報を設定する必要があります。
 * ジョブ設定 -> ビルド・トリガ
  ** GitHub hook trigger for GITScm polling」をチェックします
 * ジョブ設定 -> ビルド -> シェルの実行
 ** シェルスクリプトに@<kw>{bash build.sh}を入れます

保存すればmaster branchにpushする度に、buildが自動で実行されます。

== まとめ
最初にモニタリングと外形監視の概念をお話して、インフラでよく使ってるツールとインフラの管理手法を簡単に説明しました。
最後に外形監視（UptimeRobot）を運用するDemoを紹介しましたが、いくつの注意点があります。
 1. Terraformのstatefileはローカルに保存していますが、remote@<fn>{lifn05}（aws s3など）に保存することはお勧めです。
 2. UptimeRobotのapi_keyは直接にスクリプトに書いてますが、実際の場合、Jenkinsのsecret textなどで環境変数を渡す方が安全だと思います。
//footnote[lifn05][https://www.terraform.io/docs/backends/types/remote.html]