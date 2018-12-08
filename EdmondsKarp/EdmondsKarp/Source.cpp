#include <fstream>
#include <iostream>
#include <list>

using namespace std;

struct edge {
	int a, b;
	int flow;
	bool isRev;
	edge* rev;
	
};

struct graph {

	int vertexCount;
	int edgeCount;

	list<edge*>* adjList;

	int nr0 = 0;
};

void readGraph(graph& g, ifstream& in) {

	in >> g.vertexCount >> g.edgeCount;
	g.adjList = new list<edge*>[g.vertexCount + 1];

	for (int i = 1; i <= g.edgeCount; i++) {

		int a, b, flow;
		in >> a >> b >> flow;
		edge* norm = new edge{ a,b,flow, false };
		edge* rev = new edge{ b,a,0, true, norm };
		norm->rev = rev;
	
		g.adjList[a].push_back(norm);
		g.adjList[b].push_back(rev);	
	}
}

void processPath(graph& g, list<edge*>& path) {

	if (path.empty())
		return;

	int mini = (*path.begin())->flow;

	for (auto edg : path) {
		//cout << edg->a << " ";
		mini = mini < edg->flow ? mini : edg->flow;
	}

	//cout << path.back()->b << "\n";

	for (auto edg : path) {
		edg->flow -= mini;
		edg->rev->flow += mini;

		if (edg->flow == 0)
			g.nr0++;
	}
}

void pathDiscover(graph& g, int crt, int sink, bool* viz, list<edge*>& path) {

	if (crt == sink) {
		processPath(g, path);
	}

	else {

		viz[crt] = true;

		for (auto edg : g.adjList[crt]) {
			if (!viz[edg->b] && edg->flow > 0 && g.nr0 == 0) {
				path.push_back(edg);
				pathDiscover(g, edg->b, sink, viz, path);
			}
		}
		viz[crt] = false;
		
	}
	if (!path.empty()) {
		if (path.back()->flow == 0)
			g.nr0--;
		path.pop_back();
	}
}

void edmondsKarp(graph& g, int source, int sink) {

	list<edge*> path;
	bool* viz = (bool*)calloc(g.vertexCount + 1, sizeof(bool));
	pathDiscover(g, source, sink, viz, path);
}


int main() {

	graph g;

	ifstream in("maxflow.in");
	readGraph(g, in);
	in.close();

	edmondsKarp(g, 1, g.vertexCount);

	int flow = 0;
	for (auto adj : g.adjList[g.vertexCount]) {
		if (adj->isRev)
			flow += adj->flow;
	}

	ofstream out("maxflow.out");
	out << flow;
	out.close();

	return 0;
}