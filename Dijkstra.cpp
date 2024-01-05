#include <iostream>   //用于输入和输出流操作，包括 std::cin、std::cout 等。
#include <iomanip>
#include <climits>   //包含了一些整数类型的常量，如 INT_MAX 和 INT_MIN。
#include <algorithm> //提供了一些常见的算法函数，如排序、查找等。例如，std::sort 可用于对容器进行排序。
#include <vector>    //实现了动态数组（向量）的容器，提供了控制大小、访问元素和迭代等功能。
#include <tuple>     //提供了元组的实现，用于存储和操作多个不同类型的值。
#include <set>
#include <queue>    //实现了队列的容器，遵循先进先出（FIFO）的原则。
#include <time.h>   //C 语言中的标准头文件，提供了处理日期和时间的函数。
#include <random>

using namespace std;   //它是一个命名空间的声明，表示在当前代码中使用 std 命名空间中的所有标识符，而无需显式地加上 std:: 前缀。

const int infty = INT_MAX;   //定义了一个名为 infty 的常量，其值被设置为 INT_MAX。INT_MAX 是 <climits> 头文件中定义的整数类型的最大值。通过将 infty 设置为 INT_MAX，实际上将其用作表示无穷大的值。

//Code for printing a vector    //将 vector 容器的内容以类似数组的形式输出到标准输出流。
template<typename T>
ostream& operator<<(ostream& s, vector<T> t) {
	s << "[";
	for (size_t i = 0; i < t.size(); i++) {
		s << t[i] << (i == t.size() - 1 ? "" : ",");
	}
	return s << "] ";
}

//Struct used for each element of the adjacency list. 
struct Neighbour {
	int vertex;
	int weight;
};
//Graph class (uses adjacency list)
class Graph {
public:
	int n; //Num. vertices
	int m; //Num. arcs
	vector<vector<Neighbour> > adj;
	Graph(int n) {
		this->n = n;
		this->m = 0;
		this->adj.resize(n, vector<Neighbour>());   //在构造函数中，将 n 设置为传入的参数值，m 设置为 0，adj 初始化为大小为 n 的二维向量。
	}
	~Graph() {   //析构函数，用于释放图的资源。在析构函数中，将 n 和 m 设置为 0，使用 clear() 方法清空 adj 向量。
		this->n = 0;
		this->m = 0;
		this->adj.clear();
	}
	void addArc(int u, int v, int w) {   //添加一条从顶点 u 到顶点 v 的弧，并指定权重 w。在邻接表中，将 v 和 w 封装为 Neighbour 对象，并添加到 adj[u] 的末尾。同时，将 m 值增加 1。
		this->adj[u].push_back(Neighbour{ v, w });
		this->m++;
	}
};

//Struct and comparison operators used with std::set std::priority_queue)
struct QueueItem {
	int label;
	int vertex;
};
struct minQueueItem {   //它用于自定义优先队列中元素的比较规则，实现了最小优先队列。  升序
	bool operator() (const QueueItem& lhs, const QueueItem& rhs) const {
		return tie(lhs.label, lhs.vertex) < tie(rhs.label, rhs.vertex);
	}
};
struct maxQueueItem {    //  最小堆的规则
	bool operator() (const QueueItem& lhs, const QueueItem& rhs) const {
		return tie(lhs.label, lhs.vertex) > tie(rhs.label, rhs.vertex);
	}
};

