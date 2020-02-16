#@# ToDo: ファイルを分割して終わった箇所と書きかけの箇所を分離する
#@# である調→ですます調のほうがいい感じがするので統一
#@# 参考文献が本文中でうまく参照できていない感じだけど、本文が冗長になることを避けたいのでできるだけ引用で済ます方針。
#@# ファイルの分割について https://blog.kakeragames.com/2019/12/02/split-re-view-file.html
#@# = working

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



=== M5Stackによる触覚の可視化
//image[REMOPRE1117][REMOPRE1117]{
//}


筆者が背負っているPC（HP OMENX）の中で動いている Node.js サーバのコード（index.js）はこんなかんじです。
MicPCから送られてきた解析結果をマッピングして、VCのデバッグ情報をWebsocketで受け取ります。
それらの値をトリガーとして、M5Stackにシリアル通信でコマンドを送りLEDを光らせます。

なお、すべてクラウド環境で動作したい設計なので、ngrok（エングロック）を使ってトンネル化し、
ローカル環境上で実行しているアプリケーションの通信をインターネット経由で動作するようにしています。
これは会場のネットワーク、特にWifiが数千人の国際イベントではまともに動作しないことを想定し、LTE等の公衆回線を使うことを想定した設計です。

//list[OMEN-index.js1][ngrok起動(node module使用)]{
const express = require('express');
var app = express();
const PORT = process.env.PORT || 4000
var server = app.listen(PORT, () => {
    console.log('listening to requests on port ' + PORT)
});

const ngrok = require('ngrok')

connectNgrok().then(url => {
    console.log('URL : ' + url);
});
async function connectNgrok() {
    let url = await ngrok.connect({
        addr: 4000, // port or network address, defaults to 80
        subdomain: 'omen', // reserved tunnel name https://alex.ngrok.io
        authtoken: '***', // your authtoken from ngrok.com
        region: 'jp', // one of ngrok regions (us, eu, au, ap), defaults to us
    });
    return url;
}
//}


M5Stackは中々優秀なのですが、通信を文字で行うと流石に重たく、24個のフルカラーLEDが美しく光ってくれないので、intをうまく使って3種類の点灯モードと強度を詰め込んでいきます。

//list[OMEN-index.js2][VCからのメッセージを送ってマイコンでLED光らせる index.js]{
// Socket setup
var L_Top2Bot = 200 //数字の説明：xyz, y = 左右（左＝0、右＝1）
R_Top2Bot = 210
L_Bot2Top = 201
R_Bot2Top = 211
L_Center2Edge = 202
R_Center2Edge = 212
L_Edge2Center = 203
R_Edge2Center = 213
L_MeterLED = 204
R_MeterLED = 214
Random_Rainbow = 205
HandShaked = 206
hit2body = 207
Start_HB = 250
Stop_HB = 251

var LED_Mode = 1; //LEDが光るモード。0にすると拍手・笑いをdisable

let pySendSerial = new PythonShell('sendserial.py',
 { mode: 'text', pythonOptions: ['-u'], scriptPath: './' });
pySendSerial.on('message', data => {
    console.log(data)
})

// VCからのメッセージ(print())を受け取りM5Stackにシリアル通信を行う
//(VC非起動時はエラーになるのでコメントアウトする)
// -----------------------------------------------------------
const vci_logcat = require('./vci-logcat/bin/vci-logcat')
var vci = vci_logcat.vciEmitter
vci.on('print', (arg) => {
    console.log(arg);
    if (arg == 'hit2apple') {
        pySendSerial.send(String(Random_Rainbow))
    }
    if (arg == 'hit2body') {
        pySendSerial.send(String(hit2body))
    }
    if (arg == 'HugOn') {
        pySendSerial.send(String(Start_HB)) //鼓動表現。一定の周期で光らせる
    }
    if (arg == 'HugExit') {
        pySendSerial.send(String(Stop_HB)) //
    }
    if (arg == 'enableLEDmeter') { // 笑い、拍手にLEDを反応させる
        LED_Mode = 1
        console.log("enabled")
    }
    if (arg == 'disableLEDmeter') { // 笑い、拍手にLEDが反応させない
        LED_Mode = 0
        console.log("disabled")
    }
    if (arg == 'Handshaked') {
        pySendSerial.send(String(HandShaked))
    }
})

// 値の範囲を変換する関数。例：(5,0,10,0,100) => return 50
const map = (value, fromMin, fromMax, toMin, toMax) => {
    let result = 0;
    result = (value <= fromMin)
        ? toMin : (value >= fromMax)
            ? toMax : (() => {
                let ratio = (toMax - toMin) / (fromMax - fromMin);
                return (value - fromMin) * ratio + toMin;
            })();
    return result;
};

// 値の範囲をmin~maxの間で制限する関数。例：(5,0,4) => return 4
function value_limit(val, min, max) {
    return val < min ? min : (val > max ? max : val);
}

