= LIMIAでのBigQuery活用事例

LIMIAでは、分析や集計のためにBigQueryを多用しています。利用方法は、次のような形です。

 1. 全てのデータをBigQueryに流す
 2. 特徴を分析して運営で最適化
 3. 機械に置き換えて最適化を加速

(2)(3)を通じてユーザ体験の向上を目指しています。
しかし、その準備である(1)に工数を取られてしまいます。
そこで、(1)の知見を共有することで、ユーザ体験の向上に専念できる事を目指します。

LIMIAでは保持してる多くのデータをBigQueryに格納しています。
具体的には、イベントログ、アクセスログ、マスターデータ、関連システムデータです。
それぞれについて、BigQueryへの転送方法を以下に示します。

== イベントログ

iOS, Androidアプリのイベントログは、Firebase Analyticsを使って計測しています。
Firebase Analyticsのイベントは、管理画面でボタンを押すだけでBigQueryにデータが連携されます。

Webのイベントログは、Google Analyticsを使って計測しています。
Google Analytics APIで集計結果を取得する事が可能です。
そこで必要な以下のパターンを作り、BigQueryに格納しました。

 * コンテンツ別閲覧数と滞在時間
 * コンテンツ別経路別流入数
 * 検索語別検索流入数

「コンテンツ別閲覧数と滞在時間」を取得するGoogle Analytics APIのパラメータは、以下のようになります。

 * metrics: ga:pageviews,ga:avgTimeOnPage
 * dimensions: ga:pagePath
 * samplingLevel: HIGHER_PRECISION

metricsに取得したい項目を設定します。
dimensionsに集計軸を設定します。
コンテンツ別での集計結果を取得したいので、コンテンツ毎に異なっているURLを設定しました。
samplingLevelにHIGHER_PRECISIONを設定することで、より高いサンプリングレートでの集計結果を取得できます。
試したところHIGHER_PRECISIONを設定しない場合は1%サンプリング、設定した場合20%サンプリングとなりました。

毎晩前日分の集計データを上記パラメータを使ってGoogle Analytics APIから取得し、
BigQueryに日別のテーブル(ex. pv_by_contents_20190831)に格納しました。

== アクセスログ

システムはAWSにあるため、ALBとCloudFrontのログがS3に格納されています。
それをEmbulkを使ってBigQueryへ転送しています。
Embulkコンテナを作り、ECS Fargateで回しています。
まず、Dockerfileを示します。

//listnum[Dockerfile][Dockerfile][Dockerfile]{
FROM openjdk:8-jre-alpine

RUN apk add --no-cache libc6-compat python py2-pip coreutils tzdata && \
  cp /usr/share/zoneinfo/Asia/Tokyo /etc/localtime && \
  echo "Asia/Tokyo" > /etc/timezone

RUN pip install awscli

RUN wget -q https://dl.embulk.org/embulk-latest.jar -O /usr/local/bin/embulk \
   && chmod +x /usr/local/bin/embulk

RUN /usr/local/bin/embulk gem install embulk-input-mysql && \
  /usr/local/bin/embulk gem install embulk-input-s3 && \
  /usr/local/bin/embulk gem install embulk-input-dynamodb && \
  /usr/local/bin/embulk gem install embulk-output-bigquery && \
  /usr/local/bin/embulk gem install embulk-parser-none

COPY files/etc/embulk /etc/embulk
COPY files/entrypoint.sh /entrypoint.sh

ENTRYPOINT ["/entrypoint.sh"]
//}

次にDocker起動時のentryポイントとして設定したshell scriptを示します。
引数で指定したテーブルを転送します。
引数なしで起動すると、全テーブルを転送します。