//Struct used for each Fibonacci heap node
struct FibonacciNode {    //用于表示斐波那契堆的节点
	int degree;
	FibonacciNode* parent;
	FibonacciNode* child;
	FibonacciNode* left;
	FibonacciNode* right;
	bool mark;
	int key;
	int nodeIndex;
};
//Fibonacci heap class
class FibonacciHeap {
private:
	FibonacciNode* minNode;
	int numNodes;
	vector<FibonacciNode*> degTable;
	vector<FibonacciNode*> nodePtrs;
public:
	FibonacciHeap(int n) {
		//Constructor function
		this->numNodes = 0;
		this->minNode = NULL;
		this->degTable = {};
		this->nodePtrs.resize(n);
	}
	~FibonacciHeap() {
		//Destructor function
		this->numNodes = 0;
		this->minNode = NULL;
		this->degTable.clear();
		this->nodePtrs.clear();
	}
	int size() {
		//Number of nodes in the heap
		return this->numNodes;
	}
	bool empty() {
		//Is the heap empty?
		if (this->numNodes > 0) return false;
		else return true;
	}
	void insert(int u, int key) {
		//Insert the vertex u with the specified key (value for L(u)) into the Fibonacci heap. O(1) operation
		this->nodePtrs[u] = new FibonacciNode;  // 创建一个新的 FibonacciNode 对象，并将其指针存储在 nodePtrs 数组中，使用顶点 u 作为索引
		this->nodePtrs[u]->nodeIndex = u;   // 设置新节点的索引和关键值
		FibonacciNode* node = this->nodePtrs[u];
		node->key = key;
		node->degree = 0;
		node->parent = NULL;
		node->child = NULL;
		node->left = node;
		node->right = node;
		node->mark = false;
		FibonacciNode* minN = this->minNode;   // 获取当前最小节点
		if (minN != NULL) {    // 如果堆中已经存在最小节点 minN
			// 将新节点插入到 minN 的左侧
			FibonacciNode* minLeft = minN->left;
			minN->left = node;
			node->right = minN;
			node->left = minLeft;
			minLeft->right = node;
		}
		// 如果堆为空或者新节点的关键值小于当前最小节点的关键值，则更新最小节点指针
		if (minN == NULL || minN->key > node->key) {
			this->minNode = node;
		}
		this->numNodes++;  // 增加堆中节点的数量
	}
	//从堆中提取具有最小关键值的节点
	FibonacciNode* extractMin() {
		//Extract the node with the minimum key from the heap. O(log n) operation, where n is the number of nodes in the heap
		FibonacciNode* minN = this->minNode;   // 获取当前最小节点
		if (minN != NULL) {
			int deg = minN->degree;  // 获取最小节点的度数
			FibonacciNode* currChild = minN->child;  // 处理最小节点的子节点
			FibonacciNode* remChild;
			for (int i = 0; i < deg; i++) {
				remChild = currChild;
				currChild = currChild->right;
				_existingToRoot(remChild);  // 将子节点从堆中移除并添加到根链表中
			}
			_removeNodeFromRoot(minN);  // 从根链表中移除最小节点
			this->numNodes--;
			if (this->numNodes == 0) {
				this->minNode = NULL;
			}
			else {
				this->minNode = minN->right;   // 更新最小节点指针，并进行合并操作
				FibonacciNode* minNLeft = minN->left;
				this->minNode->left = minNLeft;
				minNLeft->right = this->minNode;
				_consolidate();
			}
		}
		return minN;
	}
	void decreaseKey(int u, int newKey) {   //用于减小斐波那契堆中具有索引 u 的节点的关键值。
		//Decrease the key of the node in the Fibonacci heap that has index u. O(1) operation
		FibonacciNode* node = this->nodePtrs[u];   // 获取具有索引 u 的节点
		if (newKey > node->key) return;   // 如果新的关键值大于节点当前的关键值，则不进行任何操作，直接返回
		node->key = newKey;   // 更新节点的关键值为新的关键值
		if (node->parent != NULL) {   // 如果节点有父节点
			if (node->key < node->parent->key) {
				FibonacciNode* parentNode = node->parent;
				_cut(node);  // 切断节点与其父节点之间的链接
				_cascadingCut(parentNode);  // 进行级联切断操作，直到满足堆性质
			}
		}
		if (node->key < this->minNode->key) {   // 如果节点的关键值小于当前最小节点的关键值，则更新最小节点指针
			this->minNode = node;
		}
	}
private:
	//The following are private functions used by the public methods above
	void _existingToRoot(FibonacciNode* newNode) {
		FibonacciNode* minN = this->minNode;
		newNode->parent = NULL;
		newNode->mark = false;
		if (minN != NULL) {
			FibonacciNode* minLeft = minN->left;
			minN->left = newNode;
			newNode->right = minN;
			newNode->left = minLeft;
			minLeft->right = newNode;
			if (minN->key > newNode->key) {
				this->minNode = newNode;
			}
		}
		else {
			this->minNode = newNode;
			newNode->right = newNode;
			newNode->left = newNode;
		}
	}
	void _removeNodeFromRoot(FibonacciNode* node) {
		if (node->right != node) {
			node->right->left = node->left;
			node->left->right = node->right;
		}
		if (node->parent != NULL) {
			if (node->parent->degree == 1) {
				node->parent->child = NULL;
			}
			else {
				node->parent->child = node->right;
			}
			node->parent->degree--;
		}
	}
	void _cut(FibonacciNode* node) {
		_removeNodeFromRoot(node);
		_existingToRoot(node);
	}
	void _addChild(FibonacciNode* parentNode, FibonacciNode* newChildNode) {
		if (parentNode->degree == 0) {
			parentNode->child = newChildNode;
			newChildNode->right = newChildNode;
			newChildNode->left = newChildNode;
			newChildNode->parent = parentNode;
		}
		else {
			FibonacciNode* child1 = parentNode->child;
			FibonacciNode* child1Left = child1->left;
			child1->left = newChildNode;
			newChildNode->right = child1;
			newChildNode->left = child1Left;
			child1Left->right = newChildNode;
		}
		newChildNode->parent = parentNode;
		parentNode->degree++;
	}
	void _cascadingCut(FibonacciNode* node) {
		FibonacciNode* parentNode = node->parent;
		if (parentNode != NULL) {
			if (node->mark == false) {
				node->mark = true;
			}
			else {
				_cut(node);
				_cascadingCut(parentNode);
			}
		}
	}
	void _link(FibonacciNode* highNode, FibonacciNode* lowNode) {
		_removeNodeFromRoot(highNode);
		_addChild(lowNode, highNode);
		highNode->mark = false;
	}
	void _consolidate() {
		int deg, rootCnt = 0;
		if (this->numNodes > 1) {
			this->degTable.clear();
			FibonacciNode* currNode = this->minNode;
			FibonacciNode* currDeg, * currConsolNode;
			FibonacciNode* temp = this->minNode, * itNode = this->minNode;
			do {
				rootCnt++;
				itNode = itNode->right;
			} while (itNode != temp);
			for (int cnt = 0; cnt < rootCnt; cnt++) {
				currConsolNode = currNode;
				currNode = currNode->right;
				deg = currConsolNode->degree;
				while (true) {
					while (deg >= int(this->degTable.size())) {
						this->degTable.push_back(NULL);
					}
					if (this->degTable[deg] == NULL) {
						this->degTable[deg] = currConsolNode;
						break;
					}
					else {
						currDeg = this->degTable[deg];
						if (currConsolNode->key > currDeg->key) {
							swap(currConsolNode, currDeg);
						}
						if (currDeg == currConsolNode) break;
						_link(currDeg, currConsolNode);
						this->degTable[deg] = NULL;
						deg++;
					}
				}
			}
			this->minNode = NULL;
			for (size_t i = 0; i < this->degTable.size(); i++) {
				if (this->degTable[i] != NULL) {
					_existingToRoot(this->degTable[i]);
				}
			}
		}
	}
};
//End of FibonacciHeap class

