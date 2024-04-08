
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


void Algorithms::maxFlow(Graph& graph, const unordered_map<string, Reservoir>& reservoirs, const unordered_map<string, City>& cities) {
    createMainSource(graph, reservoirs);
    createMainTarget(graph, cities);

    Vertex* superSource = graph.findVertex("SuperSource");
    Vertex* superSink = graph.findVertex("SuperSink");

    if (!superSource || !superSink) {
        throw std::runtime_error("Super source or super sink not found in the graph");
    }

    double totalMaxFlow = graph.edmondsKarp(superSource, superSink);

    // Define the output file name
    string outputFile = "../output/output.txt";

    // Write max flow values for each city to the output file
    ofstream outputFileStream(outputFile);
    if (!outputFileStream.is_open()) {
        std::cerr << "Failed to open output file: " << outputFile << std::endl;
        throw std::runtime_error("Failed to open output file");
    }

    for (const auto& [key, v] : graph.getVertexMap()) {
        if (v->getType() == Type::CITY) {
            outputFileStream << v->getCode() << "-" << " " << v->getFlow() << std::endl;
        }
    }

    outputFileStream.close();

    std::cout << "Total maximum flow from all reservoirs to all cities: " << totalMaxFlow << std::endl;
}

void Algorithms::createMainSource(Graph& graph, const unordered_map<string, Reservoir>& reservoirs) {
    Vertex* superSource = new Vertex("SuperSource", Type::SUPER_SOURCE);
    if (!graph.addVertex(superSource)) {
        std::cerr << "Failed to add super source to graph" << std::endl;
        delete superSource;  // Clean up if not added to graph
        return;
    }

    for (const auto& [code, reservoir] : reservoirs) {
        Vertex* reservoirVertex = graph.findVertex(reservoir.getCode());
        if (reservoirVertex) {
            // Use the . operator to access methods on reservoir object
            superSource->addEdge(reservoirVertex, reservoir.getMaxDelivery(), 0);  // Assuming 0 for direction
        } else {
            std::cerr << "Reservoir vertex not found for code: " << code << std::endl;
        }
    }
}


void Algorithms::createMainTarget(Graph& graph, const unordered_map<string, City>& cities) {
    Vertex* superSink = new Vertex("SuperSink", Type::SUPER_SINK);
    if (!graph.addVertex(superSink)) {
        std::cerr << "Failed to add super sink to graph" << std::endl;
        delete superSink; // Clean up if not added to graph
        return;
    }

    for (const auto& [cityCode, city] : cities) {
        Vertex* cityVertex = graph.findVertex(cityCode);
        if (cityVertex) {
            cityVertex->addEdge(superSink, city.getDemand(), 0); // Assuming 0 for direction
        }
    }
}

