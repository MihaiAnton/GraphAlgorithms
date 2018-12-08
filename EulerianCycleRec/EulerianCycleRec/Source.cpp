#include <fstream>
#include <list>

using namespace std;

struct graph {
	int vertexCount;
	int edgeCount;
	int** mat;
	list<int>* adjList;
};

void readGraph(graph& g, ifstream& in) {
	in >> g.vertexCount >> g.edgeCount;
	g.adjList = new list<int>[g.vertexCount + 1];
	g.mat = new int*[g.vertexCount + 1];
	for (int i = 1; i <= g.vertexCount; i++) {
		g.mat[i] = new int[g.vertexCount + 1];
		memset(g.mat[i], 0, g.vertexCount + 1);
	}


	for (int i = 1; i <= g.edgeCount; i++) {
		int a, b;
		in >> a >> b;
		g.adjList[a].push_back(b);
		g.adjList[b].push_back(a);

		g.mat[a][b] = 1;
		g.mat[b][a] = 1;

	}
}

void euler(graph& g, int source, list<int>& path) {

	while (!g.adjList[source].empty()) {
		int adj = g.adjList[source].back();
		if (!(g.mat[source][adj] && g.mat[adj][source])) {
			g.adjList[source].pop_back();
			continue;
		}
		else {
			g.mat[source][adj] = 0;
			g.mat[adj][source] = 0;
			g.adjList[source].pop_back();
		}
		
		euler(g, adj, path);
	}
	path.push_back(source);
}

void printPath(list<int>& path, ofstream& out) {

	for (const auto& elem : path) {
		out << elem << " ";
	}
}




int main() {

	graph g;

	ifstream in("euler.in");
	readGraph(g, in);
	in.close();

	list<int> path;
	euler(g, 1, path);

	ofstream out("euler.out");
	printPath(path, out);
	out.close();

	return 0;
}