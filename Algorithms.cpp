
#include "Algorithms.h"
#include "Graph.h"
#include <fstream>
#include <sstream>
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

    graph.removeVertex(superSource);
    graph.removeVertex(superSink);
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

    graph.removeVertex(superSource);
    graph.removeVertex(superSink);
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

std::vector<std::string> Algorithms::simulateReservoirRemoval(Graph& graph, const std::string& reservoirCode, const unordered_map<string, Reservoir>& reservoirs, const unordered_map<string, City>& cities) {
    createMainSource(graph, reservoirs);
    createMainTarget(graph, cities);

    // Save the original max flows to all cities
    double originalMaxFlow = graph.edmondsKarp(graph.findVertex("SuperSource"), graph.findVertex("SuperSink"));
    auto originalFlows = getMaxFlowToCities(graph, cities);

    // Temporarily remove the specified reservoir by setting its delivery to 0
    auto* reservoirVertex = graph.findVertex(reservoirCode);
    if (!reservoirVertex) {
        std::cerr << "Reservoir not found in the graph" << std::endl;
        return {};
    }

    for (auto* edge : reservoirVertex->getAdj()) {
        edge->setCapacity(0);
    }

    // Calculate the new max flows after the reservoir removal
    graph.resetFlows(graph);
    graph.edmondsKarp(graph.findVertex("SuperSource"), graph.findVertex("SuperSink"));
    auto newFlows = getMaxFlowToCities(graph, cities);

    // Restore the original capacity of the removed reservoir
    for (auto* edge : reservoirVertex->getAdj()) {
        edge->setCapacity(reservoirs.at(reservoirCode).getMaxDelivery());
    }

    // Identify cities whose water supply does not meet its demand
    std::vector<std::string> affectedCities;
    for (const auto& city : cities) {
        double originalFlow = originalFlows[city.first];
        double newFlow = newFlows[city.first];

        if (newFlow < city.second.getDemand() && newFlow < originalFlow) {
            int originalFlowInt = static_cast<int>(originalFlow);  // Cast to int
            int newFlowInt = static_cast<int>(newFlow);  // Cast to int

            affectedCities.push_back(city.first + " - " + city.second.getName() +
                                     " (Old Flow: " + std::to_string(originalFlowInt) +
                                     ", New Flow: " + std::to_string(newFlowInt) + ")");
        }
    }

    return affectedCities;
}

std::unordered_map<std::string, double> Algorithms::getMaxFlowToCities(Graph& graph, const std::unordered_map<std::string, City>& cities) {
    std::unordered_map<std::string, double> flows;

    for (const auto& cityPair : cities) {
        const std::string& cityCode = cityPair.first;
        Vertex* cityVertex = graph.findVertex(cityCode);
        if (cityVertex) {
            double flowToCity = 0.0;
            for (Edge* edge : cityVertex->getIncoming()) { // Assuming Vertex has a method to get incoming edges
                flowToCity += edge->getFlow(); // Assuming Edge has a getFlow method
            }
            flows[cityCode] = flowToCity;
        }
    }

    return flows;
}

std::vector<std::string> Algorithms::determinePipelineFailures(Graph& graph, const std::string& input, const std::unordered_map<std::string, City>& cities) {
    graph.resetFlows(graph);  // Assuming resetFlow is a method that takes a Graph object
    auto originalFlow = getMaxFlowToCities(graph, cities);
    graph.resetFlows(graph);

    auto pipeCodes = processInput(input);  // Assuming processInput parses input and returns edge codes

    for (auto& pipeCode : pipeCodes) {
        std::string code1 = pipeCode.first;
        std::string code2 = pipeCode.second;

        for (auto& vertexPair : graph.getVertexMap()) {
            Vertex* vertex = vertexPair.second;
            for (auto& edge : vertex->getAdj()) {
                if (edge->getCapacity() <= 0) continue;
                if ((edge->getOrig()->getCode() != code1 && edge->getDest()->getCode() != code1) ||
                    (edge->getOrig()->getCode() != code2 && edge->getDest()->getCode() != code2)) continue;

                edge->setCapacity(0);  // Simulate pipeline failure
            }
        }
    }

    auto newFlow = getMaxFlowToCities(graph, cities);
    graph.resetFlows(graph);  // Reset the flow after simulation

    std::vector<std::string> affectedCities;
    for (const auto& pair : cities) {
        const std::string& cityCode = pair.first;
        const City& city = pair.second;
        double demand = city.getDemand();  // Assuming City objects have a getDemand method

        if (originalFlow[cityCode] == demand && newFlow[cityCode] < demand) {
            int newFlowInt = static_cast<int>(newFlow[cityCode]);
            int originalFlowInt = static_cast<int>(originalFlow[cityCode]);
            affectedCities.push_back(cityCode + ": " + city.getName() + "- Old flow: " +
                                     std::to_string(originalFlowInt) + "- New flow: " + std::to_string(newFlowInt));
        }
    }

    return affectedCities;
}

std::vector<std::pair<std::string, std::string>> Algorithms::processInput(const std::string& input) {
    std::vector<std::pair<std::string, std::string>> codePairs;
    std::istringstream stream(input);
    std::string segment;

    while (std::getline(stream, segment, ',')) {
        std::istringstream pairStream(segment);
        std::string code1, code2;
        if (std::getline(pairStream, code1, '-') && std::getline(pairStream, code2)) {
            codePairs.emplace_back(std::move(code1), std::move(code2));
        }
    }

    return codePairs;
}