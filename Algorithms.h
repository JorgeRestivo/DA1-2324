#ifndef ALGORITHMS_H
#define ALGORITHMS_H

#include "Graph.h"
#include "VertexEdge.h"
#include <string>
#include <vector>

class Algorithms {
public:
    static double getMaxFlowToCity(const std::string& cityCode, Graph& graph, const std::unordered_map<std::string, City>& cityMap);
    static void printMaxFlowToAllCities(Graph& graph, const std::unordered_map<std::string, City>& cityMap);
};

#endif // ALGORITHMS_H
