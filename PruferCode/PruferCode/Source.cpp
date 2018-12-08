#include <fstream>
#include <list>

using namespace std;

struct graph
{
	int vertexCount;
	int edgeCount;

	int* grades;
	list<int>* adjList;
};

void readGraph(graph& graph, ifstream& in)
{
	in >> graph.vertexCount >> graph.edgeCount;
	graph.adjList = new list<int>[graph.vertexCount + 1];
	graph.grades = (int*)calloc(graph.vertexCount + 1, sizeof(int));

	for (int i = 1; i <= graph.edgeCount; i++)
	{
		int a, b;
		in >> a >> b;
		graph.adjList[a].push_back(b);
		graph.adjList[b].push_back(a);
		graph.grades[a]++;
		graph.grades[b]++;
	}
}


int getMinLeaf(graph& graph)
{
	for (int i = 1; i <= graph.vertexCount; i++)
	{
		if (graph.grades[i] == 1)
			return i;
	}
}

void deleteLeaf(graph& graph, int leafId)
{
	for (auto elem : graph.adjList[leafId])
	{
		graph.grades[elem]--;
		graph.adjList[elem].remove(leafId);
	}

	graph.grades[leafId]--;
}

list<int> pruferCode(graph& graph)
{
	//returns a list of ints representing the prufer code
	list<int> code;

	for (int i = 1; i <= graph.vertexCount - 2; i++)
	{
		int vertexId = getMinLeaf(graph);
		int parent = *graph.adjList[vertexId].begin();
	
		deleteLeaf(graph, vertexId);

		code.push_back(parent);
	}

	return code;
}

int  getLeafFromPrufer(list<int>& code, list<int>::iterator it, int maxPos)
{
	bool* appears = (bool*)calloc(maxPos+1, sizeof(bool));
	while (it != code.end())
	{
		appears[*it] = true;
		it++;
	}
	for (int i = 1; i <= maxPos; i++)
		if (!appears[i])
			return i;
}

graph graphFromPrufer(list<int> code)
{
	int codeSize = code.size();

	graph g;
	g.adjList = new list<int>[codeSize + 3];
	g.edgeCount = 0;
	g.vertexCount = codeSize + 2;

	list<int>::iterator it = code.begin();

	for (int i = 1; i <= codeSize; i++)
	{
		int parent = *it;
		
		int leaf = getLeafFromPrufer(code,it,g.vertexCount);
		code.push_back(leaf);
		g.adjList[parent].push_back(leaf);
		g.adjList[leaf].push_back(parent);		

		it++;
	}


	int leaf1 = getLeafFromPrufer(code, it, g.vertexCount);
	code.push_back(leaf1);
	int leaf2 = getLeafFromPrufer(code, it, g.vertexCount);

	g.adjList[leaf1].push_back(leaf2);
	g.adjList[leaf2].push_back(leaf1);
	return g;
}


int main()
{
	graph g;
	
	ifstream in("graph.in");
	readGraph(g, in);
	in.close();

	list<int> code = pruferCode(g);

	ofstream out("graph.out");
	for (auto elem : code)
	{
		out << elem << " ";
	}
	
	out << "\n";

	graph g2;

	g2 = graphFromPrufer(code);

	for (int i = 1; i <= g2.vertexCount; i++)
	{
		for (auto elem : g2.adjList[i])
		{
			if (i < elem)
				out << i << " " << elem << "\n";
		}
	}

	out.close();
	return 0;
}