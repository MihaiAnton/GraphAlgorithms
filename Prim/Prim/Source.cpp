#include <fstream>
#include <list>
#include <queue>

using namespace std;

struct graph
{
	int n, m;
	int adjMat[2000][2000] = { 0 };
	int key[200];
	bool inMST[200];
	int parent[200];
};

void readGraph(graph& g, ifstream& in)
{
	in >> g.n >> g.m;
	for (int i = 1; i <= g.m; i++)
	{
		int a, b, w;
		in >> a >> b >> w;

		g.adjMat[a][b] = w;
		g.adjMat[b][a] = w;
	}
}


list<pair<int,int>> Prim(graph& g)
{
	//runs prim's algorithm on the graph
	int i;
	queue<pair<int, int>> queue;
	list<pair<int, int>> tree;

	for (i = 1; i <= g.n; i++)
	{
		g.key[i] = INT_MAX;
		g.inMST[i] = false;
	}
	g.key[1] = 0;
	g.parent[1] = -1;

	for (int i = 1; i <= g.n - 1; i++)
	{
		int keyMin = INT_MAX, idMin;
		for (int i = 1; i <= g.n; i++)
		{
			if (g.key[i] < keyMin && !g.inMST[i])
				keyMin = g.key[i], idMin = i;
		}

		g.inMST[idMin] = true;

		for (int i = 1; i <= g.n; i++)
			if (g.adjMat[idMin][i] && !g.inMST[i] && g.adjMat[idMin][i] < g.key[i])
			{
				g.parent[i] = idMin; g.key[i] = g.adjMat[idMin][i];
			}
	}

	for (int i = 2; i <= g.n; i++)
	{
		tree.push_back({ i, g.parent[i] });
	}

	return tree;
}














int main()
{

	graph* g = new graph;
	ifstream in("graph.in");
	readGraph(*g, in);
	in.close();

	list<pair<int, int>> tree = Prim(*g);

	ofstream out("graph.out");
	for (auto pair : tree)
	{
		out << pair.first << " " << pair.second << "\n";
	}
	out.close();

	return 0;
}




