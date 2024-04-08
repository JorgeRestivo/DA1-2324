#ifndef PIPE_H
#define PIPE_H

#include <string>
#include <vector>

using namespace std;

class Pipe {
public:
    // Constructors
    Pipe();
    Pipe(const std::string& servicePointA, const std::string& servicePointB, int capacity, int direction);

    //Reading CSV
    vector<Pipe> readFromCSV(const string& filename);

    // Getters
    std::string getServicePointA() const;
    std::string getServicePointB() const;
    int getCapacity() const;
    int getDirection() const;

    // Setters
    void setServicePointA(const std::string& servicePointA);
    void setServicePointB(const std::string& servicePointB);
    void setCapacity(int capacity);
    void setDirection(int direction);

private:
    std::string servicePointA;
    std::string servicePointB;
    int capacity;
    int direction;
};

#endif // PIPE_H
