#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include "Reservoir.h"

using namespace std;

// Constructors
Reservoir::Reservoir() : id(0), maxDelivery(0.0) {}

Reservoir::Reservoir(const std::string& name, const std::string& municipality, int id, const std::string& code, int maxDelivery)
        : name(name), municipality(municipality), id(id), code(code), maxDelivery(maxDelivery) {}

// Destructor
Reservoir::~Reservoir() {}

//Reading CSV
vector<Reservoir> Reservoir::readFromCSV(const string& filename) {
    vector<Reservoir> reservoirs;

    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error opening file: " << filename << endl;
        return reservoirs;
    }

    string line;
    // Skip the header line
    getline(file, line);

    while (getline(file, line)) {
        stringstream ss(line);
        string name1, municipality1, code1;
        int id1, maxDelivery1;

        // Parsing CSV line
        getline(ss, name1, ',');
        getline(ss, municipality1, ',');
        ss >> id1; ss.ignore();
        getline(ss, code1, ',');
        ss >> maxDelivery1; ss.ignore();

        // Creating Reservoir object and adding to vector
        Reservoir reservoir(name1, municipality1, id1, code1, maxDelivery1);
        reservoirs.push_back(reservoir);
    }

    file.close();
    return reservoirs;
}

// Getters
std::string Reservoir::getName() const {
    return name;
}

std::string Reservoir::getMunicipality() const {
    return municipality;
}

int Reservoir::getId() const {
    return id;
}

std::string Reservoir::getCode() const {
    return code;
}

int Reservoir::getMaxDelivery() const {
    return maxDelivery;
}

// Setters
void Reservoir::setName(const std::string& name) {
    this->name = name;
}

void Reservoir::setMunicipality(const std::string& municipality) {
    this->municipality = municipality;
}

void Reservoir::setId(int id) {
    this->id = id;
}

void Reservoir::setCode(const std::string& code) {
    this->code = code;
}

void Reservoir::setMaxDelivery(int maxDelivery) {
    this->maxDelivery = maxDelivery;
}
