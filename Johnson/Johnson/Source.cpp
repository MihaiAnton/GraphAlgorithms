#include <fstream>
#include <list>
#include <vector>
#include <queue>

using namespace std;

struct graph
{
	int vertexCount;
	int edgeCount;
	list<pair<int/*adj id*/, int/*weight*/>>* adjList;
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

void initDistances(graph& graph, int sourceNode)
{
	int i = 1;
	for (i = 1; i <= graph.vertexCount; i++)
	{
		graph.distances[i] = INT_MAX;
	}
	graph.distances[sourceNode] = 0;
}


bool bellmanFord(graph& graph, int sourceNode)
{
	//runs the bellman ford algorithm from the source node
	//returns true if it contains negative cycles, false otherwise

	initDistances(graph, sourceNode);

	for (int i = 0; i <= graph.vertexCount; i++)			//for vertices times
	{
		int j;
		for (j = 0; j <= graph.vertexCount; j++)
		{
			for (auto pair : graph.adjList[j])
			{
				bool relaxed = relax(graph, j, pair.second, pair.first);
			}
		}
	}

	for (int j = 1; j <= graph.vertexCount; j++)
	{
		for (auto pair : graph.adjList[j])
		{
			bool relaxed = relax(graph, j, pair.second, pair.first);
			if (relaxed == true)
				return true;
		}
	}
	return false;
}

void dijkstra(graph& graph, int sourceNode)
{
	//runs dijkstra from sourcenode to find every minimum distance
	priority_queue<pair<int, int>> queue;

	initDistances(graph, sourceNode);
	queue.push({ 0,sourceNode });

	while (!queue.empty())
	{
		int dist = -queue.top().first;
		int crtId = queue.top().second;
		queue.pop();

		for (auto pair : graph.adjList[crtId])
		{
			bool relaxed = relax(graph, crtId, pair.second, pair.first);
			if(relaxed)
				queue.push({ -graph.distances[pair.first], pair.first });
		}
	}
}

void reponder(graph& graph)
{
	//reponders each edge; supposes a run of bellmanFord on the graph before this
	for (int j = 1; j <= graph.vertexCount; j++)
	{
		for (auto& pair : graph.adjList[j])
		{
			pair.second = pair.second + graph.distances[j] - graph.distances[pair.first];
		}
	}
}

bool johnson(graph& graph, ofstream& out)
{
	//runs the johnson algorithm from the source node and outputs the result in the stream
	//returns rue if the graph has negative cycles, false otherwise
	for (int i = 1; i <= graph.vertexCount; i++)
	{													//
		graph.adjList[0].push_back({ i,0 });			//	add a 0 distanced vertex connected to all the vertices
	}													//
	
	if (bellmanFord(graph, 0) == true)					//run bellman fors to detect the minimum paths and to detect negative cycles
	{
		out << "Cicluri negative!";
		return true;
	}
	else												//no negative cycles found
	{
		int i;
		int* distances = (int*)malloc((graph.vertexCount + 1) * sizeof(int));	//vector to memorize distances before reponder
		for (i = 1; i <= graph.vertexCount; i++) 
			distances[i] = graph.distances[i];									//copy the distances

		reponder(graph);								//reponder the edges
		graph.adjList[0].clear();						//remove the added vertex

		for (i = 1; i <= graph.vertexCount; i++)		//for each possible source node
		{
			dijkstra(graph, i);							//run dijkstra

			int j;
			for (j = 1; j <= graph.vertexCount; j++)	//print the distances
			{
				int dist;
				if (graph.distances[j] == INT_MAX)
					dist = -1;
				else
					dist = graph.distances[j] - distances[i] + distances[j];
				out << dist << " ";
			}
			out << "\n";
		}
		free(distances);
	}
	return false;
}

int main()
{
	graph graph;

	ifstream in("graph.in");
	readGraph(graph, in);
	in.close();

	ofstream out("graph.out");
	bool result = johnson(graph, out);
	
	out.close();

	return 0;
}