tuple<vector<int>, vector<int>> dijkstraFibonacci(Graph& G, int s) {
	//Dijkstra's algorithm using a Fibonacci heap object
	int u, v, w;
	FibonacciHeap Q(G.n);
	vector<int> L(G.n), P(G.n);
	vector<bool> D(G.n);
	for (int u = 0; u < G.n; u++) {
		D[u] = false;
		L[u] = infty;
		P[u] = -1;
	}
	L[s] = 0;
	Q.insert(s, 0);
	while (!Q.empty()) {
		u = Q.extractMin()->nodeIndex;
		D[u] = true;
		for (auto& neighbour : G.adj[u]) {
			v = neighbour.vertex;
			w = neighbour.weight;
			if (D[v] == false) {
				if (L[u] + w < L[v]) {
					if (L[v] == infty) {
						Q.insert(v, L[u] + w);
					}
					else {
						Q.decreaseKey(v, L[u] + w);
					}
					L[v] = L[u] + w;
					P[v] = u;
				}
			}
		}
	}
	return make_tuple(L, P);
}

tuple<vector<int>, vector<int>> dijkstraTree(Graph& G, int s) {
	//Dijkstra's algorithm using a self-balancing binary tree (C++ set)
	int u, v, w;
	set<QueueItem, minQueueItem> Q;   // 自动根据指定的比较规则对元素进行排序，并且不允许重复元素
	vector<int> L(G.n), P(G.n);
	vector<bool> D(G.n);
	for (u = 0; u < G.n; u++) {
		D[u] = false;
		L[u] = infty;
		P[u] = -1;
	}
	L[s] = 0;
	Q.emplace(QueueItem{ 0,s });
	// 会先删除原始点的路径，再生成一个新的路径。这种频繁的删除添加操作不适合数据密集情况
	while (!Q.empty()) {
		u = (*Q.begin()).vertex;  //获取第一个元素。通过 (*Q.begin()).vertex 的方式访问该元素的 vertex 成员
		Q.erase(*Q.begin());   // 从有序集合 Q 中删除第一个元素
		D[u] = true;
		for (auto& neighbour : G.adj[u]) {
			v = neighbour.vertex;
			w = neighbour.weight;
			if (D[v] == false) {
				if (L[u] + w < L[v]) {
					if (L[v] == infty) {
						Q.emplace(QueueItem{ L[u] + w, v });
					}
					else {
						Q.erase({ L[v], v });
						Q.emplace(QueueItem{ L[u] + w, v });
					}
					L[v] = L[u] + w;
					P[v] = u;
				}
			}
		}
	}
	return make_tuple(L, P);
}

