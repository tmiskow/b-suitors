# b-suitors

My implementation of parallel b-suitors algorithm created for Concurrent Programming course at University of Warsaw (2017/2018).

The solution is based on the paper: [*Efficient approximation algorithms for weighted b-Matching* (Khan, Arif, et al, 2016)](https://www.cs.purdue.edu/homes/apothen/Papers/bMatching-SISC-2016.pdf).

## Problem description

From the abstract of *Efficient approximation algorithms for weighted b-Matching* (Khan, Arif, et al, 2016):

> B-Matching is a generalization of the well-known matching problem in graphs, where the objective is to choose a subset of M edges in the graph such that at most a specified number b(v) of edges in M are incident on each vertex v. Subject to this restriction we maximize the sum of the weights of the edges in M.
## Parallel algorithm

From *Efficient approximation algorithms for weighted b-Matching* (Khan, Arif, et al, 2016):

> In this Subsection we describe a shared memory parallel b-Suitor algorithm. It uses iteration rather than recursion; it queues vertices whose proposals have been rejected for later processing unlike the recursive algorithm that processes them immediately. It is to be noted that b-Suitor finds the solution irrespective of the order of the vertices as well as the edges are processed which means the solution is stable irrespective of how operating system schedules the threads. It uses locks for synchronizing multiple threads to ensure sequential consistency. 
> 
> The parallel algorithm is described in Algorithm 3. The algorithm maintains a queue of unsaturated vertices Q which it tries to find partners for during the current iteration of the while loop, and also a queue of vertices Q’ that become deficient in this iteration to be processed again in the next iteration. The algorithm then attempts to find a partner for each vertex u in Q in parallel. It tries to find b(u) proposals for u to make while the adjacency list N(u) has not been exhaustively searched thus far in the course of the algorithm.
## Pseudo-code

**Input:** A graph G = (V, E, w) and a vector b.  **Output:** A 1/2−approximate edge weighted b-Matching M.

```python
procedure Parallel b-Suitor(G, b):
  Q = V ; Q'= {}; 
  while Q is not empty:
    for all vertices u in Q in parallel:
      i = 1;
      while i <= b(u) and N(u) != exhausted:
        Let p in N(u) be an eligible partner of u;
        if p != NULL:
          Lock p; 
          if p is still eligible:
            i = i + 1; 
            Make u a Suitor of p;
            if u annuls the proposal of a vertex v:
              Add v to Q';
              Update db(v); 
          Unlock p; 
        else:
          N(u) = exhausted; 
    Update Q using Q';
    Update b using db;
```

## Performance tests

**Graph:** Pennsylvania road network (http://snap.stanford.edu/data/roadNet-PA.html) 

**CPU:** Intel Core i5-6200U (2.30 GHz, 2 cores, 4 threads)

**Results:**

| Number of threads | Time (in seconds) | Speedup     |
| ----------------- | ----------------- | ----------- |
| 1                 | 16.9026           | 1           |
| 2                 | 14.6354           | 1.154912063 |
| 3                 | 13.4919           | 1.252796122 |
| 4                 | 12.4983           | 1.352391925 |
| 5                 | 11.1461           | 1.516458672 |
| 6                 | 12.8377           | 1.316637715 |
| 7                 | 13.1144           | 1.288858049 |
| 8                 | 11.9881           | 1.409948199 |

![](https://i.imgur.com/MwFNt56.png)![](https://i.imgur.com/BdSe6M8.png)
