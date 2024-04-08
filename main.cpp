#include <iostream>
#include <filesystem>
#include "Graph.h"
#include "Reservoir.h"
#include "PumpingStation.h"

namespace fs = std::__fs::filesystem;

using namespace std;


void displayMenu() {
    cout << "\n=== Menu ===\n" << endl;
    cout << "1. (2.1) Determine maximum water flow." << endl;
}

void performAction(Graph& graph, int choice) {
    switch (choice) {
        case 1:
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
    string pipesFile = cityDataSet == 1 ? "Pipes_Madeira.csv" : "Pipes.csv";

    Graph graph;
    Reservoir r;
    PumpingStation p;
    City c;

    //Read the files
    vector<Reservoir> reservoirs = r.readFromCSV(baseDirectory + reservoirFile);
    vector<PumpingStation> pumpingStations = p.readFromCSV(baseDirectory + pumpingStationsFile);
    vector<City> cities = c.readFromCSV(baseDirectory + citiesFile);

    // Add reservoirs to the graph
    for (const auto& reservoir : reservoirs) {
        graph.addVertex(reservoir);
    }

    // Add pumping stations to the graph
    for (const auto& pumpingStation : pumpingStations) {
        graph.addVertex(pumpingStation);
    }

    // Add cities to the graph
    for (const auto& city : cities) {
        graph.addVertex(city);
    }


    for (const auto& reservoir : reservoirs) {
        cout << "Name: " << reservoir.getName() << endl;
        cout << "Municipality: " << reservoir.getMunicipality() << endl;
        cout << "ID: " << reservoir.getId() << endl;
        cout << "Code: " << reservoir.getCode() << endl;
        cout << "Max Delivery: " << reservoir.getMaxDelivery() << endl;
        cout << "===============================" << endl;
    }

    int choice;
    do {
        displayMenu();
        cout << "Enter your choice: ";
        cin >> choice;
        performAction(graph, choice);
    } while (choice != 0);

    return 0;

}