tuple<vector<int>, vector<int>> dijkstraHeap(Graph& G, int s) {
	//Dijkstra's algorithm using a binary heap (C++ priority_queue)
	// 优先队列默认是最大堆，这里使用最小堆
	int u, v, w;
	priority_queue<QueueItem, vector<QueueItem>, maxQueueItem> Q;   //声明了一个优先队列 Q，用于存储待处理的节点及其距离信息。
	vector<int> L(G.n), P(G.n);
	vector<bool> D(G.n);
	for (u = 0; u < G.n; u++) {
		D[u] = false;
		L[u] = infty;
		P[u] = -1;
	}
	L[s] = 0;
	Q.emplace(QueueItem{ 0,s });   //将源节点 s 添加到优先队列 Q 中。这里使用了 emplace 函数，通过构造一个 QueueItem 类型的对象，并将其插入到队列中。
	// 这个最小堆不适合数据密集情况，因为priority_queue在维护堆的时候会消耗更多时间
	while (!Q.empty()) {
		u = Q.top().vertex;   //获取队列中距离最小的节点
		Q.pop();   //从队列中移除该节点
		if (D[u] != true) {   // 未经处理
			D[u] = true;
			for (auto& neighbour : G.adj[u]) {
				v = neighbour.vertex;
				w = neighbour.weight;
				if (D[v] == false) {
					if (L[u] + w < L[v]) {
						Q.emplace(QueueItem{ L[u] + w, v });
						L[v] = L[u] + w;
						P[v] = u;
					}
				}
			}
		}
	}
	return make_tuple(L, P);
}

tuple<vector<int>, vector<int>> dijkstraBasic(Graph& G, int s) {
	//Basic Dijkstra's algorithm (O(n^2) complexity)
	int u, v, w, minL;
	size_t i, uPos;
	vector<int> L(G.n), P(G.n), Candidates;  // 定义向量 L, P, Candidates，用于存储距离、前驱节点和候选节点的信息
	vector<bool> D(G.n);   // 定义向量 D，用于标记节点是否被访问过
	for (u = 0; u < G.n; u++) {
		D[u] = false; // 将节点 u 的访问状态标记为未访问
		L[u] = infty; // 将节点 u 的距离初始化为无穷大
		P[u] = -1; // 将节点 u 的前驱节点初始化为 -1，表示没有前驱节点
	}
	L[s] = 0;
	Candidates.push_back(s);   //将节点 s 添加到 Candidates 向量的末尾。
	while (!Candidates.empty()) {
		uPos = 0;
		minL = L[Candidates[0]];
		for (i = 1; i < Candidates.size(); i++) {
			if (L[Candidates[i]] < minL) {
				minL = L[Candidates[i]];
				uPos = i;
			}
		}
		u = Candidates[uPos];
		swap(Candidates[uPos], Candidates.back());   // 将当前处理的节点移动到列表末尾
		Candidates.pop_back();   // 将当前节点 u 从候选节点列表中移除
		D[u] = true;
		for (auto& neighbour : G.adj[u]) {
			v = neighbour.vertex;
			w = neighbour.weight;
			if (D[v] == false) {   // 如果该点未访问
				if (L[u] + w < L[v]) {
					if (L[v] == infty) {
						Candidates.push_back(v);
					}
					L[v] = L[u] + w;
					P[v] = u;
				}
			}
		}
	}
	return make_tuple(L, P);   //使用 make_tuple 函数创建了一个元组，其中包含了两个变量 L 和 P
}

