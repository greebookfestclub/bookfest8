
= 環境構築: reviewでbuildできるようにする
== install docker desktop for mac
ここから落としてきてinstall
https://hub.docker.com/

== install review

//list[BuildReview][BuildReview][shell]{
git clone git@github.com:kmuto/review.git
cd review
docker build --tag=review .
//}

なぜかbuildできない。。。
Macをrebootしたら解決。

== 執筆環境構築
download atom
https://atom.io/

atomを起動して、installメニューから次の2つのplugin packageをインストール。

* linter-redpen
* language-review

redpenがcliに依存しているらしいので、それもインストール。

//list[InstallRedpen][InstallRedpen][shell]{
brew cask install adoptopenjdk
brew install redpen
//}

== build環境構築
TechBoosterのtemplateを利用。

//list[GetTemplate][GetTemplate][shell]{
gem install bundler
git clone git@github.com:TechBooster/ReVIEW-Template.git
git submodule init
git submodule update
npm install
npm run pdf
//}

大量のエラー。。。

== dockerを使って
//list[BuildTemplate][BuildTemplate][shell]{
docker pull vvakame/review:3.1
./build-in-docker.sh
//}

すんなり通った。

githubにorgとrepoを作って、準備完了。
https://github.com/greebookfestclub/bookfest7

= Envoy Mobile SDKのbuild

https://envoy-mobile.github.io/docs/envoy-mobile/latest/start/building/building.html

説明に従って、bazelを入れる。
https://docs.bazel.build/versions/master/install-os-x.html#install-with-installer-mac-os-x

//list[InstallBazel][InstallBazel][shell]{
./bazel-0.27.1-installer-darwin-x86_64.sh --user
//}

Envoy Mobileのコードを取得。

git clone https://github.com/lyft/envoy-mobile.git --recursive

そんで、build。

//list[BuildEnvoyMobile][BuildEnvoyMobile][shell]{
bazel build android_dist --config=android
//}

ANDROID_HOMEとANDROID_NDK_HOMEの設定が必要と出た。

AndroidStudioから ndk, lldb, cmake を有効化。
次の環境変数を設定。

//list[SetEnv][SetEnv][shell]{
export ANDROID_HOME=/Users/masahiro.higuchi/Library/Android/sdk
export ANDROID_NDK_HOME=/Users/masahiro.higuchi/Library/Android/sdk/ndk/20.0.5594570
//}

//list[InstallTools1][InstallTools1][shell]{
brew install coreutils wget cmake libtool go bazel automake ninja clang-format autoconf aspell
//}

jdk8が必要と出たので、脳死で入れる。

//list[InstallTools2][InstallTools2][shell]{
brew cask install homebrew/cask-versions/adoptopenjdk8
//}

retryしてok

//list[InstallTools3][InstallTools3][shell]{
brew install coreutils wget cmake libtool go bazel automake ninja clang-format autoconf aspell
//}

これも必要

//list[InstallTools4][InstallTools4][shell]{
go get -u github.com/bazelbuild/buildtools/buildifier
//}

python3が必要とのことなので、

//list[InstallTools5][InstallTools5][shell]{
brew install pyenv
//}

//list[InstallTools6][InstallTools6][shell]{
export PYENV_ROOT=${HOME}/.pyenv
if [ -d "${PYENV_ROOT}" ]; then
    export PATH=${PYENV_ROOT}/bin:$PATH
    eval "$(pyenv init -)"
fi
//}

//list[InstallTools7][InstallTools7][shell]{
pyenv install 3.7.3
pyenv global 3.7.3
//}

build済みのsdk発見。こっちを使おう。
https://github.com/lyft/envoy-mobile/releases

== サーバ構築
通信相手のサーバを構築する。

ec2 a1.2xlarge インスタンスが必要。
a1って何？？？
調べてみると、ARMのインスタンス。
東京には来ていない。
Ohaioで立ててみる。

//list[BuildServer][BuildServer][shell]{
# basic toolchain
sudo apt-get install build-essential openjdk-8-jdk python zip unzip \
  software-properties-common make cmake bc libtool ninja-build automake \
  time apt-transport-https
# clang-8
wget http://releases.llvm.org/8.0.0/clang+llvm-8.0.0-aarch64-linux-gnu.tar.xz
sudo tar -C /usr/local/ -xvf clang+llvm-8.0.0-aarch64-linux-gnu.tar.xz --strip 1
rm -rf clang+llvm-8.0.0-aarch64-linux-gnu.tar.xz
# bazel 0.26.1
wget https://github.com/bazelbuild/bazel/releases/download/0.26.1/bazel-0.26.1-dist.zip
mkdir -p /tmp/bazel_build
unzip -o bazel-0.26.1-dist.zip -d /tmp/bazel_build
rm -rf bazel-0.26.1-dist.zip
cd /tmp/bazel_build
env EXTRA_BAZEL_ARGS="--host_javabase=@local_jdk//:jdk" bash ./compile.sh
sudo cp output/bazel /usr/local/bin/bazel
rm -rf /tmp/bazel_build
# bloaty
git clone https://github.com/google/bloaty
cd bloaty
mkdir build
cd build
cmake ..
make -j6
cp bloaty /usr/local/bin/bloaty
//}

ここで時間切れ。
