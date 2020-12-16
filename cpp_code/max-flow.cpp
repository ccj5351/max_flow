/**
 * @file: max-flow.cpp
 * @brief:
 * @author: Changjiang Cai, caicj5351@gmail.com
 * @version: 0.0.1
 * @creation date: 11-12-2015
 * @last modified: Mon 14 Dec 2015 10:36:23 AM EST
 */

#include "max-flow.h"
// constructor
max_flow::max_flow(int s, int t, bool isDis){
	vertexNum = V;
	source = s;
	sink = t;
	isDisplay = isDis;
}
// do initialization
void max_flow::initialize(){
	memset(discoverTime, 0, sizeof(discoverTime));
	memset(finishTime, 0, sizeof(finishTime));
	memset(parents, -1, sizeof(parents));
	dfs_time = 0;
	for (int i = 0; i < V; ++i)
	    vColors[i] = WHITE;
}

/* Returns true if there is a path from source 's' to sink 't' in
  residual graph. Also fills parent[] to store the path */
// arguments:
// rGraph: residual graph;
// s: source;
// t: sink;
// breadth-first-search algorithm
bool max_flow::runBFS(int rGraph[V][V]){
	// Create a visited array and mark all vertices as not visited
	bool IsVisited[V];
	memset(IsVisited, 0, sizeof(IsVisited));
	parents[source] = -1;
	// Create a queue, enqueue source vertex and 
	// mark source vertex as visited.
	// queue : FIFO, first in first out;
	queue <int> q;
	q.push(source);
	IsVisited[source] = true;
	if (isDisplay)
		std::cout << "\n\nBFS traversal starts : \n";
	int counter = 1;
	// Standard BFS Loop
	while (!q.empty()){

		int u = q.front();
		// removes the next element in the queue, 
		// effectively reducing its size by one.
		// The element removed is the "oldest" element in the queue whose value 
		// can be retrieved by calling member queue::front. 
		q.pop();
		if (isDisplay){
			std::cout << " \nlevel " << counter <<
				" starts from vertex " << u + 1 << "\n";
		}
		for (int v = 0; v < V; v++){
			if (IsVisited[v] == false && rGraph[u][v] > 0)
			{
				q.push(v);
				if (isDisplay){
					std::cout << "  " << u + 1 << " -- (capacity = "
						<< rGraph[u][v]
						<< ") --> " << v + 1 << endl;
				}
				parents[v] = u;
				IsVisited[v] = true;
			}
		}
		counter++;
	}
	string BFS = "BFS";
	printPath( BFS, source, sink);
	// If we reached sink in BFS starting from source, 
	// then return true, else return false;
	return (IsVisited[sink] == true);
}



/* Returns true if there is a path from source 's' to sink 't' in
residual graph. Also fills parent[] to store the path */
// arguments:
// rGraph: residual graph;
// s: source;
// t: sink;
// parent: recording the path;
// isDisplay : flag for printing the path or not. 
// depth-first-search algorithm
// algorithm :
// Initially all vertices are white (unvisited). 
// DFS starts in arbitrary vertex and runs as follows:
// 1) Mark vertex u as gray(visited).
// 2) For each edge(u, v), where u is white, 
//    run depth - first search for u recursively.
// 3) Mark vertex u as black and backtrack to the parent.
bool max_flow::DFS(int rGraph[V][V]){
	initialize();
	string DFS = "DFS";
	// If we reached sink in BFS starting from source, 
	// then return true, else return false;
	runDFS(rGraph, source);
	// If we reached sink in BFS starting from source, 
	// then return true, else return false;
	if (printPath(DFS, source, sink))
		return true;
	else 
		return false;
}

void max_flow::runDFS(int rGraph[V][V], int u){
	dfs_time++;
	discoverTime[u] = dfs_time;
	vColors[u] = GRAY;
	for (int v = 0; v < vertexNum; v++){
		if (vColors[v] == WHITE && rGraph[u][v] > 0){
			if (isDisplay){
				std::cout << "  " << u + 1 << " -- (capacity = "
					<< rGraph[u][v]
					<< ") --> " << v + 1 << endl;
			}
			parents[v] = u;
			// If we reached sink in DFS starting from source, 
			// then return, else keep doing dfs traversal;
			if (vColors[sink] != BLACK){
			   runDFS(rGraph, v);
			}
			else {
				std::cout << " already arrived at sink, stop the search.\n";
				return;
			}
		}
		else if (vColors[v] != WHITE && rGraph[u][v] > 0){
			if (isDisplay){
				std::cout << "  " << u + 1 << " -- (capacity = "
					<< rGraph[u][v]
					<< ") --> " << v + 1 << " -- backtracking to --> "
					<< v + 1 << ", due to " << v+ 1 <<
					" has already been visited before." << endl;
			}
		}

	}
	vColors[u] = BLACK;
}



