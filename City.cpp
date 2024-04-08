#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include "City.h"

using namespace std;

// Constructors
City::City() : id(0), demand(0.0) {}

City::City(const string& name, int id, const string& code, double demand, const string& population)
        : name(name), id(id), code(code), demand(demand), population(population) {}

//Reading CSV
vector<City> City::readFromCSV(const string& filename) {
    vector<City> cities;

    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error opening file: " << filename << endl;
        return cities;
    }

    string line;
    // Skip the header line
    getline(file, line);

    while (getline(file, line)) {
        stringstream ss(line);
        string name1, code1, population1;
        int id1;
        double demand1;

        // Parsing CSV line
        getline(ss, name1, ',');
        ss >> id1;
        ss.ignore();
        getline(ss, code1, ',');
        ss >> demand1;
        getline(ss, population1, ',');


        // Creating PumpingStation object and adding to vector
        City city(name1, id1, code1, demand1, population1);
        cities.push_back(city);
    }

    file.close();
    return cities;
}

// Destructor
City::~City() {}

// Getters
string City::getName() const {
    return name;
}

int City::getId() const {
    return id;
}

string City::getCode() const {
    return code;
}

double City::getDemand() const {
    return demand;
}

string City::getPopulation() const {
    return population;
}

// Setters
void City::setName(const std::string& name) {
    this->name = name;
}

void City::setId(int id) {
    this->id = id;
}

void City::setCode(const std::string& code) {
    this->code = code;
}

void City::setDemand(double demand) {
    this->demand = demand;
}

void City::setPopulation(int population) {
    this->population = population;
}
