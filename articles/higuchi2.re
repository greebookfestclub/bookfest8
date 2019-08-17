= k-means++のgolang実装

== はじめに

業務でメディアアプリの開発を行なっています。
メディアアプリは、より多くのコンテンツを楽しんでもらうことが重要です。
ユーザの興味は、それぞれ異なります。
ユーザ毎に最適なコンテンツを配信することで、より多くのコンテンツを楽しんでいただけると考えました。
そこで、記事の一覧表示やプッシュ通知などでユーザに最適なコンテンツを推薦し、それを配信しています。

//footnote[fn01][ユーザセグメント手法は、全員に同じコンテンツを配信する事に比べて良くなります。しかし、セグメントで複数のユーザまとめてしまうため、一人一人異なるコンテンツを推薦できる手法(@<bib>{AmazonCF})に比べて精度は劣ります。精度は劣る代わりにモデルさえあれば初回起動ユーザにも推薦を提供できます。]
//footnote[fn02][その他の分類手法として、XGBoostやEM Clusteringがあります。k-means++は他の手法に比べて計算量が同等以下で実装が簡単です。しかし、分類精度は劣ります。]
//footnote[fn03][情報フィルタリングと呼ばれることもあります。(@<bib>{GoogleIF})]

推薦手法の一つとして、モデルベース手法@<fn>{fn03}があります。
メディアアプリを例とすると、記事とユーザのモデルを作り、それらをベクトルで表現(@<bib>{word2vec})します。
モデルベース手法とは、記事ベクトルとユーザベクトルを使って推薦を行う手法です。
モデルベース手法の中で最も簡単な手法として、ユーザセグメント手法@<fn>{fn01}(@<bib>{RecommenderSystem})があります。
ユーザをクラスタに分類し、対象クラスタに最適なコンテンツを配信する手法です。
ここでは、基本的な分類手法であるk-means++@<fn>{fn02}(@<bib>{k-means})の実装について解説します。

== k-means algorithm

 1. クラスタ中心点をランダムに選択する。
 2. 全てのデータについて、最近傍中心点(所属するクラスタ)を決める。
 3. クラスタに所属する全てのデータを中心点として再設定する。
 4. (2)(3)を変化がなくなるまで繰り返す

== k-means++ algorithm

k-meansの(1)の初期値選択を次のように変更する。

 11. 1つ目の中心点をランダムに選択する。
 12. 次の中心点を選択するとき、他の中心点から遠いものが選ばれ易くする。
 13. (12)を繰り返して、必要な中心点を作る。

k-meansとk-means++の違いは初期値の決め方のみで、(2)から(4)の分類処理は同じです。

== k-means実装

//listnum[k-means][k-means][go]{
// kmeans処理本体。クラスタ中心点を返す。
// 入力のseedsは初期値、dataは分類対象データ。
func center(seeds [][]float64, data []*entityUserVector.Entity) ([][]float64, error) {
	centers := make([][]float64, len(seeds))
	copy(centers, seeds)
	userMap := make(map[uint64]*entityUserVector.Entity, len(data))
	ideaCluster := make(map[uint64]uint64, len(data))
	// 異なる長さのvectorが存在するとruntime errorとなるため、決め打ちする。
	//vectorLen := len(data[0].Vector)
	vectorLen := 200
	for _, v := range data {
		//logger.Debug("k=", k, ", userId=", v.UserId, ", vector=", v.Vector)
		clusterId, _ := nearest(centers, v.Vector)
		ideaCluster[v.UserId] = clusterId
		userMap[v.UserId] = v
	}
	for i := 0; i < 100; i++ {
		delta := 0
		centers = make([][]float64, len(seeds))
		cnt := make([]uint64, len(ideaCluster))
		// clusterに所属するvectorの平均を求める
		for ideaId, clusterId := range ideaCluster {
			if len(centers[clusterId]) == 0 {
				centers[clusterId] = make([]float64, vectorLen)
			}
			vec := userMap[ideaId].Vector
			for k, v := range vec {
				centers[clusterId][k] += v
			}
			cnt[clusterId]++
		}
		// clusterの中心を平均で更新する
		for clusterId, vector := range centers {
			for k, v := range vector {
				centers[clusterId][k] = v / float64(cnt[clusterId])
			}
		}
		// 新たな中心を使って所属するclusterを計算
		for _, v := range data {
			clusterId, _ := nearest(centers, v.Vector)
			if clusterId != ideaCluster[v.UserId] {
				ideaCluster[v.UserId] = clusterId
				delta++
			}
		}
		logger.Debug("i=", i, ", delta=", delta)
		// 収束したら処理を終了
		if delta == 0 {
			break
		}
	}
	return centers, nil
}
//}

== k-means++実装

//listnum[k-means++][k-means++][go]{
	// kmeans++の初期値を返す。入力のnumClusterはクラスタ数、dataは分類対象データ。
	// 初期値同士を遠ざけることで、局所解を防ぐと共に収束を早くする。
	// SEE ALSO: http://ilpubs.stanford.edu:8090/778/1/2006-13.pdf
	func seeds(numCluster uint64, data []*entityUserVector.Entity) ([][]float64, error) {
		seeds := make([][]float64, 0)
		e := data[rand.Intn(len(data))]
		seeds = append(seeds, e.Vector)
		for i := uint64(1); i < numCluster; i++ {
			var sum float64
			dist := make([]float64, len(data))
			for k, v := range data {
				_, dist[k] = nearest(seeds, v.Vector)
				sum += dist[k]
			}
			threshold := rand.Float64() * sum
			var stack float64
			var dataNum int
			for k, v := range dist {
				stack += v
				if threshold < stack {
					dataNum = k
					break
				}
			}
			seeds = append(seeds, data[dataNum].Vector)
		}
		return seeds, nil
	}
//}

== まとめ
