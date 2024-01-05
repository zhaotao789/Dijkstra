#include <iostream>   //������������������������ std::cin��std::cout �ȡ�
#include <iomanip>
#include <climits>   //������һЩ�������͵ĳ������� INT_MAX �� INT_MIN��
#include <algorithm> //�ṩ��һЩ�������㷨�����������򡢲��ҵȡ����磬std::sort �����ڶ�������������
#include <vector>    //ʵ���˶�̬���飨���������������ṩ�˿��ƴ�С������Ԫ�غ͵����ȹ��ܡ�
#include <tuple>     //�ṩ��Ԫ���ʵ�֣����ڴ洢�Ͳ��������ͬ���͵�ֵ��
#include <set>
#include <queue>    //ʵ���˶��е���������ѭ�Ƚ��ȳ���FIFO����ԭ��
#include <time.h>   //C �����еı�׼ͷ�ļ����ṩ�˴������ں�ʱ��ĺ�����
#include <random>

using namespace std;   //����һ�������ռ����������ʾ�ڵ�ǰ������ʹ�� std �����ռ��е����б�ʶ������������ʽ�ؼ��� std:: ǰ׺��

const int infty = INT_MAX;   //������һ����Ϊ infty �ĳ�������ֵ������Ϊ INT_MAX��INT_MAX �� <climits> ͷ�ļ��ж�����������͵����ֵ��ͨ���� infty ����Ϊ INT_MAX��ʵ���Ͻ���������ʾ������ֵ��

//Code for printing a vector    //�� vector �����������������������ʽ�������׼�������
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
		this->adj.resize(n, vector<Neighbour>());   //�ڹ��캯���У��� n ����Ϊ����Ĳ���ֵ��m ����Ϊ 0��adj ��ʼ��Ϊ��СΪ n �Ķ�ά������
	}
	~Graph() {   //���������������ͷ�ͼ����Դ�������������У��� n �� m ����Ϊ 0��ʹ�� clear() ������� adj ������
		this->n = 0;
		this->m = 0;
		this->adj.clear();
	}
	void addArc(int u, int v, int w) {   //���һ���Ӷ��� u ������ v �Ļ�����ָ��Ȩ�� w�����ڽӱ��У��� v �� w ��װΪ Neighbour ���󣬲���ӵ� adj[u] ��ĩβ��ͬʱ���� m ֵ���� 1��
		this->adj[u].push_back(Neighbour{ v, w });
		this->m++;
	}
};

//Struct and comparison operators used with std::set std::priority_queue)
struct QueueItem {
	int label;
	int vertex;
};
struct minQueueItem {   //�������Զ������ȶ�����Ԫ�صıȽϹ���ʵ������С���ȶ��С�  ����
	bool operator() (const QueueItem& lhs, const QueueItem& rhs) const {
		return tie(lhs.label, lhs.vertex) < tie(rhs.label, rhs.vertex);
	}
};
struct maxQueueItem {    //  ��С�ѵĹ���
	bool operator() (const QueueItem& lhs, const QueueItem& rhs) const {
		return tie(lhs.label, lhs.vertex) > tie(rhs.label, rhs.vertex);
	}
};

