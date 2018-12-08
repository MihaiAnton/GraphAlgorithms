#include <fstream>

using namespace std;

struct graph
{
	int n, m;
	int mat[100][100] = { 0 };
	int dist[100];
};


void readGraph(graph& g, ifstream& in)
{
	in >> g.n >> g.m;

	for (int i = 1; i <= g.m; i++)
	{
		int a, b, weight;
		in >> a >> b >> weight;
		g.mat[a][b] = weight;
	}
}

void BellmanKalaba(graph& g, int destNode)
{
	//runs the bellman kalaba algorithm and computes the minimum distance from all the nodes to one node

	int* prev = (int*)malloc((g.n + 1) * sizeof(int));
	int* crt = (int*)malloc((g.n + 1) * sizeof(int));

	for (int i = 1; i <= g.n; i++)
	{
		if (i == 1)								//construct the vector
		{
			for (int j = 1; j <= g.n; j++)
			{
				if (g.mat[j][destNode] > 0 || j == destNode)		//is edge
				{
					crt[j] = g.mat[j][destNode];
				}
				else
					crt[j] = INT_MAX;				
			}
		}

		else
		{
			for (int j = 1; j <= g.n; j++)		//for each vertex
			{
				for (int k = 1; k <= g.n; k++)
				{
					if (g.mat[j][k] && prev[k] != INT_MAX && (g.mat[j][k] + prev[k]) < crt[j])
						crt[j] = g.mat[j][k] + prev[k];
				}
			}
		}

		for (int j = 1; j <= g.n; j++)			//copy crt in prev
			prev[j] = crt[j];
	}

	for (int j = 1; j <= g.n; j++)			//copy crt in prev
		g.dist[j] = crt[j];
}

int main()
{


	graph g;

	ifstream in("graph.in");
	readGraph(g, in);
	in.close();


	BellmanKalaba(g, 7);


	ofstream out("graph.out");
	for (int i = 1; i <= g.n; i++)
		out << g.dist[i] << " ";
	out.close();


	return 0;
}