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
    static double maxFlowToCity(Graph& graph, const unordered_map<string, Reservoir>& reservoirs, const unordered_map<string, City>& cities, const std::string& cityCode);
    vector<std::string> simulateReservoirRemoval(const std::string& reservoirCode);
    static std::vector<std::string> simulateReservoirRemoval(Graph& graph, const std::string& reservoirCode, const unordered_map<string, Reservoir>& reservoirs, const unordered_map<string, City>& cities);
    static std::unordered_map<std::string, double> getMaxFlowToCities(Graph& graph, const std::unordered_map<std::string, City>& cities);

    static std::vector<std::pair<std::string, std::string>> processInput(const std::string& input);
    static std::vector<std::string> determinePipelineFailures(Graph& graph, const std::string& input, const std::unordered_map<std::string, City>& cities);

    static void createMainSource(Graph& graph, const unordered_map<string, PumpingStation>& pumpingStations);
    static std::vector<std::string> simulatePumpingStationRemoval(Graph& graph, const std::unordered_map<std::string, std::vector<Edge*>>& stationPipes, const std::unordered_map<std::string, City>& cities);
    std::unordered_map<std::string, std::vector<Edge*>> createStationPipes(Graph& graph);

    };

#endif // ALGORITHMS_H