// 音響解析結果の値をLEDの数に再マップし,intに直してM5Stackに送信する
io.on('connection', (socket) => {
    socket.on('send_EmoAna_Result', (obj) => {
        // mapするときの最大値を決定。最大値との比率で値を決める（設定した最大値以上で１、それ以外は0~1）
        var Applause_fromMax = 0.13
        var Laugh_fromMax = 0.02
        obj.L_L = map(value_limit((obj.L_L - 0.001), 0, Laugh_fromMax), 0, Laugh_fromMax, 0, 1)
        obj.L_A = map(value_limit((obj.L_A - 0.04), 0, Applause_fromMax), 0, Applause_fromMax, 0, 1)
        console.log(obj);
        io.emit('tc2client', obj)
        //EmoAnaから入ってきた0~1の解析結果をLEDの数（0~28）に再マップ
        L_L_barheight = Math.round(map(obj.L_L, 0, 1, 0, 28))
        L_A_barheight = Math.round(map(obj.L_A, 0, 1, 0, 28)) + 30

        pySendSerial.send(String(L_L_barheight))
        pySendSerial.send(String(L_A_barheight))

        pySendSerial.send(String(L_MeterLED))
        pySendSerial.send(String(R_MeterLED))
    });
})
//}

会場で笑いや拍手が起きると、両脇のカラーLEDが3種類のパターンでカラフルに明滅します。
実際、演者はHMDを装着しており、会場のリアクションに反応することは難しいのですが、これによって触覚とLEDが双方向性を与えてくれます。
なお、ステージや会場の遠くからの視認性を確認するために、ハロウィン前夜の六本木ヒルズで実験したりしました。



== 当日に起きたこと ＆ 今後に向けて

その後、国際リモートライブのためのリハーサル、特に日豪同時リハーサルやReal-Time Live!特有の公式練習、ひたすらダンスの練習（オリジナル振付）、機材がクソ重い、音声品質と遠のくゴール、不測のトラブル、すごすぎる他の発表者…などなどいろいろありましたが、そろそろ長すぎるような気もするのでこのへんで筆を置きたいと思います。

#@# Matt AI - Speech Driven Digital Human with Emotions http://sa2019.conference-program.com/presentation/?id=real_106&sess=sess230 Jingxiang Li, et al. Tencent Technology Company Limited

結局のところ、シナリオのおもしろさをレビューするのも大事だし、設計のレビューも、音響周りのレビューも、技術のライブデモはほとんど技術なのでした。　

次回のSIGGRAPHはワシントンです。
でもオリンピックも直撃の日程なので、なかなか無理はできなさそう。

2021年のSAは東京なので、ここに向けてどんどんパワーアップして、世界中を驚きと笑いでひっくり返したいですね！

== あとがきにかえて
 SA19RTL ふりかえり ミルキークイーンさん/尾車Roseいのりさん
いつも元気で知的で英語シナリオの監修までやっていただいたミルキーさん。
本当にありがとうございました。

我々がなぜバーチャルビーイング（virtual being）なのか?という本質的な問 いについての深いシナリオまでできていたのですが、 残念ながら本番は声が元気すぎてマイクが死んでしまったものと推測しま す。今後、再挑戦する機会を是非とも作りたい! ぜひご一緒できましたら幸いです。
リンリンさん/法元明菜さん。
いつもよく気がつく努力家なリンリン/法元明菜さん。作って頂いた振り付け がとてもよく、徹夜で練習しまくりました。本番では音が飛んでいたのに踊 り続けられたのはこの特訓のおかげ!そして自己紹介パートのステキなイラ ストありがとうございました!
シーさん/岩井映美里さん いろんなご都合で本番直前のみの絡みとなり「いつもマイペースで何を考え ているか分からないところがあるよ~」という情報だけは頂いていたので実 はちょっぴり不安があったのですが、そんなことは杞憂でした。本当に声もキレイだし、芯がしっかりした人物なのだと感じました。特に最後のダンスシ ーン!会場のミュージックがトラブルで切れており、しかもバーチャル内で はそのトラブルがわからない状態だったのに、最後まで歌い続けて踊りもし っかり...会場は何が起きているのか、これがライブなのか!とわかり手拍子 から拍手の渦...今思い出しても泣きそうになります。
スタッフの皆様/おわりに
チェアの皆さんや他のReal-Time Live!発表者にもインタビューしてみたので すが、やっぱり会場をもっとも沸かせたのはウチの発表!新開発の会場熱気分析 システムもみごとにメーター振り切っていました。会場投票が無かったのは

 残念!トロフィー持って帰りたかった!くやしい! でも音声トラブルがあってシナリオがしっかり聞いてもらえなかったのも事実だし、審査員賞を取ったパフォーマンスもホント凄かった!これは是非と も学びたいやつです。 というわけで次はの挑戦は夏のSIGGRAPH（ワシントンD.C. 8/19-23）で会場 投票一位を目指して頑張るか、中国ライブかな...同じことは二度とやらない のがラボのポリシーでもあるので、技術的チャレンジも盛り込んでがんばっていきたいと思います!

執筆に戻るべく、
目の前の業務を片付けるべく、
集中しているところに
別の飛び込み業務が入ってきて、
それに付き合った挙げ句執筆は進まず、
片付けた業務も無駄になり、
いま3時間と集中力を失いました

書いたものが自分の書籍になる技術書展は尊いです
広報チェックとかのプロセスはありますが
自分の意志とは関係ないところで
生み出したものが曲げられたり変えられたり
日の目を見なかったりすることに比べれば
共有知にする努力ははるかに尊い
