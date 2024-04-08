#include <iostream>
#include <filesystem>
#include "Graph.h"
#include "Reservoir.h"
#include "PumpingStation.h"
#include "Pipe.h"
#include "Algorithms.h"

namespace fs = std::__fs::filesystem;

using namespace std;


void displayMenu() {
    cout << "\n=== Menu ===\n" << endl;
    cout << "1. (2.1) Determine maximum water flow." << endl;
    cout << "2. (2.1) Determine maximum water flow to a SPECIFIC city." << endl;
}

void performAction(Graph& graph, int choice, const unordered_map<string, City>& cityMap) {
    std::string cityName;
    double maxFlow = 0;
    switch (choice) {
        case 1:
            cout << "Maximum water flow to all cities:" << endl;
            Algorithms::printMaxFlowToAllCities(graph, cityMap);
            break;
        case 2:
            cout << "Enter City Name (format C_X):";
            cin >> cityName;
            maxFlow = Algorithms::getMaxFlowToCity(cityName, graph, cityMap);
            if (maxFlow != -1) {
                cout << cityName << ": " << maxFlow << endl;
            }
            break;
        case 0:
            cout << "Exiting the program. Goodbye!" << endl;
            break;
        default:
            cout << "Invalid choice. Please try again." << endl;
    }
}


int main() {
    // Prompt user to choose the dataset
    int cityDataSet;
    cout << "Current working directory: " << fs::current_path() << endl;
    cout << "Choose the dataset (Madeira (1) / Continente (2)): ";
    cin >> cityDataSet;

    // Parse the CSV files based on the chosen dataset
    string baseDirectory = cityDataSet == 1 ? "../Project1DataSetSmall/" : "../Project1LargeDataSet/";
    string reservoirFile = cityDataSet == 1 ? "Reservoirs_Madeira.csv" : "Reservoir.csv";
    string pumpingStationsFile = cityDataSet == 1 ? "Stations_Madeira.csv" : "Stations.csv";
    string citiesFile = cityDataSet == 1 ? "Cities_Madeira.csv" : "Cities.csv";
    string pipesFile = cityDataSet == 1 ? "Pipes_Madeira.csv" : "Pipe.csv";

    Graph graph;
    Reservoir r;
    PumpingStation p;
    City c;
    Pipe pp;

    //Read the files
    vector<Reservoir> reservoirs = r.readFromCSV(baseDirectory + reservoirFile);
    vector<PumpingStation> pumpingStations = p.readFromCSV(baseDirectory + pumpingStationsFile);
    vector<City> cities = c.readFromCSV(baseDirectory + citiesFile);
    vector<Pipe> pipes = pp.readFromCSV(baseDirectory + pipesFile);

    // Add reservoirs to the graph
    for (const auto& reservoir : reservoirs) {
        graph.addVertex(reservoir);
    }

    // Add pumping stations to the graph
    for (const auto& pumpingStation : pumpingStations) {
        if (!pumpingStation.getCode().empty()) {
            graph.addVertex(pumpingStation);
        }
    }

    // Add cities to the graph
    for (const auto& city : cities) {
        graph.addVertex(city);
    }

    // Add pipes as edges
    for (const auto& pipe : pipes) {
        Vertex* sourceVertex = graph.findVertex(pipe.getServicePointA());
        Vertex* destVertex = graph.findVertex(pipe.getServicePointB());

        if (sourceVertex && destVertex) {
            sourceVertex->addBidirectionalEdge(destVertex, pipe.getCapacity(), pipe.getDirection());
        } else {
            throw std::invalid_argument("One or both vertices not found for pipe edge");
        }
    }

    graph.printGraph(graph);


    unordered_map<string, City> cityMap;
    for (const auto& city : cities) {
        cityMap[city.getCode()] = city;
    }

    int choice;
    do {
        displayMenu();
        cout << "Enter your choice: ";
        cin >> choice;
        performAction(graph, choice, cityMap); // Pass cityMap
    } while (choice != 0);

    return 0;


    return 0;

}