// Returns the maximum flow from s to t in the given graph
int max_flow::fordFulkerson_BFS_RG(int graph[V][V], int & AugmentingNum){

	int u, v;
	// Create a residual graph and fill the residual graph with
	// given capacities in the original graph as residual capacities
	// in residual graph
	int rGraph[V][V]; // Residual graph where rGraph[i][j] indicates 
	// residual capacity of edge from i to j (if there
	// is an edge. If rGraph[i][j] is 0, then there is not)  
	for (u = 0; u < V; u++)
		for (v = 0; v < V; v++)
			rGraph[u][v] = graph[u][v];

	initialize();
	int max_flow = 0;  // There is no flow initially
	AugmentingNum = 1;
	// Augment the flow while there is augmenting path from source to sink;
	while (runBFS(rGraph)){
		// Find bottleneck edge, which has the minimum residual capacity 
		// of the edges along the augmenting path filled by BFS. 
		// That is residual capacity of this augmenting path.

		// path flow, which will be augmented until 
		// no augmenting path can
		// be found in the residual capacity graph.
		int path_flow = INT_MAX;
		for (v = sink; v != source; v = parents[v]){
			u = parents[v];
			path_flow = min(path_flow, rGraph[u][v]);
		}

		// update residual capacities of the forward edges 
		// and backward edges
		// along the path;
		for (v = sink; v != source; v = parents[v]){
			u = parents[v];
			// residual capacity of forward edges;
			rGraph[u][v] -= path_flow;
			// residual capacity of backward edges;
			rGraph[v][u] += path_flow;
		}

		// Add path flow to overall flow
		std::cout << "  and its residual capacity is " << path_flow 
			<< endl << endl;
		max_flow += path_flow;
		AugmentingNum++;
		
		if (isDisplay){
			std::cout << "fordFulkerson_BFS_RG: \n";
			for (int i = 0; i < V; ++i){
				for (int j = 0; j < V; ++j)
					cout << rGraph[i][j] << " ";
				std::cout << endl;
			}
		}
	}

	// Return the overall flow
	std::cout << AugmentingNum << " augmenting paths have been executed.\n";
	std::cout << "The max flow is "  << max_flow << "."<< endl;

	return max_flow;

}

// Returns the maximum flow from s to t in the given graph
int max_flow::fordFulkerson_DFS_RG(int graph[V][V], int & AugmentingNum){

	int u, v;
	// Create a residual graph and fill the residual graph with
	// given capacities in the original graph as residual capacities
	// in residual graph
	int rGraph[V][V]; // Residual graph where rGraph[i][j] indicates 
	// residual capacity of edge from i to j (if there
	// is an edge. If rGraph[i][j] is 0, then there is not)  
	for (u = 0; u < V; u++)
		for (v = 0; v < V; v++)
			rGraph[u][v] = graph[u][v];
	int max_flow = 0;  // There is no flow initially
	AugmentingNum = 1;
	// Augment the flow while there is augmenting path from source to sink;
	while (DFS(rGraph)){
		// Find bottleneck edge, which has the minimum residual capacity 
		// of the edges along the augmenting path filled by BFS. 
		// That is residual capacity of this augmenting path.

		// path flow, which will be augmented until 
		// no augmenting path can
		// be found in the residual capacity graph.
		int path_flow = INT_MAX;
		for (v = sink; v != source; v = parents[v]){
			u = parents[v];
			path_flow = min(path_flow, rGraph[u][v]);
		}

		// update residual capacities of the forward edges 
		// and backward edges
		// along the path;
		for (v = sink; v != source; v = parents[v]){
			u = parents[v];
			// residual capacity of forward edges;
			rGraph[u][v] -= path_flow;
			// residual capacity of backward edges;
			rGraph[v][u] += path_flow;
		}
		// Add path flow to overall flow
		std::cout << "  and its residual capacity is " << path_flow 
			<< endl << endl;
		max_flow += path_flow;
		AugmentingNum++;
		initialize();

		if (isDisplay){
			std::cout << "fordFulkerson_DFS_RG: \n";
			for (int i = 0; i < V; ++i){
				for (int j = 0; j < V; ++j)
					cout << rGraph[i][j] << " ";
				std::cout << endl;
			}
		}
	}

	// Return the overall flow
	std::cout << AugmentingNum << " augmenting paths have been executed.\n";
	std::cout << "The max flow is " << max_flow << "." << endl;
	return max_flow;

}

