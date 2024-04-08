#ifndef PUMPINGSTATION_H
#define PUMPINGSTATION_H

#include <string>
#include <vector>

using namespace std;

class PumpingStation {
public:
    // Constructors
    PumpingStation();
    PumpingStation(int id, const std::string& code);

    //Reading CSV
    vector<PumpingStation> readFromCSV(const string& filename);

    // Destructor
    ~PumpingStation();

    // Getters
    int getId() const;
    std::string getCode() const;

    // Setters
    void setId(int id);
    void setCode(const std::string& code);

private:
    int id;
    std::string code;
};

#endif // PUMPINGSTATION_H
