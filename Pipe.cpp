#include "Pipe.h"
#include "Pipe.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

// Constructors
Pipe::Pipe() : capacity(0), direction(0) {}

Pipe::Pipe(const std::string& servicePointA, const std::string& servicePointB, int capacity, int direction)
        : servicePointA(servicePointA), servicePointB(servicePointB), capacity(capacity), direction(direction) {}

//Reading CSV
vector<Pipe> Pipe::readFromCSV(const string& filename) {
    vector<Pipe> pipes;

    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error opening file: " << filename << endl;
        return pipes;
    }

    string line;
    // Skip the header line
    getline(file, line);

    while (getline(file, line)) {
        stringstream ss(line);
        string servicePointA, servicePointB;
        int capacity, direction;

        // Parsing CSV line
        getline(ss, servicePointA, ',');
        getline(ss, servicePointB, ',');
        ss >> capacity;
        ss.ignore(); // Ignore the comma
        ss >> direction;

        // Creating Reservoir object and adding to vector
        Pipe pipe(servicePointA, servicePointB, capacity, direction);
        pipes.push_back(pipe);
    }

    file.close();
    return pipes;
}

// Getters
std::string Pipe::getServicePointA() const {
    return servicePointA;
}

std::string Pipe::getServicePointB() const {
    return servicePointB;
}

int Pipe::getCapacity() const {
    return capacity;
}

int Pipe::getDirection() const {
    return direction;
}

// Setters
void Pipe::setServicePointA(const std::string& servicePointA) {
    this->servicePointA = servicePointA;
}

void Pipe::setServicePointB(const std::string& servicePointB) {
    this->servicePointB = servicePointB;
}

void Pipe::setCapacity(int capacity) {
    this->capacity = capacity;
}

void Pipe::setDirection(int direction) {
    this->direction = direction;
}
