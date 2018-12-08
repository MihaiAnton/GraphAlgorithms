#include <fstream>
#include <list>
#include <vector>
#include <algorithm>

using namespace std;

struct vertex
{
	int id;
	int st;
	int ft;
	bool visited = false;

	bool operator<(vertex& ot) { return ft > ot.ft; }
	bool operator>(vertex& ot) { return ft < ot.ft; }
};


struct graph
{
	int vertexCount;
	int edgeCount;
	list<int>* adjList;
	vector<vertex> vertices;
};

void readGraph(graph& g, ifstream& in)
{
	in >> g.vertexCount >> g.edgeCount;

	g.adjList = new list<int>[g.vertexCount + 1];

	for (int i = 0; i <= g.vertexCount; i++)
	{
		g.vertices.push_back({ i,0,0 });
	}


	for (int i = 1; i <= g.edgeCount; i++)
	{
		int a, b;
		in >> a >> b;
		g.adjList[a].push_back(b);
	}
}


void runDFS(graph& g, int source, int& time)
{
	if (!g.vertices[source].visited)
	{
		g.vertices[source].visited = true;

		time++;
		g.vertices[source].st = time;

		for (auto elem : g.adjList[source])
		{
			runDFS(g, elem, time);
		}

		time++;
		g.vertices[source].ft = time;
	}
}




int main()
{
	graph g;

	ifstream in("graph.in");
	readGraph(g, in);
	in.close();

	int time = 0;
	runDFS(g, 1, time);

	ofstream out("graph.out");
	sort(g.vertices.begin()+1, g.vertices.end());
	for (int i = 1; i <= g.vertexCount; i++)
	{
		out << g.vertices[i].id << " ";
	}
	out.close();


	delete[] g.adjList;
	return 0;
}