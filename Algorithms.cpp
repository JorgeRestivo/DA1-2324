
#include "Algorithms.h"
#include "Graph.h"
#include <fstream>
#include <algorithm>
using namespace std;

void Algorithms::maxFlow(Graph& graph, const unordered_map<string, Reservoir>& reservoirs, const unordered_map<string, City>& cities) {
    createMainSource(graph, reservoirs);
    createMainTarget(graph, cities);

    Vertex* superSource = graph.findVertex("SuperSource");
    Vertex* superSink = graph.findVertex("SuperSink");

    if (!superSource || !superSink) {
        throw std::runtime_error("Super source or super sink not found in the graph");
    }

    double totalMaxFlow = graph.edmondsKarp(superSource, superSink);

    // Define the Output file name
    string outputFile = "../Output/MaxFlowToEachCity.txt";

    // Write max flow values for each city to the Output file
    ofstream outputFileStream(outputFile);
    outputFileStream << "By city:" << std::endl;

    // Create a vector to store the city vertices
    std::vector<Vertex*> cityVertices;

    // Collect city vertices into the vector
    for (const auto& [key, v] : graph.getVertexMap()) {
        if (v->getType() == Type::CITY) {
            cityVertices.push_back(v);
        }
    }

    // Sort city vertices by their codes
    std::sort(cityVertices.begin(), cityVertices.end(), [](const Vertex* a, const Vertex* b) {
        return Algorithms::compareCityCodes(a->getCode(), b->getCode());
    });

    // Write city information to the output file
    for (const auto& cityVertex : cityVertices) {
        auto it = cities.find(cityVertex->getCode());
        if (it != cities.end()) {
            std::string cityName = it->second.getName();
            outputFileStream << cityVertex->getCode() << "-" << cityName << " " << cityVertex->getFlow() << std::endl;
        } else {
            std::cerr << "City with code " << cityVertex->getCode() << " not found in the cities map." << std::endl;
        }
    }

    outputFileStream.close();

    std::cout << "Total maximum flow: " << totalMaxFlow << " m3/sec." << std::endl;
    std::cout << "If you want to see all the cities detail please search for file MaxFlowToEachCity.txt on Output folder." << std::endl;
}

double Algorithms::maxFlowToCity(Graph& graph, const unordered_map<string, Reservoir>& reservoirs, const unordered_map<string, City>& cities, const std::string& cityCode) {
    createMainSource(graph, reservoirs);
    createMainTarget(graph, cities);

    Vertex* superSource = graph.findVertex("SuperSource");
    Vertex* superSink = graph.findVertex("SuperSink");

    if (!superSource || !superSink) {
        throw std::runtime_error("Super source or super sink not found in the graph");
    }

    double totalMaxFlow = graph.edmondsKarp(superSource, superSink);

    // Retrieve the max flow for the specific city
    double maxFlowToCity = graph.getMaxFlowToCity(cityCode);

    return maxFlowToCity;
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

bool Algorithms::compareCityCodes(const std::string& code1, const std::string& code2) {
    // Extract numeric part of the codes
    int num1 = std::stoi(code1.substr(code1.find('_') + 1));
    int num2 = std::stoi(code2.substr(code2.find('_') + 1));

    return num1 < num2;
}

