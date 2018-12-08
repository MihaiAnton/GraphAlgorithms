#include <fstream>
#include <list>

using namespace std;

struct graph {

	int edgeCount;
	int leftPartCount;
	int rightPartCount;
	int vertCount;

	list<int>* adjNodes;

	int* matchOfL;
	int* matchOfR;
	bool* visited;
};

void readGraph(graph& g, ifstream& in) {
	in >> g.leftPartCount >> g.rightPartCount >> g.edgeCount;
	g.vertCount = g.leftPartCount + g.rightPartCount;
	g.adjNodes = new list<int>[g.vertCount+1];
	g.matchOfL = (int*)calloc((g.vertCount + 1) , sizeof(int));
	g.matchOfR = (int*)calloc((1 + g.vertCount) , sizeof(int));
	g.visited = (bool*)calloc(g.vertCount+1, sizeof(bool));

	for (int i = 1; i <= g.edgeCount; i++) {
		int from, to;
		in >> from >> to;
		to += g.leftPartCount;
		g.adjNodes[from].push_back(to);
	}
}

bool findMatch(graph& g, int vertex) {

	//tries to find a match for vertex; returns true if succesful

	if (g.visited[vertex])		//already matched
		return false;
	g.visited[vertex] = true;

	for (auto adj : g.adjNodes[vertex])
		if (g.matchOfR[adj] == 0) {
			g.matchOfL[vertex] = adj;
			g.matchOfR[adj] = vertex;
			return true;
		}

	for (auto adj : g.adjNodes[vertex])
		if (findMatch(g, g.matchOfR[adj])) {
			g.matchOfL[vertex] = adj;
			g.matchOfR[adj] = vertex;
			return true;
		}

	return false;
}

int hopcroftKarp(graph& g) {

	//find the maximum matching and changes the graph data acording to it
	

	
	bool changed = true;	//there was a change in the last step or is the first iteration

	while (changed) {
		changed = false;
		
		for (int vertex = 1; vertex <= g.vertCount; vertex++)
			g.visited[vertex] = false;

		for (int vertex = 1; vertex <= g.leftPartCount; vertex++) {
			if(!g.matchOfL[vertex])
				changed |= findMatch(g, vertex);
		}
	}

	int maxMatching = 0;
	for (int vertex = 1; vertex <= g.leftPartCount; vertex++) {
		maxMatching += (g.matchOfL[vertex] != 0);
	}

	return maxMatching;

}



int main() {

	graph g;
	ifstream in("cuplaj.in");
	readGraph(g, in);
	in.close();

	int maxMatching = hopcroftKarp(g);

	ofstream out("cuplaj.out");
	out << maxMatching << "\n";
	for (int vertex = 1; vertex <= g.leftPartCount; vertex++)
		if (g.matchOfL[vertex])
			out << vertex << " " << g.matchOfL[vertex]-g.leftPartCount << "\n";
	out.close();
	


	return 0;
}