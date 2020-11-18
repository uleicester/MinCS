#include "ReadFSMs.h"

#include <iostream>
#include <vector>
#include <string>
#include <iostream>


using namespace std;
struct weight
{
	int vertex;
	string path;
	int distance;
	bool visited;
};
class myBFS
{
public:
	myBFS(FSM &graph, int initialVertex);
	vector<string> bfs(FSM &graph, int initialVertex);
private:
	void evaluateNextVertices(FSM &graph, int initialVertex);
	bool notAllVerticesReached;
	vector<weight> paths;
	vector<string> sP;
	int visitedCount;
};

myBFS::myBFS(FSM &graph, int initialVertex)
{
	visitedCount = 0;
	notAllVerticesReached = false;
	for(int i = 0; i < graph.getStates(); i++)
	{
		weight temp;		
		temp.path = "";
		temp.vertex = i;
		if(i!=initialVertex)
		{
			temp.visited = false;
			temp.distance = INT_MAX;
			
		}
		else{
			temp.visited = true;
			temp.distance = 0;
		}
		paths.push_back(temp);
	}
}
void myBFS::evaluateNextVertices(FSM &graph, int initialVertex)
{
	char buf[6];
	vector<weight> vertices;
	vertices.push_back(paths[initialVertex]);
	int pos = 0;
	while(vertices.size() < paths.size())
	{
		weight source = vertices[pos];
		int currentListNumber = visitedCount;
		for(int j = 0 ; j < graph.getInputs(); j++)
		{
			weight nextVertex = paths[graph.returnNextStateValue(source.vertex,j)];
			if(nextVertex.visited == false)
			{				
				visitedCount++;
				nextVertex.visited = true;
				nextVertex.distance = source.distance+1;
				nextVertex.path = source.path +itoa(j,buf,10);
				vertices.push_back(nextVertex);
				paths[graph.returnNextStateValue(source.vertex,j)]=nextVertex;
			}			
		}
		if(pos<vertices.size())
			pos++;
	}
	if(visitedCount=paths.size()){
		for(int i = 0 ; i < paths.size() ; i++)
		{
			sP.push_back(paths[i].path);
		}
		notAllVerticesReached = true;
	}
}
vector<string> myBFS::bfs(FSM &graph, int initialVertex)
{
	while(!notAllVerticesReached)
	{
		evaluateNextVertices(graph, initialVertex);
	}
	return sP;
}