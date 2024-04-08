#ifndef DA_TP_CLASSES_GRAPH
#define DA_TP_CLASSES_GRAPH

#include <iostream>
#include <vector>
#include <list>
#include <queue>
#include <limits>
#include <algorithm>
#include <climits>
#include <execution>
#include <unordered_map>
#include "VertexEdge.h"
#include "Reservoir.h"
#include "PumpingStation.h"
#include "City.h"

class Graph {
public:
    ~Graph();

    const unordered_map<std::string, Vertex *> & getVertexMap() const;

    /**
     * @brief Auxiliary function to find a vertex with a given ID
     * @details Time Complexity: O(1)
     * @param name
     * @returns The found vertex with the specified name
    */
    Vertex * findVertex(const std::string &code) const;

    /**
     * @brief Adds a vertex with a given content or info (in) to a graph (this).
     * @details Time Complexity: O(1)
     * @param station
     * @returns true if successful, and false if a vertex with that content already exists.
     */
    bool addVertex(const Reservoir& reservoir);
    bool addVertex(const PumpingStation& pumpingStation);
    bool addVertex(const City& city);
    bool addVertex(Vertex* vertex);

    double getMaxFlowToCity(const std::string& cityCode);

    void printGraph(const Graph& graph);
    void resetFlows(Graph& graph);

    /**
     * @brief Adds a bidirectional edge to a graph (this), given the contents of the source and destination vertices and the edge weight (w)
     * @details Time Complexity: O(1)
     * @param dest
     * @param sourc
     * @param w
     * @returns true if successful, and false if the source or destination vertex does not exist.
     */
    //bool addBidirectionalEdge(const Station &sourc, const Station &dest, const Connection& w) const;

    /**
     * @brief Handles the bottleneck values got from the maxflow_bfs obtaining the Maximum Flow
     * @details Time Complexity: O(V*E^2), being V the number of vertexes and E the number of edges
     * @param startStation
     * @param endStation
     * @returns the value of the Maximum Flow
     */
    int maxFlow(Vertex* startStation, Vertex* endStation);

    /**
     * @brief Processes dfs_graphCost results and selects the one of interest
     * @details Time Complexity: O(n*log(n)) being n the number of elements in the results vector
     * @param graph
     * @param startStation
     * @param endStation
     * @returns the vector with the information regarding the best path
     */
    //vector<int> graphCost(Graph & graph, Vertex startStation, Vertex endStation);

    /**
     * @brief Sets up the variables needed while the dfs_graphCostHelper searches all the paths
     * @details Time Complexity: O(V+E), being V the number of vertexes and E the number of edges
     * @param graph
     * @param startStation
     * @param endStation
     * @returns a vector of vectors containing information regarding every path
     */
    vector<vector<int>> dfs_graphCost(Graph & graph, Vertex & startStation, Vertex & endStation);

    /**
     * @brief Does the actual searching always saving every information regarding every path it goes through
     * @details Time Complexity: O(V+E), being V the number of vertexes and E the number of edges
     * @param graph
     * @param current
     * @param endStation
     * @param path
     * @param results
     * @param maxFlowSoFar
     */
    void dfs_graphCostHelper(Graph &graph, Vertex &current, Vertex &endStation, vector<string> &path,
                             vector<vector<int>> &results, double &maxFlowSoFar);


    /**
     * @brief Searches for the maximum number of trains that can get to the given station from any other station
     * @details Time Complexity: O(V*(V+E)*n), being V the number of vertexes, E the number of edges and n the number of vertexes along a path
     * @param stationName
     * @returns The highest maximum flow from a determined station to the given station
     */
    int getMaxTrains(const string &stationName) const;

    /**
     * @brief Edmonds-Karp algorithm adapted to this graph
     * @details Time Complexity: O(V*E^2) being V the number of vertexes and E the number of edges
     * @param s
     * @param t
     * @returns the maximum flow from vertex s to t
     */
    double edmondsKarp(Vertex* s, Vertex* t);

protected:

    /// Vertex Map
    std::unordered_map<std::string, Vertex *> vertexMap;

    /// Dist matrix for Floyd-Warshall
    double ** distMatrix = nullptr;

    /// Path matrix for Floyd-Warshall
    double **pathMatrix = nullptr;

    /**
     * @brief Adapted Ford-Fulkerson algorithm for the graph in question
     * @details Time Complexity: O((V+E)*n), being V the number of vertexes, E the number of edges and n the number of vertexes along a path
     * @param source
     * @param target
     * @returns the maximum flow
     */
    int fordFulkerson(Vertex *source, Vertex *target) const;

    /**
     * @brief Adapted bfs algorithm for the graph in question
     * @details Time Complexity: O(V+E), being V the number of vertexes and E the number of edges
     * @param source
     * @param sink
     * @returns false if there is no path from source to sink and true otherwise
     */
    bool bfs(Vertex *source, Vertex *sink) const;


    /**
     * @brief Calculates the bottleneck capacity of a determined augmenting path
     * @details Time Complexity: O(n), being n the number of vertexes along a path
     * @param sink
     * @returns The value of the bottleneck capacity of the current path
     */
    static int getBottleneckCapacity(Vertex* sink);

    /**
     * @brief Follows the path from sink to source and updates the flow along each edge
     * @details Time Complexity: O(n), being n the number of  vertexes along a path
     * @param sink
     * @param bottleneck
     */
    static void updateFlow(Vertex* sink, int bottleneck);

    /**
     * @brief Finds the minimum residual capacity along a path
     * @details Time Complexity: O(n), being n the number of edges along the path between t and s
     * @param s
     * @param t
     * @returns the minimum residual capacity along a path from s to t
     */
    static double findMinResidualAlongPath(Vertex *s, Vertex *t);

    /**
     * @brief Finds an augmenting path
     * @details Time Complexity: O(V+E), being V the number of Vertexes and E the number of Edges
     * @param s
     * @param t
     * @returns true if an augmenting path was found and false otherwise
     */
    bool findAugmentingPath(Vertex *s, Vertex *t);

    /**
     * @brief Visits and tests vertices
     * @details Time Complexity: O(1)
     * @param q
     * @param e
     * @param w
     * @param residual
     */
    static void testAndVisit(queue<Vertex *> &q, Edge *e, Vertex *w, double residual);

    /**
     * @brief Finds paths with non-zero flow and updates the flow along those paths
     * @details Time Complexity: O(n), being n the number of edges along the path between t and s
     * @param s
     * @param t
     * @param f
     */
    static void augmentFlowAlongPath(Vertex *s, Vertex *t, double f);
};

/**
 * @brief Deletes Matrixes
 * @param m
 * @param n
 */
void deleteMatrix(double **m, int n);

#endif /* DA_TP_CLASSES_GRAPH */