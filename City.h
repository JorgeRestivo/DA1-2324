#ifndef CITY_H
#define CITY_H

#include <string>
#include <vector>

using namespace std;

class City {
public:
    // Constructors
    City();
    City(const string& name, int id, const string& code, double demand, const string& population);

    vector<City> readFromCSV(const string& filename);

    // Destructor
    ~City();

    // Getters
    string getName() const;
    int getId() const;
    string getCode() const;
    double getDemand() const;
    string getPopulation() const;

    // Setters
    void setName(const string& name);
    void setId(int id);
    void setCode(const string& code);
    void setDemand(double demand);
    void setPopulation(int population);

private:
    string name;
    int id;
    string code;
    double demand;
    string population;
};

#endif // CITY_H