vector<int> getPath(int u, int v, vector<int>& P) {
	//Get the u-v-path specified by the predecessor vector P
	vector<int> path;
	int x = v;
	if (P[x] == -1) return path;
	while (x != u) {
		path.push_back(x);
		x = P[x];
	}
	path.push_back(u);
	reverse(path.begin(), path.end());  // 转置
	return path;
}

int main() {
	//Construct a small example graph. (A directed cycle on 5 vertices here. All arcs have weight 10)
	//Graph G(5);
	//G.addArc(0, 1, 5);
	//G.addArc(0, 2, 7);
	//G.addArc(1, 2, 3);
	//G.addArc(1, 3, 4);
	//G.addArc(1, 4, 6);
	//G.addArc(2, 3, 4);
	//G.addArc(2, 4, 8);
	//G.addArc(3, 4, 5);
	//G.addArc(4, 0, 7);

	int numNodes = 5e+3;
	Graph G(numNodes);
	
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int> dist(1, 1e+3);
	
	for (int i = 0; i < numNodes; i++) {
	    for (int j = 0; j < numNodes; j++) {
	        if (i != j) {
	            int weight = dist(gen);
	            G.addArc(i, j, weight);
	        }
	    }
	}

	//Set the source vertex and declare some variables
	int s = 0;
	vector<int> L, P;

	//Execute Dijkstra's algorithm using a Fibonacci heap    
	clock_t start = clock();
	tie(L, P) = dijkstraFibonacci(G, s);
	double duration1 = ((double)clock() - start) / CLOCKS_PER_SEC;

	//Execute Dijkstra's algorithm using a self-balancing binary tree
	start = clock();
	tie(L, P) = dijkstraTree(G, s);
	double duration2 = ((double)clock() - start) / CLOCKS_PER_SEC;

	//Execute Dijkstra's algorithm using a binary heap
	start = clock();
	tie(L, P) = dijkstraHeap(G, s);
	double duration3 = ((double)clock() - start) / CLOCKS_PER_SEC;

	//Execute basic version of Dijkstra's algorithm
	start = clock();
	tie(L, P) = dijkstraBasic(G, s);    //tie 函数  元组
	double duration4 = ((double)clock() - start) / CLOCKS_PER_SEC;

	//Output some information
	cout << "Input graph has " << G.n << " vertices and " << G.m << " arcs\n";
	cout << "Dijkstra with Fibonacci heap took      " << duration1 << " sec.\n";
	cout << "Dijkstra with self-balancing tree took " << duration2 << " sec.\n";
	cout << "Dijkstra with binary heap took         " << duration3 << " sec.\n";
	cout << "Dijkstra (basic form) took             " << duration4 << " sec.\n";
	cout << "Shortest paths from source to each vertex are as follows:\n";
	//for (int u = 0; u < G.n; u++) {
	//	cout << "v-" << s << " to v-" << u << ",\t";
	//	if (L[u] == infty) {
	//		cout << "len = infinity. No path exists\n";
	//	}
	//	else {
	//		cout << "len = " << setw(4) << L[u] << "\tpath = " << getPath(s, u, P) << "\n";   //setw(4) 设置了输出的宽度为 4 个字符
	//	}
	//}

	//批量注释: Ctrl + K, Ctrl + C 取消注释 : Ctrl + K, Ctrl + U
}
