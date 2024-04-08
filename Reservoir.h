#ifndef RESERVOIR_H
#define RESERVOIR_H

#include <string>
#include <vector>

using namespace std;

class Reservoir {
public:
    // Constructors
    Reservoir();
    Reservoir(const std::string& name, const std::string& municipality, int id, const std::string& code, int maxDelivery);

    // Destructor
    ~Reservoir();

    //Reading CSV
    vector<Reservoir> readFromCSV(const string& filename);

    // Getters
    string getName() const;
    string getMunicipality() const;
    int getId() const;
    string getCode() const;
    int getMaxDelivery() const;

    // Setters
    void setName(const std::string& name);
    void setMunicipality(const std::string& municipality);
    void setId(int id);
    void setCode(const std::string& code);
    void setMaxDelivery(int maxDelivery);

private:
    string name;
    string municipality;
    int id;
    string code;
    int maxDelivery;
};

#endif // RESERVOIR_H
