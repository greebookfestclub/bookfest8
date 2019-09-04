= k-means++のgolang実装

私は業務でメディアアプリの開発を行なっています。
メディアアプリは、より多くのコンテンツを楽しんでもらうことが重要です。
ユーザの興味は、それぞれ異なります。
ユーザ毎に最適なコンテンツを配信することで、より多くのコンテンツを楽しんでいただけると考えました。
そこで、記事の一覧表示やプッシュ通知などでユーザに最適なコンテンツを推薦し、それを配信しています。

//footnote[fn01][ユーザセグメント手法は、全員に同じコンテンツを配信する事に比べて良くなります。しかし、セグメントで複数のユーザまとめてしまうため、一人一人異なるコンテンツを推薦できる手法(@<bib>{AmazonCF})に比べて精度は劣ります。精度は劣る代わりにモデルさえあれば初回起動ユーザにも推薦を提供できます。]
//footnote[fn02][その他の分類手法として、XGBoost@<bib>{xgboost}などがあります。k-means++は他の手法に比べて計算量が同等以下で実装が簡単です。しかし、分類精度は劣ります。]
//footnote[fn03][情報フィルタリングと呼ばれることもあります。(@<bib>{GoogleIF})]

推薦手法の一つとして、モデルベース手法@<fn>{fn03}があります。
モデルベース手法とは、記事ベクトルとユーザベクトルを使って推薦を行う手法です。
メディアアプリを例とすると、記事とユーザのモデルを作り、それらをベクトルで表現します。
モデルベース手法の中で最も簡単な手法として、ユーザセグメント手法@<fn>{fn01}(@<bib>{RecommenderSystem})があります。
ユーザをクラスタに分類し、対象クラスタに最適なコンテンツを配信する手法です。
ここでは、基本的な分類手法であるk-means++@<fn>{fn02}の実装について解説します。

== k-means algorithm

@<kw>{k-means}は最も基本的な分類手法で、少ない計算量で分類することができます。
アルゴリズムは、次のようになっています。

 1. クラスタ中心点をランダムに生成する。
 2. 全てのデータについて、最近傍中心点(所属するクラスタ)を決める。
 3. クラスタに所属する全てのデータの平均を取り、それを新たな中心点として設定する。
 4. (2)(3)を変化が閾値以下になるまで繰り返す

== k-means++ algorithm

k-meansはランダムに取得した初期値が近くにあると、上手く分類できないという課題があります。
そこで、初期値を散らすために@<kw>{k-means++}(@<bib>{k-means})という手法が考案され、広く使われています。
k-meansの(1)の初期値選択を次のように変更します。

//footnote[fn04][単純に遠いものを選んでしまうと異常値に引っ張られてしまう事で局所解に陥ったり、収束が遅くなったりします。確率的に選ばれやすくする事がポイントです。]

 11. 1つ目の中心点をランダムに選択する。
 12. 次の中心点を選択するとき、他の中心点から遠いものが選ばれやすく(@<fn>{fn04})する。
 13. (12)を繰り返して、必要な中心点を作る。

k-meansとk-means++の違いは初期値の決め方のみで、(2)から(4)の分類処理は同じです。

== 中心点決定処理

seedsは初期値、dataは分類対象データとすると、k-means分類処理は次のように書く事が出来ます。

//listnum[k-means][k-means][go]{
func center(seeds [][]float64, data []*Entity) ([][]float64, error) {
	centers := make([][]float64, len(seeds))
	copy(centers, seeds)
  userMap, userCluster := initialize(data)
	for i := 0; i < 100; i++ {
    // 新たな中心点を計算
    centers = getClusterCenters(userCluster)
		// 新たな中心を使って所属するclusterを計算
    delta = getUserCluster(&userCluster)
		logger.Debug("i=", i, ", delta=", delta)
		// 収束したら処理を終了
		if delta == 0 {
			break
		}
	}
	return centers, nil
}
//}

4行目で初期化を行い、5行目で本体処理ループに入ります。
経験上20回程度で収束するため、保険として100回で打ち切るようにしています。
7行目で中心点を更新し、9行目で再近傍クラスタを再計算しています。
deltaは再近傍クラスタが変更されたデータの数を表しており、変更がなくなった場合12行目で処理を終えています。
初期化処理は、次のようになります。

//listnum[initialize][initialize][go]{
func initialize(data []*Entity) (map[uint64]*Entity, map[uint64]uint64) {
  userMap := make(map[uint64]*entityUserVector.Entity, len(data))
  userCluster := make(map[uint64]uint64, len(data))
	for _, v := range data {
		clusterId, _ := nearest(centers, v.Vector)
		userCluster[v.UserId] = clusterId
		userMap[v.UserId] = v
	}
  return userMap, userCluster
}
//}

後続処理で利用するユーザと情報の関係を作成しています。
クラスタ中心点の計算は、次のようになります。