// specialized BFS algorithm
bool max_flow::runSBFS(int flowGraph[V][V], int capacityGraph[V][V]
	// , int & pathCapacity
	){
	
	// Create a visited array and mark all vertices as not visited
	bool IsVisited[V];
	memset(IsVisited, 0, sizeof(IsVisited));
	parents[source] = -1;
	// Create a queue, enqueue source vertex and 
	// mark source vertex as visited.
	// queue : FIFO, first in first out;
	queue <int> q;
	q.push(source);
	IsVisited[source] = true;
	if (isDisplay)
		std::cout << "\n\nSBFS traversal starts : \n";
	int counter = 1;
	// Standard BFS Loop
	while (!q.empty()){
		int u = q.front();
		// removes the next element in the queue, 
		// effectively reducing its size by one.
		// The element removed is the "oldest" element in the queue whose value 
		// can be retrieved by calling member queue::front. 
		q.pop();
		
		if (isDisplay){
			std::cout << " \nlevel " << counter <<
				" starts from vertex " << u + 1 << "\n";
		}
		for (int v = 0; v < V; v++){
			if (IsVisited[v] == false && IsVisited[sink] == false){
				if (flowGraph[u][v] < capacityGraph[u][v]){
					q.push(v);
					if (isDisplay){
						std::cout << "  " << u + 1 << " -- "
							<< flowGraph[u][v]
							<< "/" << capacityGraph[u][v] << " --> " << v + 1 << endl;
					}
					parents[v] = u;
					IsVisited[v] = true;
				}
				else if (flowGraph[v][u] > 0){
					q.push(v);
					if (isDisplay){
						std::cout << "  " << u + 1 << " -- "
							<< flowGraph[u][v]
							<< "/" << capacityGraph[v][u] << " --> " << v + 1 << endl;
					}
					parents[v] = u;
					IsVisited[v] = true;
				}
				else {}
			}
		}
		counter++;
	}
	string SBFS = "SBFS";
	savePrintPath(SBFS, source, sink);
	// If we reached sink in BFS starting from source, 
	// then return true, else return false;
	return (IsVisited[sink] == true);
}

