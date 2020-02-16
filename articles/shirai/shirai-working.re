#@# ToDo: ファイルを分割して終わった箇所と書きかけの箇所を分離する
#@# である調→ですます調のほうがいい感じがするので統一
#@# 参考文献が本文中でうまく参照できていない感じだけど、本文が冗長になることを避けたいのでできるだけ引用で済ます方針。
#@# ファイルの分割について https://blog.kakeragames.com/2019/12/02/split-re-view-file.html
= working

=== VCI を使ったVirtual Castの拡張

VCはそのバーチャル空間にインタラクティブ性を持たせるために独自のスクリプトシステムを有しており「VCI」と呼ばれています。
VCIはUnity上で3Dオブジェクトをセットアップし、スクリプトはLua言語を使って書くことができます。
#@# https://virtualcast.jp/wiki/doku.php?id=vci:script:luatutorial

上記のシナリオに現れる「パオズゲーム」や「バーチャル・ウイリアムテル」は、「直感アルゴリズム」番組中ではVCIを使って実装されています。

コード的にはLuaなのでこんなかんじです。

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

なお、VC自体の設計思想は「VR」というよりは
「HMDを使って演じるバーチャルな3Dキャラクターによる放送局のためのシステム」ですので、
VCIは主にグラフィックスについてのインタラクション、具体的にはシーングラフと衝突、ユーザーによるグラブアクションなどが取得できますが、
外部システムやデバイスとの通信はできません。
この先は色々な魔改造を施していくことになります。


== 実装と解説


//image[SA19RTLeq][筆者の機材的な装備（解説を入れる予定）][scale=0.5]{
//}

シナリオと技術的な実装要素を図にするとこんなかんじです。
//image[SA19-scenario][シナリオと技術的な実装要素（ほぼ最終版資料より）]{
//}


#@# https://docs.google.com/presentation/d/1IosX_76FQ8Fi4ewnXgXh5uGrb83492zZMgIc7kjINuc/edit#slide=id.p6
#@# SA19RTL https://docs.google.com/presentation/d/1MnfQwyAcq4JnD1lZKzVyksAS9ieWsNxJvo6msbVW-KQ/edit#slide=id.g78f914e51a_1_0

続いて、個々の技術についての実装紹介になります。

=== 会場音声分析からのギフト送信
まず、会場音声分析からのギフト送信を解説します。
会場の歓声や拍手、笑いなどによって、VCIのアイテムをバーチャルギフトとして降らせることがゴールです。

会場の音響を分析するPCを用意し、python環境で開発した音声分析システムを使い、その分析結果を
WebSocketを扱うことができるNode.jsのライブラリ「Socket.IO」を使ってJSON形式にて送受信させます。
このあたりはラボのインターン、栢之間さん（東工大2年生）が中心になってPythonによる音響分析を実装しています。

//list[MicPC-index.js][index.js（音響分析PC）]{
const { PythonShell } = require('python-shell'); //pythonをnodeから起動する
const socketio = require('socket.io')
app.use(express.static('public'));
const expressServer = app.listen(3000); 
const io = socketio(expressServer); 
var python_script_path = '../../EmotionAnalysis/'; //実行するpython scriptの場所

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

//list[MicPC-index.js][client.js（音響分析PC）]{
// socket接続をし、index.jsから送られてきた解析結果を同socketに存在する別のclientに送信する

//localのindex.jsと接続するためのsocket。何らかのブラウザで下記URLを開くことで接続される。
const LocalSocket = io.connect('http://localhost:3000'); //サーバーと同じポートを指定すること

// Main,OMENそれぞれのindex.jsで起動しているポートないしURLを入力すること
const MainSocket = io.connect('https://main.jp.ngrok.io/');
const OMENSocket = io.connect('https://omen.jp.ngrok.io/');

// index.jsから送られてきた解析結果をそのまま接続されているsocketのclientに送る
LocalSocket.on('send_EmoAna_Result', (obj) => {
    console.log(obj)
    MainSocket.emit('send_EmoAna_Result', obj)
    OMENSocket.emit('send_EmoAna_Result', obj)
})
//}

上記の main.py で呼ばれる音声分析系はこんなかんじです。

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
STFTは short-time Fourier transform 短時間フーリエ変換です。
MFCCはメル周波数ケプストラム係数（Mel-Frequency Cepstrum Coefficients）という
聴覚フィルタに基づく音響分析手法で、人間の聴覚特性にあわせたフィルタバンクを使って高速に音響の特徴を抽出できます。
結果として applause, laugh, excitement つまり、拍手のような喝采、笑いと興奮を抽出します。単純に拍手だけ判定することも可能です。

#@# 冗長なのでとりあえず使用しない方向で
#@# //list[judgeSet.py][judgeSet.py main.pyに呼び出される]{
#@# def realtime(project):

=== VCI と node.js を使ったVirtual Castの拡張

前述のとおり、VCおよびVCIには外部システムとの通信機能はありませんが、唯一、デバッグ情報をWebSocketで受け取ることができます。
oocytanbさんの vci-logcat というプロジェクト（
@<href>{https://github.com/oocytanb/vci-logcat}
）が詳しいのでここでの詳説は割愛します。

VCI→他のシステムへの通信はWebSocket通信を使って実装します。
逆に他のシステム→VCIへのイベントは、キーボード信号を使って実装することにします。
具体的には pyAutokey という Pythonライブラリを使って、VCを使っているPCのキーイベントを発生させて、VC側ではそのキーイベントを拾います。

//list[MainPCindex.js][自動マウス＆キー操作（node.js)]{
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
        // 起動後マウスとキー入力を奪われるので、本番以外はコメントアウト推奨
        // 止めたいときは、Alt + Ctrl + Delete を押せば止まる
        // 1 紙吹雪、2 パオズ 3・4 Hapbeat(vci)の帯の色を戻す
        // ---------------------------------------------------------------------
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

//list[MainPCindex.js][自動マウス＆キー操作（python)]{
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

マッピング後の解析結果が閾値を超えたらマウスを (100,100）に動かし、指定のキー入力を動作させます。
ただしこのスクリプトは起動後にマウスとキー入力を奪われるので、本番以外はコメントアウト推奨です。

=== M5Stackによる触覚の可視化
//image[REMOPRE1117][REMOPRE1117]{
//}


筆者が背負っているPC（HP OMENX）の中で動いている nodeサーバのコード（index.js）はこんなかんじです。
音声分析PCから送られてきた解析結果をマッピングして、VCのデバッグ情報をWebsocketで受け取ります。
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
