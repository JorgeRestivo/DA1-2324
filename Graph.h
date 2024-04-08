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

    Vertex * findVertex(const std::string &code) const;

    bool addVertex(const Reservoir& reservoir);
    void removeVertex(Vertex* vertex);
    bool addVertex(const PumpingStation& pumpingStation);
    bool addVertex(const City& city);
    bool addVertex(Vertex* vertex);

    double getMaxFlowToCity(const std::string& cityCode);

    void printGraph(const Graph& graph);
    void resetFlows(Graph& graph);

    double edmondsKarp(Vertex* s, Vertex* t);

protected:

    std::unordered_map<std::string, Vertex *> vertexMap;

    double ** distMatrix = nullptr;

    double **pathMatrix = nullptr;

    bool bfs(Vertex *source, Vertex *sink) const;

    static void updateFlow(Vertex* sink, int bottleneck);

    static double findMinResidualAlongPath(Vertex *s, Vertex *t);

    bool findAugmentingPath(Vertex *s, Vertex *t);

    static void testAndVisit(queue<Vertex *> &q, Edge *e, Vertex *w, double residual);

    static void augmentFlowAlongPath(Vertex *s, Vertex *t, double f);
};

void deleteMatrix(double **m, int n);

#endif /* DA_TP_CLASSES_GRAPH */