#ifndef ALGORITHMS_H
#define ALGORITHMS_H

#include "Graph.h"
#include "VertexEdge.h"
#include "Reservoir.h"
#include <string>
#include <vector>

class Algorithms {
public:
    static double getMaxFlowToCity(const std::string& cityCode, Graph& graph, const std::unordered_map<std::string, City>& cityMap);
    static void maxFlow(Graph& graph, const unordered_map<string, Reservoir>& reservoirs, const unordered_map<string, City>& cities);
    static void createMainSource(Graph& graph, const unordered_map<string, Reservoir>& reservoirs);
    static void createMainTarget(Graph& graph, const unordered_map<string, City>& cities);
    static bool compareCityCodes(const std::string& code1, const std::string& code2);
};

#endif // ALGORITHMS_H