// here we use a specialization of BFS searches for 
// an augmenting path in the network flow G,
// instead of its residual capacity.
// we consider only the following edges in G;
//  * Outgoing edges of v with flow less than the capacity;
//  * Incoming edges of v with nonzero flow.
int max_flow::fordFulkerson_BFS_G(int capacityGraph[V][V], int & AugmentingNum){
	int u, v;
	// Create a flow graph and fill the flow graph with 0;
	int flowGraph[V][V], rGraph[V][V];
	for (u = 0; u < V; u++)
		for (v = 0; v < V; v++)
			flowGraph[u][v] = 0;
	initialize();
	int max_flow = 0;  // There is no flow initially
	AugmentingNum = 1;
	int pathResidualCapacity = INT_MAX;

	// Augment the flow while there is augmenting path from source to sink;
	while (runSBFS(flowGraph, capacityGraph)){
		// calculating the residual graph;
		// 1) cf(u, v) = c(u,v) - f(u, v) , if (u,v) is in E;
		// 2) cf(u, v) = f(v, u) , if (v,u) is in E;1) 
		// 3) cf(u, v) = 0, otherwise;
		for (u = 0; u < V; u++)
			for (v = 0; v < V; v++){
				// 1) cf(u, v) = c(u,v) - f(u, v) , if (u,v) is in E;
				if (capacityGraph[u][v] != 0)
					rGraph[u][v] = capacityGraph[u][v] - flowGraph[u][v];
				// 2) cf(u, v) = f(v, u) , if (v,u) is in E;1) 
				else if (capacityGraph[v][u] != 0)
					rGraph[u][v] = flowGraph[v][u];
				// 3) cf(u, v) = 0, otherwise;
				else
					rGraph[u][v] = 0;
			}
		// calculating the residual capacity of the augmenting path;
		for (int i = 0, size = augmentpath.size(); i < size - 1; ++i){
			int child = augmentpath[i + 1];
			int ancestor = augmentpath[i];
			pathResidualCapacity = min(pathResidualCapacity, rGraph[ancestor][child]);
		}
		// push path residual Capacity units of flow
		// along the path;
		for(int i = 0, size = augmentpath.size();i < size -1 ; ++ i){
			int child = augmentpath[i + 1];
			int ancestor = augmentpath[i];
			// * for each edge (u, v) in the augmenting path;
			// * if (u, v) is in E, then (u, v).f += cf(p);
			// * else, (v, u).f -= cf(p);

			// residual capacity of forward edges;
			if (capacityGraph[ancestor][child] != 0)
				flowGraph[ancestor][child] += pathResidualCapacity;
			else
				// residual capacity of backward edges;
				flowGraph[child][ancestor] -= pathResidualCapacity;
		}

		// Add path flow to overall flow
		std::cout << "  and its residual capacity is " << pathResidualCapacity
			<< endl << endl;
		max_flow += pathResidualCapacity;
		AugmentingNum++;
		// clear the path for the next loop
		augmentpath.clear();

		if (isDisplay){
			std::cout << "fordFulkerson_BFS_G: flow/residualCapacity/Capacity\n";
			for (int i = 0; i < V; ++i){
				for (int j = 0; j < V; ++j)
					cout << flowGraph[i][j] << "/" << rGraph[i][j] << "/" << capacityGraph[i][j] << "  ";
				std::cout << endl;
			}
		}
	}

	// Return the overall flow
	std::cout << AugmentingNum << " augmenting paths have been executed.\n";
	std::cout << "The max flow is " << max_flow << "." << endl;
	return max_flow;
}

// Driver program to test above functions
int main()
{
	// Let us create a graph shown in the above example
	int graph[V][V] = { 
	{ 0, 6, 3, 5, 0, 0 },
	{ 0, 0, 1, 0, 0, 3 },
	{ 0, 0, 0, 0, 9, 7 },
	{ 0, 0, 1, 0, 2, 0 },
	{ 0, 0, 0, 0, 0, 5 },
	{ 0, 0, 0, 0, 0, 0 }
	};
	bool isDisplay = true;
	int s = 0, t = V - 1;
	max_flow mf(s, t, isDisplay);
	mf.initialize();
	// method 1 for finding augmenting paths;
	std::cout << "*******************************************************" 
		<< endl;
	std::cout << "\nMethod 1 begins:" << endl;
	int AugmentingNum1 = 0, AugmentingNum2 = 0,
		AugmentingNum3 = 0;
	int max_flow1 = mf.fordFulkerson_BFS_RG(graph, AugmentingNum1);
	std::cout << "\nMethod 1 : The maximum possible flow is " 
		<< max_flow1 << endl << endl;

	// method 2 for finding augmenting paths;
	std::cout << "*******************************************************"	
		<< endl;
	std::cout << "\nMethod 2 begins:" << endl;
	int max_flow2 = mf.fordFulkerson_DFS_RG(graph, AugmentingNum2);
	std::cout << "\nMethod 2 :The maximum possible flow is " 
		<< max_flow2 << endl << endl;

	// method 3 for finding augmenting paths;
	std::cout << "*******************************************************"
		<< endl;
	std::cout << "\nMethod 3 begins:" << endl;
	int max_flow3 = mf.fordFulkerson_BFS_G(graph, AugmentingNum3);
	std::cout << "\nMethod 3 :The maximum possible flow is "
		<< max_flow3 << endl << endl;

	std::cout << "*******************************************************"
		<< "\nFord-Fulkerson max-flow algorithm finishes, with 3 methods to find the augmenting paths:\n" 
		<< "  1) Method 1, BFS in residual capacity graph, producing " << max_flow1 
		<< " max-flow, via " << AugmentingNum1 << " augmenting times.\n"
	    << "  2) Method 2, DFS in residual capacity graph, producing " << max_flow2
		<< " max-flow, via " << AugmentingNum2 << " augmenting times.\n"
		<< "  3) Method 3, BFS in flow graph, producing " << max_flow3
		<< " max-flow, via " << AugmentingNum3 << " augmenting times.\n";
	return 0;
}
