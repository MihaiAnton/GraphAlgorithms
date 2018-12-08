#include <fstream>
#include <list>
#include <queue>
#include <iostream>

using namespace std;

struct edge {
	int a, b;
	int flow;
	edge* rev;
};

struct graph {
	int vertexCount;
	int edgeCount;

	list<edge*>* adjList;	
	list<edge*> goingToSink;
	edge** parent;
	bool* viz;
};

void readGraph(graph& g, ifstream& in) {

	in >> g.vertexCount >> g.edgeCount;
	g.adjList = new list<edge*>[g.vertexCount + 1];
	g.parent = (edge**)malloc((g.vertexCount + 1) * sizeof(edge*));
	g.viz = (bool*)calloc(g.vertexCount + 1, sizeof(bool));

	for (int i = 1; i <= g.edgeCount; i++) {

		int a, b, flow;
		in >> a >> b >> flow;

		edge* edg = new edge{ a,b,flow };
		edge* rev = new edge{ b,a,0, edg };
		edg->rev = rev;
		g.adjList[a].push_back(edg);
		g.adjList[b].push_back(rev);


		

		if (b == g.vertexCount)
			g.goingToSink.push_back(edg);
	}
}

bool findAugmentingPath(graph& g, int source, int sink) {

	queue<int> q;
	q.push(source);

	for (int i = 1; i <= g.vertexCount; i++) {
		g.viz[i] = false;
	}

	while (!q.empty()) {

		int crt = q.front();
		q.pop();
	
		g.viz[crt] = true;

		if (crt == sink)
			continue;

		for (auto edg : g.adjList[crt]) {

			if (!g.viz[edg->b] && edg->flow > 0) {
				g.parent[edg->b] = edg;
				q.push(edg->b);
			}
		}
	}

	return g.viz[sink];
}

int fordFulkerson(graph& g, int source, int sink) {

	int maxFlow = 0;
	int paths = 0;
	int founds = 0;
	g.parent[source] = NULL;
	while (findAugmentingPath(g, source, sink)) {
	
		for (auto edg : g.goingToSink) {

			if (edg->flow > 0 && g.viz[edg->a]) {
				g.parent[sink] = edg;
				
				int mini = edg->flow;
				edge* copy = edg;
				for (; copy != NULL; copy = g.parent[copy->a]) {
					mini = mini < copy->flow ? mini : copy->flow;
				}

				maxFlow += mini;

				copy = edg;
				for (; copy != NULL; copy = g.parent[copy->a]) {
					copy->flow -= mini;
					copy->rev->flow += mini;
				}
			}
		}
	}
	cout << founds << "\n" << paths;
	return maxFlow;
}




int main() {

	graph g;

	ifstream in("maxflow.in");
	readGraph(g, in);
	in.close();

	int maxFlow = fordFulkerson(g, 1, g.vertexCount);

	ofstream out("maxflow.out");
	out << maxFlow;
	out.close();


	return 0;
}









