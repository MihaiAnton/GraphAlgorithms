#include <fstream>
#include <queue>
#include <list>


using namespace std;

struct graph
{
	int vertexCount;
	int edgeCount;

	list<int>* adjList;
};

void readgraph(graph& g, ifstream& in)
{
	in >> g.vertexCount >> g.edgeCount;

	g.adjList = new list<int>[g.vertexCount + 1];

	for (int i = 1; i <= g.edgeCount; i++)
	{
		int a, b;
		in >> a >> b;

		g.adjList[a].push_back(b);
		g.adjList[b].push_back(a);
	}
}

bool bipartite(graph& g)
{
	//checks if the grapf is bipartite
	char* part = (char*)calloc((g.vertexCount + 1), sizeof(char));

	queue<pair<int,int>> q;

	bool bipart = true;

	q.push({ 1,1 });
	part[1] = 1;

	int crtPart = 1;
	int otherPart = 2;

	while (!q.empty() && bipart)
	{
		int vertex = q.front().first;
		crtPart = q.front().second;
		otherPart = (crtPart == 1) ? 2 : 1;
		q.pop();

		for (auto adj : g.adjList[vertex])
		{
			if (part[adj] == crtPart)
				bipart = false;

			if (part[adj] == 0)
			{
				part[adj] = otherPart;
				q.push({ adj, otherPart });
			}
		}
	}
	free(part);
	return bipart;
}

int main()
{
	graph g;

	ifstream in("graph.in");
	readgraph(g, in);
	in.close();

	ofstream out("graph.out");
	if (bipartite(g))
		out << "Graf bipartit.";
	else
		out << "Graful nu este bipartit.";
	out.close();

	delete[] g.adjList;

	return 0;
}