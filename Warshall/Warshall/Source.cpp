#include <fstream>

using namespace std;


struct graph
{
	int n, m;
	int mat[100][100] = { 0 };
};


void readGraph(graph& g, ifstream& in)
{
	in >> g.n >> g.m;
	for (int i = 1; i <= g.n; i++)
	{
		int a, b, w;
		in >> a >> b >> w;
		g.mat[a][b] = w;
	}
}

void warshall(graph& g)
{
	for (int i = 1; i <= g.n; i++)
	{
		for (int j = 1; j <= g.n; j++)
			if (!g.mat[i][j] && i != j)				//initializare matrice
				g.mat[i][j] = INT_MAX;
	}

	for (int k = 1; k <= g.n; k++)
		for (int i = 1; i <= g.n; i++)
			for (int j = 1; j <= g.n; j++)
			{
				long long bigSum = (g.mat[i][k] + g.mat[k][j]);
				if (bigSum < g.mat[i][j] && g.mat[i][k] != INT_MAX && g.mat[k][j] != INT_MAX)
					g.mat[i][j] = g.mat[i][k] + g.mat[k][j];
			}

}

void printDistances(graph& g, ofstream& out)
{
	for (int i = 1; i <= g.n; i++)
	{
		for (int j = 1; j <= g.n; j++)
		{
			int val = g.mat[i][j];
			if (val == INT_MAX)
				val = -1;
			out << val << " ";
		}
		out << "\n";
	}
}



int main()
{
	graph g;

	ifstream in("graph.in");
	readGraph(g, in);
	in.close();

	warshall(g);

	ofstream out("graph.out");
	printDistances(g, out);
	out.close();

	return 0;
}