//listnum[getClusterCenters][getClusterCenters][go]{
func getClusterCenters(userCluster map[uint64]uint64) [][]float64 {
  vectorLen := 200
  centers = make([][]float64, len(seeds))
  cnt := make([]uint64, len(userCluster))
  // clusterに所属するvectorの平均を求める
  for userId, clusterId := range userCluster {
    if len(centers[clusterId]) == 0 {
      centers[clusterId] = make([]float64, vectorLen)
    }
    vec := userMap[userId].Vector
    for k, v := range vec {
      centers[clusterId][k] += v
    }
    cnt[clusterId]++
  }
  for clusterId, vector := range centers {
    for k, v := range vector {
      centers[clusterId][k] = v / float64(cnt[clusterId])
    }
  }
  return centers
}
//}

各中心点が再近傍であるデータ点の相加平均を取り、それを新たな中心点とします。
12行目で足し上げ、18行目では個数で割り算しています。
最近傍点再計算処理は、次のようになります。

//listnum[getClusterCenters][getClusterCenters][go]{
func getUserCluster(userCluster *map[uint64]uint64) uint64 {
  var delta uint64
  for _, v := range data {
    clusterId, _ := nearest(centers, v.Vector)
    if clusterId != userCluster[v.UserId] {
      userCluster[v.UserId] = clusterId
      delta++
    }
  }
  return delta
}
//}

全てのデータに対し、最近傍点を再計算しています。
最近傍点が変化したデータ数をdeltaとしています。
deltaが0なら収束しているため、処理を終了します。

== 初期値生成処理

k-means++の初期値生成実装について解説します。
numClusterは分類したいクラスタ数、dataは分類対象データとすると、初期値生成処理は次のように書く事が出来ます。

//listnum[k-means++][k-means++][go]{
func seeds(numCluster uint64, data []*Entity) ([][]float64, error) {
  seeds := getFirstSeed(data)
  seeds = getOtherSeeds(numCluster, data, seeds)
  return seeds, nil
}
//}

２行目のgetFirstSeed()で、1つ目の初期値をランダムに選択します。
３行目のgetOtherSeeds()で、2つ目以降の初期値を生成します。
初期値をランダムに取得する処理は、次のようになります。

//listnum[getFirstSeed][getFirstSeeds][go]{
func getFirstSeed(data []*Entity) [][]float64 {
  seeds := make([][]float64, 0)
	e := data[rand.Intn(len(data))]
	seeds = append(seeds, e.Vector)
  return seeds
}
//}

単純に0から最大長までの整数乱数を発生させ、その位置にある値を返しています。
2つ目以降の初期値を生成する処理は、次のようになります。

//listnum[getOtherSeeds][getOtherSeeds][go]{
func getOtherSeeds(num uint64, data []*Entity, seeds [][]float64) [][]float64 {
  for i := uint64(1); i < num; i++ {
    distance, threshold := getDistance(data, seeds)
    dataNum := getSeed(distance, threshold)
    seeds = append(seeds, data[dataNum].Vector)
  }
  return seeds, nil
}
//}

初期値は分類したいクラスタ数と同じだけ必要です。
2行目で、クラスタ数と同じ回数ループしています。
3行目で各データと最も近い初期値との距離を求めています。
4行目でそれを使って、初期値として相応しいデータを抽出しています。
各データと再近傍点の距離は次のように求めています。

//listnum[getDistance][getOtherSeeds][go]{
func getDistance(data []*Entity, seeds [][]float64) ([]float64, float64) {
  var sum float64
  dist := make([]float64, len(data))
  for k, v := range data {
    _, dist[k] = nearest(seeds, v.Vector)
    sum += dist[k]
  }
  threshold := rand.Float64() * sum
  return dist, threshold
}
//}

4行目で全データに対してループし、5行目で再近傍点からの距離を取得しています。
6行目で全ての距離の総和を求めています。
そして、8行目で0から1の間の乱数を発生させて、総和に掛け算しています。
つまり、8行目のthresholdは、0から総和までの間のランダムな値となります。
初期値として相応しいデータの抽出処理を次に示します。

//listnum[getSeed][getSeed][go]{
func getSeed(dist []float64, threshold float64) int {
  var stack float64
  var dataNum int
  for k, v := range dist {
    stack += v
    if threshold < stack {
      dataNum = k
      break
    }
  }
  return dataNum
}
//}

4行目で全データを1つづつ取り出し、5行目で足し上げています。
6行目で足し上げた数が閾値を超えた場合、ちょうど超えたところにいたデータを初期値として相応しいデータとして返しています。
他の初期値と近い場合、少ししか進まないため閾値を超える確率は小さくなります。
他の初期値と遠い場合、大きく進むため閾値を超える確率が大きくなります。

== まとめ
k-means++の実装を解説しました。
利用する機会は多いですが、実装を行った方は少ないかと思います。
次回はword2vec(@<bib>{word2vec})やMatrix Factorization(@<bib>{MatrixFactorization})など有名アルゴリズムの実装解説シリーズを書いていきたいと思います。
