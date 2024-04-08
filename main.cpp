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
    cout << "2. (2.2) Determine Water Demand Vs Actual Flow" << endl;
}

void performAction(Graph& graph, int choice, const unordered_map<string, City>& cityMap, const unordered_map<string, Reservoir>& reservoirMap) {
    std::string cityName;
    double maxFlow = 0;
    switch (choice) {
        case 1:
            Algorithms::maxFlow(graph, reservoirMap, cityMap);

            break;
        case 2: {
            string cityCode;
            cout << "Enter the code of the city: ";
            cin >> cityCode;

            double maxFlow = Algorithms::maxFlowToCity(graph, reservoirMap, cityMap, cityCode);
            cout << "Maximum flow to city " << cityCode << ": " << maxFlow << endl;

            // Find the city object with the given city code
            City city;
            for (const auto& pair : cityMap) {
                if (pair.second.getCode() == cityCode) {
                    city = pair.second;
                    break;
                }
            }

            if (city.getCode() != "") {
                // Get the demand of the city
                double cityDemand = city.getDemand();
                // Calculate the deficit
                double deficit = cityDemand - maxFlow;

                // Output the results
                cout << "City Demand: " << cityDemand << endl;
                cout << "Maximum Flow to City: " << maxFlow << endl;
                cout << "Deficit: " << deficit << endl;
            } else {
                cout << "City with code " << cityCode << " not found." << endl;
            }
            break;
        }

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


    unordered_map<string, City> cityMap;
    for (const auto& city : cities) {
        cityMap[city.getCode()] = city;
    }

    unordered_map<string, Reservoir> reservoirMap;
    for (const auto& reservoir : reservoirs) {
        reservoirMap[reservoir.getCode()] = reservoir;
    }

    int choice;
    do {
        displayMenu();
        cout << "Enter your choice: ";
        cin >> choice;
        performAction(graph, choice, cityMap, reservoirMap);
    } while (choice != 0);

    return 0;


    return 0;

}

