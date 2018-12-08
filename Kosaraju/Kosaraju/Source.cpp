#include <fstream>
#include <list>
#include <vector>

using namespace std;

struct graph
{
	int vertexCount, edgeCount;
	int connexComponents;

	list<int>* adjList;
	vector<int> compId;
	vector<bool> visited;
	


};


void readGraph(graph& graph, ifstream& in)
{
	in >> graph.vertexCount >> graph.edgeCount;				//read the data
	graph.adjList = new list<int>[graph.vertexCount + 1];	//create a lost of exact size
	graph.connexComponents = 0;

	for (int i = 0; i <= graph.vertexCount; i++)
	{
		graph.visited.push_back(false);
		graph.compId.push_back(0);
	}

	for (int i = 1; i <= graph.edgeCount; i++)			//for each edge
	{
		int a, b;
		in >> a >> b;										//read the edges
		graph.adjList[a].push_back(b);						//add each edge
	}
}

void runDFS(graph& graph, int source, list<int>& stack)
{
	//runs dfs and puts the nodes in a stack
	if (!graph.visited[source])							//if not visited
	{
								//put in on the stack
		graph.visited[source] = true;					//mark as visited

		for (auto adjVertex : graph.adjList[source])	//for each adjVertex
		{
			if (!graph.visited[adjVertex])				//if not visited
			{
				runDFS(graph, adjVertex, stack);		//continue recursively
			}
		}
		stack.push_back(source);
	}
}

void runDFSWMark(graph& graph, int source, int compId)
{
	//runs dfs from the givven node and mars all the reached vertices with the givven compId
	if (graph.compId[source] == 0)						//not yet marked
	{
		graph.compId[source] = compId;					//mark

		for (auto adjVertex : graph.adjList[source])	//for each adj vertex
		{
			runDFSWMark(graph, adjVertex, compId);		//continue recursively
		}
	}
}


void  getInverted(graph& g)
{
	graph invGraph;
	invGraph.connexComponents = 0;
	invGraph.edgeCount = g.edgeCount;
	invGraph.vertexCount = g.vertexCount;

	invGraph.adjList = new list<int>[invGraph.vertexCount + 1];	//create a lost of exact size

	for (int i = 0; i <= invGraph.vertexCount; i++)
	{
		invGraph.visited.push_back(false);
		invGraph.compId.push_back(0);
	}

	int i;
	for (i = 1; i <= g.vertexCount; i++)
	{
		for (auto adjVertex : g.adjList[i])
			invGraph.adjList[adjVertex].push_back(i);
	}
	g = invGraph;
}


void kosaraju(graph& g)
{
	//runs kosaraju and assigns to each node a connex component id
	list<int> stack;
	
	runDFS(g, 1, stack);

	getInverted(g);
	while (!stack.empty())
	{
		int vertex = stack.back();
		stack.pop_back();

		if (g.compId[vertex] == 0)			//not assigned a component
		{
			g.connexComponents++;			//set a new component
			runDFSWMark(g, vertex, g.connexComponents);	//mark the DFS tree starting at vertex
		}
	}
	
}


void printConnexComponents(graph& graph, ofstream& out)
{
	//outputs the elements of each connex component on separate line
	int id;
	for (id = 1; id <= graph.connexComponents; id++)
	{
		int vertex;
		for (vertex = 1; vertex <= graph.vertexCount; vertex++)
		{
			if (graph.compId[vertex] == id)
			{
				out << vertex << " ";
			}
		}
		out << "\n";
	}
}


void graphDestroy(graph& graph)
{
	delete[] graph.adjList;
}




int main()
{
	graph graph;

	ifstream in("graph.in");
	readGraph(graph, in);
	in.close();


	kosaraju(graph);

	ofstream out("graph.out");
	printConnexComponents(graph, out);
	out.close();


	graphDestroy(graph);
	return 0;
}