//listnum[entrypoint.sh][entrypoint.sh][sh]{
  #!/bin/sh

  decrypt() {
      aws kms decrypt --ciphertext-blob \
      fileb:///etc/embulk/blob/bigquery-service-account.blob \
      --output text --query Plaintext \
      | base64 -d > /etc/bigquery_service_account.json
  }

  process_tables() {
      ls /etc/embulk/tables/*.yml.liquid \
      | xargs -n1 java -jar /usr/local/bin/embulk run 2>&1
  }

  process_specified() {
      if [ $# -eq 0 ]; then
          exit
      fi

      for var in $@; do
          if [ -f /etc/embulk/tables/${var}.yml.liquid ]; then
              echo "/etc/embulk/tables/${var}.yml.liquid"
              ls /etc/embulk/tables/${var}.yml.liquid \
              | xargs -n1 java -jar /usr/local/bin/embulk run 2>&1
          fi
          if [ -e /etc/embulk/scripts/${var}.sh ]; then
              echo "/etc/embulk/scripts/${var}.sh"
              /etc/embulk/scripts/${var}.sh
          fi
      done
  }

  init() {
      decrypt

      if [ $# -eq 0 ]; then
          process_tables
      else
          process_specified "$@"
      fi
  }

  init "$@"
//}

Dockerfileで足元に暗号化済みのService Accountファイルをコピーしておき、
3行目からのdecrypt()で平文に戻しています。
45行目で引数の判定を行い、引数が空なら全テーブルを転送しています。
引数が指定されていた場合、引数と同じ名前の設定ファイルもしくはscriptを実行します。

次にentrypointから呼ばれるembulkのshell scriptを示します。
まずは、CloudFrontアクセスログについてです。
CloudFrontのログは、/cf-logs/直下に追加されていきます。
毎晩前日分のログ転送を行うため、ここからファイルの作成時刻が前日のものを手元に転送します。
取得したログファイルをembulkでBigQueryへ転送し、最後に取得した一時ファイルを消去します。

//listnum[access-log-cloudfront-p-limia-jp.sh][access-log-cloudfront-p-limia-jp.sh][sh]{
#!/bin/sh
aws s3 sync s3://log-bucket/cf-logs/ /tmp/access-log-cloudfront-p-limia-jp/ \
--exclude "*" --include "*.`date --date '1 day ago' +%Y-%m-%d`-*" --quiet
java -jar /usr/local/bin/embulk run \
/etc/embulk/logs/access-log-cloudfront-p-limia-jp.yml.liquid 2>&1
rm -rf /tmp/access-log-cloudfront-p-limia-jp/
//}

スクリプトから呼ばれるembulk設定ファイルを以下に示します。
既にログファイルは手元に転送されているため、fileモジュールを使って読み取ります。
前日分のデータを読み込んでいるため、出力先のBigQueryのテーブル名も前日の日付としたいです。
そこで、53行目で前日の日付を計算して設定しています。

//listnum[access-log-cloudfront-p-limia-jp.yml.liquid][access-log-cloudfront-p-limia-jp.yml.liquid][yml]{
  in:
    type: file
    path_prefix: /tmp/access-log-cloudfront-p-limia-jp
    parser:
      charset: UTF-8
      newline: CRLF
      type: csv
      delimiter: "\t"
      trim_if_not_quoted: false
      skip_header_lines: 2
      allow_extra_columns: true
      allow_optional_columns: true
      null_string: "-"
      columns:
      - {name: date, type: string}
      - {name: time, type: string}
      - {name: x_edge_location, type: string}
      - {name: sc_bytes, type: string}
      - {name: c_ip, type: string}
      - {name: cs_method, type: string}
      - {name: cs_host, type: string}
      - {name: cs_uri_stem, type: string}
      - {name: sc_status, type: string}
      - {name: cs_referer, type: string}
      - {name: cs_user_agent, type: string}
      - {name: cs_uri_query, type: string}
      - {name: cs_cookie, type: string}
      - {name: x_edge_result_type, type: string}
      - {name: x_edge_request_id, type: string}
      - {name: x_host_header, type: string}
      - {name: cs_protocol, type: string}
      - {name: cs_bytes, type: string}
      - {name: time_taken, type: string}
      - {name: x_forwarded_for, type: string}
      - {name: ssl_protocol, type: string}
      - {name: ssl_cipher, type: string}
      - {name: x_edge_response_result_type, type: string}
      - {name: cs_protocol_version, type: string}
      - {name: fle_status, type: string}
      - {name: fle_encrypted_fields, type: string}
    decoders:
    - {type: gzip}
  out:
    type: bigquery
    mode: replace
    prevent_duplicate_insert: false
    auth_method: json_key
    json_keyfile: /etc/bigquery_service_account.json
    project: {{ env.GCP_PROJECT_ID }}
    dataset: alb_logs
    compression: GZIP
    source_format: NEWLINE_DELIMITED_JSON
    table: p_limia_jp_{{ "now" | date: "%s" | minus: 86400 | date: "%Y%m%d" }}
    schema_file: /etc/embulk/schema/cloudfront_access_log.json
    auto_create_table: true
//}

BigQueryのSchema定義を以下に示します。

//listnum[cloudfront_access_log.json][cloudfront_access_log.json][yml]{
  [
    {"name": "date", "type": "string", "mode": "nullable"},
    {"name": "time", "type": "string", "mode": "nullable"},
    {"name": "x_edge_location", "type": "string", "mode": "nullable"},
    {"name": "sc_bytes", "type": "string", "mode": "nullable"},
    {"name": "c_ip", "type": "string", "mode": "nullable"},
    {"name": "cs_method", "type": "string", "mode": "nullable"},
    {"name": "cs_host", "type": "string", "mode": "nullable"},
    {"name": "cs_uri_stem", "type": "string", "mode": "nullable"},
    {"name": "sc_status", "type": "string", "mode": "nullable"},
    {"name": "cs_referer", "type": "string", "mode": "nullable"},
    {"name": "cs_user_agent", "type": "string", "mode": "nullable"},
    {"name": "cs_uri_query", "type": "string", "mode": "nullable"},
    {"name": "cs_cookie", "type": "string", "mode": "nullable"},
    {"name": "x_edge_result_type", "type": "string", "mode": "nullable"},
    {"name": "x_edge_request_id", "type": "string", "mode": "nullable"},
    {"name": "x_host_header", "type": "string", "mode": "nullable"},
    {"name": "cs_protocol", "type": "string", "mode": "nullable"},
    {"name": "cs_bytes", "type": "string", "mode": "nullable"},
    {"name": "time_taken", "type": "string", "mode": "nullable"},
    {"name": "x_forwarded_for", "type": "string", "mode": "nullable"},
    {"name": "ssl_protocol", "type": "string", "mode": "nullable"},
    {"name": "ssl_cipher", "type": "string", "mode": "nullable"},
    {"name": "x_edge_response_result_type", "type": "string", "mode": "nullable"},
    {"name": "cs_protocol_version", "type": "string", "mode": "nullable"},
    {"name": "fle_status", "type": "string", "mode": "nullable"},
    {"name": "fle_encrypted_fields", "type": "string", "mode": "nullable"}
]
//}

次はALBのアクセスログです。
年月日のprefixが付いているため、embulk-input-s3を使って転送可能です。
CloudFrontと仕組みを合わせるため、同様にファイルを手元に落としてきてから転送するようにしました。

//listnum[access-log-elb-main.yml.liquid][access-log-elb-main.yml.liquid][yml]{
#!/bin/sh
aws s3 sync s3://bucket/AWSLogs/AccountId/elasticloadbalancing/ap-northeast-1/\
`date --date '1 day ago' +%Y/%m/%d`/ /tmp/access-log-elb-main/ --quiet
java -jar /usr/local/bin/embulk run \
/etc/embulk/logs/access-log-elb-main.yml.liquid 2>&1
rm -rf /tmp/access-log-elb-main/
//}

embulk設定は、次のようになります。
Schema定義が異なるだけで、それ以外はCloudFrontと同じです。

//listnum[access-log-elb-main.yml.liquid][access-log-elb-main.yml.liquid][yml]{
  in:
  type: file
  path_prefix: /tmp/access-log-elb-main
  parser:
    charset: UTF-8
    newline: CRLF
    type: csv
    delimiter: " "
    trim_if_not_quoted: false
    skip_header_lines: 0
    allow_extra_columns: true
    allow_optional_columns: true
    null_string: "-"
    columns:
    - {name: protocol, type: string}
    - {name: timestamp, type: timestamp, format: '%Y-%m-%dT%H:%M:%S.%NZ'}
    - {name: elb, type: string}
    - {name: client_port, type: string}
    - {name: backend_port, type: string}
    - {name: request_processing_time, type: double}
    - {name: backend_processing_time, type: double}
    - {name: response_processing_time, type: double}
    - {name: elb_status_code, type: long}
    - {name: backend_status_code, type: string}
    - {name: received_bytes, type: long}
    - {name: send_bytes, type: long}
    - {name: request, type: string}
    - {name: user_agent, type: string}
    - {name: ssl_cipher, type: string}
    - {name: ssl_protocol, type: string}
    - {name: target_group_arn, type: string}
    - {name: trace_id, type: string}
    - {name: domain_name, type: string}
    - {name: chosen_cert_arn, type: string}
    - {name: matched_rule_priority, type: long}
  decoders:
  - {type: gzip}
out:
  type: bigquery
  mode: replace
  prevent_duplicate_insert: false
  auth_method: json_key
  json_keyfile: /etc/bigquery_service_account.json
  project: {{ env.GCP_PROJECT_ID }}
  dataset: alb_logs
  compression: GZIP
  source_format: NEWLINE_DELIMITED_JSON
  table: limia_jp_{{ "now" | date: "%s" | minus: 86400 | date: "%Y%m%d" }}
  schema_file: /etc/embulk/schema/alb_access_log.json
  auto_create_table: true
//}

BigQuery Schema定義は、次のようになります。

//listnum[alb_access_log.json][alb_access_log.json][yml]{
  [
    {"name": "protocol", "type": "string", "mode": "nullable"},
    {"name": "timestamp", "type": "timestamp", "mode": "nullable"},
    {"name": "elb", "type": "string", "mode": "nullable"},
    {"name": "client_port", "type": "string", "mode": "nullable"},
    {"name": "backend_port", "type": "string", "mode": "nullable"},
    {"name": "request_processing_time", "type": "float", "mode": "nullable"},
    {"name": "backend_processing_time", "type": "float", "mode": "nullable"},
    {"name": "response_processing_time", "type": "float", "mode": "nullable"},
    {"name": "elb_status_code", "type": "integer", "mode": "nullable"},
    {"name": "backend_status_code", "type": "string", "mode": "nullable"},
    {"name": "received_bytes", "type": "integer", "mode": "nullable"},
    {"name": "send_bytes", "type": "integer", "mode": "nullable"},
    {"name": "request", "type": "string", "mode": "nullable"},
    {"name": "user_agent", "type": "string", "mode": "nullable"},
    {"name": "ssl_cipher", "type": "string", "mode": "nullable"},
    {"name": "ssl_protocol", "type": "string", "mode": "nullable"},
    {"name": "target_group_arn", "type": "string", "mode": "nullable"},
    {"name": "trace_id", "type": "string", "mode": "nullable"},
    {"name": "domain_name", "type": "string", "mode": "nullable"},
    {"name": "chosen_cert_arn", "type": "string", "mode": "nullable"},
    {"name": "matched_rule_priority", "type": "integer", "mode": "nullable"}
]
//}

== マスターデータ

マスターデータとして、RDS/MySQLとDynamoDBのデータをBigQueryへ転送しています。
イベントログとアクセスログは、dailyのテーブルを作りデータを追加して行っていました。
しかし、マスターデータは前日分を消去して、転送時点のsnapshotで上書きしています。
まずは、RDS/MySQLの転送設定を示します。

//listnum[user.yml.liquid][user.yml.liquid][yml]{
  in:
      type: mysql
      user: {{ env.LIMIA_DB_USER }}
      password: {{ env.LIMIA_DB_PASS }}
      host: {{ env.LIMIA_DB_HOST_MAIN }}
      database: fily_user
      table: user
      select: "*"
      options: {useLegacyDatetimeCode: false, serverTimezone: Asia/Tokyo}
      default_timezone: "Asia/Tokyo"
      column_options:
          register_status: { value_type: long, type: long }
      parser:
          type: json
  out:
      type: bigquery
      mode: replace
      auth_method: json_key
      json_keyfile: /etc/bigquery_service_account.json
      path_prefix: /tmp/
      file_ext: .jsonl.gz
      source_format: NEWLINE_DELIMITED_JSON
      project: {{ env.GCP_PROJECT_ID }}
      dataset: fily_user
      auto_create_table: true
      table: user
      schema_file: /etc/embulk/schema/user.json
      formatter: {type: jsonl}
      encoders:
          - {type: gzip}
//}

3,4,5行目で、MySQLのHostname、UserおよびPasswordを環境変数から渡しています。
embulkはtinyint(1)のカラムをbooleanと判定してしまいます。
booleanと判定されたくない場合、12行目のようにcolumn_optionsで型を指定します。
次にBigQueryのSchemaを示します。

//listnum[user.json][user.json][yml]{
  [
      {"name":"id", "type":"integer", "mode": "required"},
      {"name":"alias", "type":"string", "mode": "required"},
      {"name":"nick_name", "type":"string", "mode": "nullable"},
      {"name":"register_status", "type":"integer", "mode": "required"},
      {"name":"deleted", "type":"boolean", "mode": "required"},
      {"name":"created_at", "type":"timestamp", "mode": "required"},
      {"name":"updated_at", "type":"timestamp", "mode": "required"}
  ]
//}

次にDynamoDBの転送設定を示します。
DynamoDBもMySQLと同様に転送できます。
ただし、構造を持ったデータを扱う場合、json文字列として扱います。

//listnum[ranking_searchword.yml.liquid][ranking_searchword.yml.liquid][yml]{
  in:
    type: dynamodb
    auth_method: env
    region: ap-northeast-1
    operation: scan
    table: {{ env.LIMIA_ENV }}.ranking.searchword
    columns:
        - {name: hashkey, type: string}
        - {name: rangekey, type: string}
        - {name: count, type: long}
    default_timezone: "Asia/Tokyo"
    parser:
        type: json
out:
    type: bigquery
    mode: replace
    auth_method: json_key
    json_keyfile: /etc/bigquery_service_account.json
    path_prefix: /tmp/
    file_ext: .jsonl.gz
    source_format: NEWLINE_DELIMITED_JSON
    project: {{ env.GCP_PROJECT_ID }}
    dataset: dynamodb
    auto_create_table: true
    table: ranking_searchword
    schema_file: /etc/embulk/schema/ranking_searchword.json
    formatter: {type: jsonl}
    encoders:
        - {type: gzip}
//}

次にBigQueryのSchemaを示します。

//listnum[ranking_searchword.json][ranking_searchword.json][yml]{
  [
      {"name":"hashkey", "type":"string", "mode": "required"},
      {"name":"rangekey", "type":"string", "mode": "required"},
      {"name":"count", "type":"integer", "mode": "required"}
  ]
//}

== 関連システムデータ

LIMIAではプロモーション効果測定にadjustという外部サービスを利用しています。
プロモーション毎にリンクを発行し、それを入稿することで、プロモーション毎の効果測定が可能となります。
集計結果はadjust管理画面から確認できます.
より詳細に集計したい場合、callback endpointを設定しておくと、
ログが発生する度にendpointへログとして必要なパラメータが付いたリクエストが行われます。
ログ情報はhttpリクエストのquery stringに入っているため、これをBigQueryに格納するようにします。
LIMIAではCloud Functionsにcallback endpointを作りました。
実装は、以下のようになります。

//listnum[package.json][package.json][yml]{
  {
  "name": "adjust",
  "version": "1.0.0",
  "description": "",
  "main": "index.js",
  "dependencies": {
    "@google-cloud/bigquery": "^2.0.1"
  },
  "devDependencies": {},
  "scripts": {
    "test": "echo \"Error: no test specified\" && exit 1"
  },
  "author": "",
  "license": "ISC"
}
//}

//listnum[index.js][index.js][yml]{
  /**
   * Insert adjust param data to BigQuery
   *
   * @param {Object} req Cloud Function request context.
   * @param {Object} res Cloud Function response context.
   */
  exports.adjust = (req, res) => {
      const {BigQuery} = require('@google-cloud/bigquery');

      const projectId = 'xxxxx'; // production project id

      const datasetId = 'grows';

      const tableId = 'adjust';

      const bigQuery = new BigQuery({
          projectId: projectId
      });

      const query = req.query;
      console.log(query);

      // 正常なリクエストの場合、idfa、idfv、gps_adidのうちどれかは値が入っている
      if (!query.idfa && !query.idfv && !query.gps_adid) {
          console.log('invalid parameter');
          res.status(400).send('invalid parameter');
          return;
      }

      const row = {
          idfa: query.idfa ? query.idfa : null,
          idfv: query.idfv ? query.idfv : null,
          gps_adid: query.gps_adid ? query.gps_adid : null,
          is_reattributed: query.is_reattributed ? query.is_reattributed : null,
          deeplink: query.deeplink ? query.deeplink : null,
          network_name: query.network_name ? query.network_name : null,
          campaign_name: query.campaign_name ? query.campaign_name : null,
          adgroup_name: query.adgroup_name ? query.adgroup_name : null,
          creative_name: query.creative_name ? query.creative_name : null,
          activity_kind: query.activity_kind ? query.activity_kind : null,
          created_at_milli: query.created_at_milli ? query.created_at_milli : null,
          cost_type: query.cost_type ? query.cost_type : null,
          cost_amount: query.cost_amount ? query.cost_amount : null,
          cost_currency: query.cost_currency ? query.cost_currency : null,
          reporting_cost: query.reporting_cost ? query.reporting_cost : null,
      };

      bigQuery
          .dataset(datasetId)
          .table(tableId)
          .insert(row)
          .then(() => {
              console.log('A row inserted');
              res.status(200).send('succeed');
          })
          .catch(err => {
              console.error('ERROR:', err);
              res.status(500).send('server error');
          });
  };
//}

一部業務の管理ツールとして、Kintoneを利用しています。
Kintone APIでデータが取得できるため、GASで取得しSpreadSheetに書き出します。
そのSpreadSheetをBigQueryから参照し、BigQueryのNative Tableにコピーしています。

//listnum[kintone.gs][kintone.gs][gs]{
// トリガーで１時間に１回実行などにする。
function myFunction() {

  //初期処理
  prepare();

  //本データの流し込み
  writeContractInfo();
}


//初期処理
function prepare(){
  //お行儀悪いけど、わざとvarを付けずにグローバルに定義

  if (typeof subdomain === "undefined") {
    subdomain = "xxx"; //https://****.cybozu.comの****を記入
  }

  if (typeof apps === "undefined") {
    apps = {
      PHOTO: { appid: xxx, name: "xxxx", token: "xxxx" } // kintoneのアプリ情報
    };
  }

  if (typeof kintone_manager === "undefined") {
    kintone_manager = new KintoneManager.KintoneManager(subdomain, apps);
  }
}

function writeContractInfo(){
  var array_kintone_fields = [ "取得するフィールド一覧を設定" ];
  writeSheetFromRecords("photo","PHOTO",array_kintone_fields);
}


//以後は基本的に変更しなくて良い場所

//引数などからkintoneの情報をspreadsheetに流し込む関数
function writeSheetFromRecords(sheet_name,apps_key,array_kintone_fields){
  prepare();
  var records = getKintoneRecords(apps_key,kintone_manager);
  var sheet = SpreadsheetApp.getActive().getSheetByName(sheet_name);
  var row = 2;
  records.forEach(function(record){
      array_kintone_fields.forEach(function(kintone_field,index){
        sheet.getRange(row,index+1).setValue(record[kintone_field].value);
      })
      row++;
  })
}

//引数からkintoneの情報をarrayにして返す関数
function getKintoneRecords(apps_key_name,kintone_manager){
  var offset = 0;
  var loopendflg = false;
  var records =[];

  while(!loopendflg){
    var query = 'offset ' + offset;
    var response = kintone_manager.search(apps_key_name, query);

    // ステータスコード
    // 成功すれば200になる
    var code = response.getResponseCode();

    // コンテンツの取得
    var content = JSON.parse(response.getContentText());

    // レコードの配列が取得
    Array.prototype.push.apply(records, content.records);

    if(content.records.length === 100){
      offset += 100;
    }else{
      loopendflg = true
    }
  }

  return records;
}
//}

また、Search Consoleのデータを取得し、SEO分析に利用しています。

== まとめ

BigQueryにデータを集約することで、SQLさえ書ければ分析可能となりました。
分析する人が増えたので、ユーザやアイテムへの理解が進み、最適化が可能となりました。
しかし、データやツールが増える度に対応しなければならず、そこに時間がかかっています。
まだまだ改良の余地は大きいと思いますので、アドバイスいただけると助かります。
今後はメンテナンス不要の仕組みを構築することにより、ユーザ体験の向上を推進したいと思います。
