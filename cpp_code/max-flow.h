/**
* @file: max-flow.cpp
* @brief:
* @author: Changjiang Cai, caicj5351@gmail.com
* @version: 0.0.1
* @creation date: 11-12-2015
* @last modified: Sat 12 Dec 2015 02:32:51 PM EST
*/
#ifndef _MAX_FLOW_HEADE_
#define _MAX_FLOW_HEADE_

// C++ program for implementation of Ford Fulkerson algorithm
#include <iostream>
#include <stdio.h>
#include <limits.h>
#include <cstring> /* memset */
#include <string>
#include <queue>
#include <vector>
using namespace std;

// Number of vertices in given graph
#define V 6
// color 
// In DFS, each vertex has three possible 
// colors representing its state:
//   * white : vertex is unvisited;
//   * gray: vertex is in progress;
//   * black: DFS has finished processing the vertex.
enum VertexColors : int { BLACK = 1, WHITE = 2, GRAY = 3 };
//#define sinkIdx 5
//#define sourceIdx 0
class max_flow {
private:
	bool isDisplay;
	int dfs_time = 0;
	int vertexNum;
	int sink;
	int source;
	// Create a visited array and mark all vertices as not visited
	VertexColors vColors[V];
	int parents[V];
	int discoverTime[V];
	int finishTime[V];
	vector<int> augmentpath;
public:
	max_flow(int s, int t, bool isDis);
	void initialize();
	bool runSBFS(int rGraph[V][V], int capacityGraph[V][V]);
	bool runBFS(int rGraph[V][V]);
	void runDFS(int rGraph[V][V], int u);
	inline bool DFS(int rGraph[V][V]);
	void DFS();
	// Returns the maximum flow from s to t in the given graph
	// method 1;
	int fordFulkerson_BFS_RG(int graph[V][V], int & AugmentingNum);
	// method 2;
	int fordFulkerson_DFS_RG(int graph[V][V], int & AugmentingNum);
	// method 3;
	int fordFulkerson_BFS_G(int graph[V][V], int & AugmentingNum);
	
	inline bool savePrintPath(std::string searchName, int s, int t){
		int ancestor = parents[t];
		if (t == s){
			std::cout << searchName << " finds an augmenting path : ";
		}
		else if (ancestor == -1){
			std::cout << "No path from " << s + 1 << " to " << t + 1;
			return false;
		}
		else {
			savePrintPath(searchName, s, ancestor);
		}

		if (t == sink)
			std::cout << t + 1 << ";" << endl;
		else
			std::cout << t + 1 << " -- ";
		// save the vertex into the augmenting path;
		augmentpath.push_back(t);
		return true;
	}

	inline bool printPath(std::string searchName, int s, int t){
		int ancestor = parents[t];
		if (t == s){
			std::cout << searchName << " finds an augmenting path : ";
		}
		else if (ancestor == -1){
			std::cout << "No path from " << s + 1 << " to " << t + 1 << endl;
			return false;
		}
		else {
			printPath(searchName, s, ancestor);
		}

		if (t == sink)
			std::cout << t + 1 << ";" << endl;
		else
			std::cout << t + 1 << " -- ";
		return true;
	}
};

#endif
