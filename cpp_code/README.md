## 1. Problem Definition:

Given a graph which represents a flow network where every edge has a capacity, and two vertices source `s` and sink `t` in the graph, to find the maximum possible flow from `s` to `t` with following constraints:


- a) Flow on an edge doesn’t exceed the given capacity of the edge.


- b) Incoming flow is equal to outgoing flow for every vertex except `s` and `t`.



## 2. Ford-Fulkerson Algorithm 

The following is simple idea of Ford-Fulkerson algorithm:

1) Start with initial flow as `0`.

2) While there is a augmenting path from source to sink. 
   - Add this path-flow to flow.

3) Return flow.


## 3. Time Complexity

Time complexity of the above algorithm is `O(max_flow * E)`. We run a loop while there is an augmenting path. In worst case, we may add 1 unit flow in every iteration. Therefore the time complexity becomes `O(max_flow *E)`.

## 4. Implement the above simple algorithm.

### 1) Residual Graph: 
Residual Graph of a flow network is a graph which indicates additional
possible flow. If there is a path from source to sink in residual graph, then
it is possible to add flow. Every edge of a residual graph has a value called
residual capacity which is equal to original capacity of the edge minus
current flow. 

### 2) Finding an augmenting path:
we can either do a BFS or DFS of the residual graph, or specialized BFS or DFS of the flow graph. 

## Usage:

Run this code as below (It will need CMake installed in your conmputer, or you can compile it via g++):
```cmake
   cd this_folder/build
   cmake ..
   make
```

Then an executable file named `max-flow` will be generated in the directory of `./bin/max-flow`.