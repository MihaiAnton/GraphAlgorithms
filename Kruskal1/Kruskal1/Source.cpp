#include <fstream>
#include <list>
#include <queue>


using namespace std;

struct edge {
	int from, to, weight;
	bool visited;

	inline bool operator<(const edge& ot)const { return weight > ot.weight; }
	inline bool operator>(const edge& ot)const { return !(*this < ot); }
};

struct graph {
	int vertexCount, edgeCount;
	list<edge*>* adjList;
};

void readGraph(graph& g, ifstream& in) {
	in >> g.vertexCount >> g.edgeCount;
	g.adjList = new list<edge*>[g.vertexCount + 1];

	for (int i = 1; i <= g.edgeCount; i++) {

		int from, to, weight;
		in >> from >> to >> weight;
		edge* edg = new edge{ from, to, weight, false };
		g.adjList[from].push_back(edg);
		//g.adjList[to].push_back(edg);		
	}
}

void markOtherSet(int* sets, int capacity, int otherId, int thisId) {
	for (int i = 1; i <= capacity; i++)
		if (sets[i] == otherId)
			sets[i] = thisId;
}

list<edge> kruskal(graph& g) {

	if (g.vertexCount == 0)
		return (list<edge>)NULL;

	list<edge> tree;
	int* set = (int*)calloc((g.vertexCount + 1) , sizeof(int));
	int nextId = 1;

	priority_queue<edge> heap;
	for (int i = 1; i <= g.vertexCount; i++) {
		for (auto edg : g.adjList[i]) {
			heap.push(*edg);
		}
	}

	int found = 0;
	while (found < g.vertexCount - 1) {

		edge crt = heap.top();
		heap.pop();

		if (set[crt.from] != set[crt.to] || (set[crt.from] == 0 && set[crt.to] == 0)) {

			int maximumSetId = max(set[crt.from], set[crt.to]);
			int otherId = set[crt.to] == maximumSetId ? set[crt.from] : set[crt.to];

			if (maximumSetId == 0) {
				maximumSetId = nextId++;
				set[crt.from] = maximumSetId;
				set[crt.to] = maximumSetId;
			}
			else if (otherId == 0) {
				set[crt.from] == 0 ? set[crt.from] = maximumSetId : set[crt.to] = maximumSetId;
			}
			else {
				
				markOtherSet(set, g.vertexCount, otherId, maximumSetId);
			}



			tree.push_back(crt);
			found++;
		}



	}

	return tree;
}



void printTree(list<edge> tree, ofstream& out) {

	int sum = 0;
	for (auto edg : tree) {
		sum += edg.weight;
	}

	out << sum << "\n" << tree.size() << "\n";

	for (auto edg : tree) {
		out << edg.to <<" "<< edg.from << "\n";
	}
}











int main() {

	graph g;

	ifstream in("apm.in");
	readGraph(g, in);
	in.close();

	list<edge> tree = kruskal(g);

	ofstream out("apm.out");
	printTree(tree, out);
	out.close();

	return 0;
}