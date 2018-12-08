#include <fstream>
#include <list>
#include <vector>
#include <queue>

using namespace std;

struct graph
{
	int vertexCount;
	int edgeCount;
	list<pair<int/*adj id*/,int/*weight*/>>* adjList;
	vector<int> distances;	
};

void readGraph(graph& graph, ifstream& in)
{
	//reads the graph data
	in >> graph.vertexCount >> graph.edgeCount;
	graph.adjList = new list<pair<int, int>>[graph.vertexCount + 1];
	for (int i = 0; i <= graph.vertexCount; i++)
	{
		graph.distances.push_back(INT_MAX);
	}

	for (int i = 1; i <= graph.edgeCount; i++)
	{
		int a, b, weight;
		in >> a >> b >> weight;

		graph.adjList[a].push_back({ b, weight });
	}
}

bool relax(graph& graph, int from, int weight, int to)
{
	//tries to find a shortest distance from 'from' to 'to with the givven weight
	if ((long long)(graph.distances[from] + weight) < graph.distances[to])
	{
		graph.distances[to] = graph.distances[from] + weight;
		return true;
	}
	return false;
}

void dijkstra(graph& graph, int sourceNode)
{
	priority_queue<pair<int, int>> queue;				//the queue keeping the -distance and the nodes
	queue.push({ 0,sourceNode });
	graph.distances[sourceNode] = 0;

	while (!queue.empty())
	{
		int crtWeight = -queue.top().first;
		int crtNode = queue.top().second;
		queue.pop();

		for (auto elem : graph.adjList[crtNode])
		{
			bool relaxed = relax(graph, crtNode, elem.second, elem.first);
			if (relaxed)
			{
				queue.push({ -graph.distances[elem.first], elem.first });
			}
		}	
	}
}

void printDistances(graph& graph, ofstream& out)
{
	//prints the distances to the stream
	int id;
	for (id = 2; id <= graph.vertexCount; id++)
	{
		if (graph.distances[id] == INT_MAX)
			out << "0 ";
		else
			out << graph.distances[id] << " ";
	}
}

void graphDestroy(graph& graph)
{
	delete[]graph.adjList;
}






int main()
{
	graph graph;
	ifstream in("graph.in");
	readGraph(graph, in);
	in.close();

	dijkstra(graph, 1);

	ofstream out("graph.out");
	printDistances(graph, out);
	out.close();

	graphDestroy(graph);

	return 0;
}