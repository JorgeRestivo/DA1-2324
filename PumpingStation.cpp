#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include "PumpingStation.h"

using namespace std;

// Constructors
PumpingStation::PumpingStation() : id(0) {}

PumpingStation::PumpingStation(int id, const std::string& code) : id(id), code(code) {}

// Destructor
PumpingStation::~PumpingStation() {}

//Reading CSV
vector<PumpingStation> PumpingStation::readFromCSV(const string& filename) {
    vector<PumpingStation> pumpingStations;

    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error opening file: " << filename << endl;
        return pumpingStations;
    }

    string line;
    // Skip the header line
    getline(file, line);

    while (getline(file, line)) {
        stringstream ss(line);
        string code1;
        int id1, maxDelivery1;

        // Parsing CSV line
        ss >> id1;
        ss.ignore();
        getline(ss, code1, ',');


        // Creating PumpingStation object and adding to vector
        PumpingStation  pumpingStation(id1, code1);
        pumpingStations.push_back(pumpingStation);
    }

    file.close();
    return pumpingStations;
}

// Getters
int PumpingStation::getId() const {
    return id;
}

std::string PumpingStation::getCode() const {
    return code;
}

// Setters
void PumpingStation::setId(int id) {
    this->id = id;
}

void PumpingStation::setCode(const std::string& code) {
    this->code = code;
}
