#include <fstream>
#include <list>
#include <vector>

using namespace std;


struct graph
{
	int vertexCount;
	int edgeCount;
	list<pair<int, int>>* adjList;
	vector<bool> visited;
};



void readGraph(graph& graph, ifstream& in)
{
	//reads the graph
	in >> graph.vertexCount >> graph.edgeCount;

	graph.adjList = new list<pair<int, int>>[graph.vertexCount + 1];
	graph.visited.push_back(true);
	for (int i = 1; i <= graph.edgeCount; i++)
	{
		int a, b;
		in >> a >> b;

		graph.visited.push_back(false);

		graph.adjList[a].push_back({ b,i });
		graph.adjList[b].push_back({ a,i });
	}
}


bool notEulerian(graph& graph)
{
	for (int i = 1; i <= graph.vertexCount; i++)
	{
		if (graph.adjList[i].size() % 2 == 1)			//condition not to be eulerian
			return true;
	}
	return false;
}

void eulerianCycle(graph& graph, list<int>& vertices, int source)
{
	while (graph.adjList[source].size() > 0)
	{
		pair<int, int> edge = graph.adjList[source].back();
		int vert = edge.first;
		int id = edge.second;

		if (!graph.visited[id])
		{
			graph.visited[id] = true;
			

			eulerianCycle(graph, vertices, vert);
		}
		else
			graph.adjList[source].pop_back();
	}
	vertices.push_back(source);
}



int main()
{

	graph graph;

	ifstream in("graph.in");
	readGraph(graph, in);
	in.close();
	
	ofstream out("graph.out");

	if (notEulerian(graph))
	{
		out << "Nu contine un ciclu eulerian.";
	}
	else
	{
		list<int> cycle;
		eulerianCycle(graph, cycle, 1);

		cycle.pop_back();

		for (auto elem : cycle)
		{
			out << elem << " ";
		}
	}
	out.close();
	
	return 0;
}