//Struct used for each Fibonacci heap node
struct FibonacciNode {    //���ڱ�ʾ쳲������ѵĽڵ�
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
		this->nodePtrs[u] = new FibonacciNode;  // ����һ���µ� FibonacciNode ���󣬲�����ָ��洢�� nodePtrs �����У�ʹ�ö��� u ��Ϊ����
		this->nodePtrs[u]->nodeIndex = u;   // �����½ڵ�������͹ؼ�ֵ
		FibonacciNode* node = this->nodePtrs[u];
		node->key = key;
		node->degree = 0;
		node->parent = NULL;
		node->child = NULL;
		node->left = node;
		node->right = node;
		node->mark = false;
		FibonacciNode* minN = this->minNode;   // ��ȡ��ǰ��С�ڵ�
		if (minN != NULL) {    // ��������Ѿ�������С�ڵ� minN
			// ���½ڵ���뵽 minN �����
			FibonacciNode* minLeft = minN->left;
			minN->left = node;
			node->right = minN;
			node->left = minLeft;
			minLeft->right = node;
		}
		// �����Ϊ�ջ����½ڵ�Ĺؼ�ֵС�ڵ�ǰ��С�ڵ�Ĺؼ�ֵ���������С�ڵ�ָ��
		if (minN == NULL || minN->key > node->key) {
			this->minNode = node;
		}
		this->numNodes++;  // ���Ӷ��нڵ������
	}
	//�Ӷ�����ȡ������С�ؼ�ֵ�Ľڵ�
	FibonacciNode* extractMin() {
		//Extract the node with the minimum key from the heap. O(log n) operation, where n is the number of nodes in the heap
		FibonacciNode* minN = this->minNode;   // ��ȡ��ǰ��С�ڵ�
		if (minN != NULL) {
			int deg = minN->degree;  // ��ȡ��С�ڵ�Ķ���
			FibonacciNode* currChild = minN->child;  // ������С�ڵ���ӽڵ�
			FibonacciNode* remChild;
			for (int i = 0; i < deg; i++) {
				remChild = currChild;
				currChild = currChild->right;
				_existingToRoot(remChild);  // ���ӽڵ�Ӷ����Ƴ�����ӵ���������
			}
			_removeNodeFromRoot(minN);  // �Ӹ��������Ƴ���С�ڵ�
			this->numNodes--;
			if (this->numNodes == 0) {
				this->minNode = NULL;
			}
			else {
				this->minNode = minN->right;   // ������С�ڵ�ָ�룬�����кϲ�����
				FibonacciNode* minNLeft = minN->left;
				this->minNode->left = minNLeft;
				minNLeft->right = this->minNode;
				_consolidate();
			}
		}
		return minN;
	}
	void decreaseKey(int u, int newKey) {   //���ڼ�С쳲��������о������� u �Ľڵ�Ĺؼ�ֵ��
		//Decrease the key of the node in the Fibonacci heap that has index u. O(1) operation
		FibonacciNode* node = this->nodePtrs[u];   // ��ȡ�������� u �Ľڵ�
		if (newKey > node->key) return;   // ����µĹؼ�ֵ���ڽڵ㵱ǰ�Ĺؼ�ֵ���򲻽����κβ�����ֱ�ӷ���
		node->key = newKey;   // ���½ڵ�Ĺؼ�ֵΪ�µĹؼ�ֵ
		if (node->parent != NULL) {   // ����ڵ��и��ڵ�
			if (node->key < node->parent->key) {
				FibonacciNode* parentNode = node->parent;
				_cut(node);  // �жϽڵ����丸�ڵ�֮�������
				_cascadingCut(parentNode);  // ���м����жϲ�����ֱ�����������
			}
		}
		if (node->key < this->minNode->key) {   // ����ڵ�Ĺؼ�ֵС�ڵ�ǰ��С�ڵ�Ĺؼ�ֵ���������С�ڵ�ָ��
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
	set<QueueItem, minQueueItem> Q;   // �Զ�����ָ���ıȽϹ����Ԫ�ؽ������򣬲��Ҳ������ظ�Ԫ��
	vector<int> L(G.n), P(G.n);
	vector<bool> D(G.n);
	for (u = 0; u < G.n; u++) {
		D[u] = false;
		L[u] = infty;
		P[u] = -1;
	}
	L[s] = 0;
	Q.emplace(QueueItem{ 0,s });
	// ����ɾ��ԭʼ���·����������һ���µ�·��������Ƶ����ɾ����Ӳ������ʺ������ܼ����
	while (!Q.empty()) {
		u = (*Q.begin()).vertex;  //��ȡ��һ��Ԫ�ء�ͨ�� (*Q.begin()).vertex �ķ�ʽ���ʸ�Ԫ�ص� vertex ��Ա
		Q.erase(*Q.begin());   // �����򼯺� Q ��ɾ����һ��Ԫ��
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
	// ���ȶ���Ĭ�������ѣ�����ʹ����С��
	int u, v, w;
	priority_queue<QueueItem, vector<QueueItem>, maxQueueItem> Q;   //������һ�����ȶ��� Q�����ڴ洢������Ľڵ㼰�������Ϣ��
	vector<int> L(G.n), P(G.n);
	vector<bool> D(G.n);
	for (u = 0; u < G.n; u++) {
		D[u] = false;
		L[u] = infty;
		P[u] = -1;
	}
	L[s] = 0;
	Q.emplace(QueueItem{ 0,s });   //��Դ�ڵ� s ��ӵ����ȶ��� Q �С�����ʹ���� emplace ������ͨ������һ�� QueueItem ���͵Ķ��󣬲�������뵽�����С�
	// �����С�Ѳ��ʺ������ܼ��������Ϊpriority_queue��ά���ѵ�ʱ������ĸ���ʱ��
	while (!Q.empty()) {
		u = Q.top().vertex;   //��ȡ�����о�����С�Ľڵ�
		Q.pop();   //�Ӷ������Ƴ��ýڵ�
		if (D[u] != true) {   // δ������
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
	vector<int> L(G.n), P(G.n), Candidates;  // �������� L, P, Candidates�����ڴ洢���롢ǰ���ڵ�ͺ�ѡ�ڵ����Ϣ
	vector<bool> D(G.n);   // �������� D�����ڱ�ǽڵ��Ƿ񱻷��ʹ�
	for (u = 0; u < G.n; u++) {
		D[u] = false; // ���ڵ� u �ķ���״̬���Ϊδ����
		L[u] = infty; // ���ڵ� u �ľ����ʼ��Ϊ�����
		P[u] = -1; // ���ڵ� u ��ǰ���ڵ��ʼ��Ϊ -1����ʾû��ǰ���ڵ�
	}
	L[s] = 0;
	Candidates.push_back(s);   //���ڵ� s ��ӵ� Candidates ������ĩβ��
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
		swap(Candidates[uPos], Candidates.back());   // ����ǰ����Ľڵ��ƶ����б�ĩβ
		Candidates.pop_back();   // ����ǰ�ڵ� u �Ӻ�ѡ�ڵ��б����Ƴ�
		D[u] = true;
		for (auto& neighbour : G.adj[u]) {
			v = neighbour.vertex;
			w = neighbour.weight;
			if (D[v] == false) {   // ����õ�δ����
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
	return make_tuple(L, P);   //ʹ�� make_tuple ����������һ��Ԫ�飬���а������������� L �� P
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
	reverse(path.begin(), path.end());  // ת��
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
	tie(L, P) = dijkstraBasic(G, s);    //tie ����  Ԫ��
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
	//		cout << "len = " << setw(4) << L[u] << "\tpath = " << getPath(s, u, P) << "\n";   //setw(4) ����������Ŀ��Ϊ 4 ���ַ�
	//	}
	//}

	//����ע��: Ctrl + K, Ctrl + C ȡ��ע�� : Ctrl + K, Ctrl + U
}
