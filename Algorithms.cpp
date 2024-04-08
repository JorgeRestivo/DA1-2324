
#include "Algorithms.h"
#include "Graph.h"
#include <fstream>

double Algorithms::getMaxFlowToCity(const std::string& cityCode, Graph& graph, const std::unordered_map<std::string, City>& cityMap) {
    Vertex* cityVertex = nullptr;
    for (const auto& entry : graph.getVertexMap()) {
        if (entry.first == cityCode) {
            cityVertex = entry.second;
            break;
        }
    }

    // Check if the city vertex was found
    if (!cityVertex) {
        cout << "City not found!" << endl;
        return -1;
    }

    // Initialize total flow and retrieve city demand
    double totalFlow = 0;
    double cityDemand = -1; // Initialize to -1 in case city demand is not found
    auto cityIt = cityMap.find(cityCode);
    if (cityIt != cityMap.end()) {
        cityDemand = cityIt->second.getDemand();
    } else {
        cout << "City demand not found for city code: " << cityCode << endl;
        return -1;
    }

    // Iterate over all reservoir vertices to calculate maximum flow to the city
    for (const auto& entry : graph.getVertexMap()) {
        Vertex* source = entry.second;
        if (source->getType() == Type::RESERVOIR) {
            // Calculate maximum flow from reservoir to the city
            double flow = graph.edmondsKarp(source, cityVertex);
            totalFlow += flow;
        }
    }

    // Adjust total flow if it exceeds city demand
    if (totalFlow > cityDemand) {
        totalFlow = cityDemand;
    }

    return totalFlow;
}

void Algorithms::printMaxFlowToAllCities(Graph& graph, const std::unordered_map<std::string, City>& cityMap) {
    for (const auto& cityEntry : cityMap) {
        const std::string& cityCode = cityEntry.first;
        const City& city = cityEntry.second;

        double maxFlow = getMaxFlowToCity(cityCode, graph, cityMap);

        // Print the result for the current city
        if (maxFlow != -1) {
            std::cout << cityCode << " - " << city.getName() << " " << maxFlow << std::endl;
        }
    }
}
