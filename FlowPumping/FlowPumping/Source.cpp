#include <fstream>
#include <list>
#include <vector>
#include <stdlib.h>
#include <stdio.h>

using namespace std;

#define INF 0x7fffffff

struct graph {
	int vertexCount;
	int edgeCount;

	list<int>* adjList;

	vector<int> height;
	vector<int> exces;
	int** flow;
};

void readGraph(graph& g, ifstream& in) {
	in >> g.vertexCount >> g.edgeCount;
	g.adjList = new list<int>[g.vertexCount + 1];

	g.flow = new int*[g.vertexCount + 1];
	for (int i = 1; i <= g.vertexCount; i++) {
		g.flow[i] = new int[g.vertexCount + 1];
		for (int j = 1; j <= g.vertexCount; j++)
			g.flow[i][j] = 0;
	}

	for (int i = 1; i <= g.edgeCount; i++) {
		int source, sink, flow;
		in >> source >> sink >> flow;

		g.adjList[source].push_back(sink);
		g.adjList[sink].push_back(source);

		g.flow[source][sink] = flow;
	}

	g.height = vector<int>(g.vertexCount + 1);
	g.exces = vector<int>(g.vertexCount + 1);
}

void initPreflow(graph& g, int source) {

	for (int i = 1; i <= g.vertexCount; i++) {
		g.height[i] = 0;
		g.exces[i] = 0;
	}
	g.height[source] = g.vertexCount;

	for (auto adj : g.adjList[source]) {
		g.exces[adj] += g.flow[source][adj];
		g.flow[adj][source] += g.flow[source][adj];
		g.flow[source][adj] = 0;
	}
}

bool findPumpVertex(graph& g, int source, int sink, int& from, int& to) {

	for(int i=1;i<=g.vertexCount;i++)
		if (i != source && i != sink && g.exces[i] > 0) {
			for (auto adj : g.adjList[i])
				if (g.flow[i][adj] > 0 && g.height[i] == (g.height[adj] + 1)) {
					from = i;
					to = adj;
					return true;
				}
		}
	return false;	
}

void pump(graph& g, int source, int sink) {

	int pumpQuantity = g.exces[source] < g.flow[source][sink]? g.exces[source] : g.flow[source][sink];
	g.exces[source] -= pumpQuantity;			//remove the exces from source
	g.exces[sink] += pumpQuantity;				//add exces to sink

	g.flow[source][sink] -= pumpQuantity;		//lower the flow
	g.flow[sink][source] += pumpQuantity;		//raise the received flow
}

bool findRaiseVertex(graph& g, int source, int sink, int& raisedVert, int& raise) {

	for (int i = 1; i <= g.vertexCount; i++) 
		if (i != source && i != sink && g.exces[i] > 0) {
			raise = 0;
			
			int lowestAdj = INF;						//determining the lowest adj
			for (auto adj : g.adjList[i]) {
				if (g.flow[i][adj] > 0 && g.height[adj] < lowestAdj)	//if can pump flow and has the lowest height so far
					lowestAdj = g.height[adj];							//update the height
			}

			if (lowestAdj == INF)	//couldn't find a suitable adjacent vertex
				continue;

			//here we reached a suitable vertex
			raisedVert = i;
			raise = lowestAdj - g.height[i] + 1;
			return true;
		}
	return false;	

}

void raiseVertex(graph& g, int vertex, int raise) {

	g.height[vertex] += raise;	//higher the vertex
}

void flowPumping(graph& g, int source, int sink) {

	initPreflow(g, source);
	int pumpVertex, sinkVertex, raisedVertex, raise;

	while (true) {

		if (findPumpVertex(g, source, sink, pumpVertex, sinkVertex)) {
			pump(g, pumpVertex, sinkVertex);
			continue;
		}
		if (findRaiseVertex(g, source, sink, raisedVertex, raise)) {
			raiseVertex(g, raisedVertex, raise);
			continue;
		}

		break;
	}
}

int getMaxFlow(graph& g, int source, int sink) {

	int maxFlow = 0;
	flowPumping(g, source, sink);

	for (auto adj : g.adjList[sink]) {
		maxFlow += g.flow[sink][adj];
	}

	return maxFlow;
}

int main() {

	graph g;
	

	ifstream in("maxflow.in");
	readGraph(g, in);
	in.close();

	int source = 1;
	int sink = g.vertexCount;
	int maxFlow = getMaxFlow(g, source, sink);

	ofstream out("maxflow.out");
	out << maxFlow;
	out.close();

	return 0;
}