#ifndef OKDA_VERTEXEDGE_H
#define OKDA_VERTEXEDGE_H

#include <iostream>
#include <vector>
#include <queue>
#include <limits>
#include <algorithm>
#include "Reservoir.h"
#include "PumpingStation.h"
#include "City.h"


class Edge;

#define INF std::numeric_limits<double>::max()

/************************* Vertex **************************/


enum class Type {
    CITY,
    RESERVOIR,
    PUMPING_STATION,
    SUPER_SOURCE,
    SUPER_SINK
};

class Vertex {
public:

    friend class Algorithms;
    explicit Vertex(const std::string& code, Type type);
    explicit Vertex(const Reservoir& reservoir, Type type);
    explicit Vertex(const PumpingStation& pumpingStation, Type type);
    explicit Vertex(const City& city, Type type);

    bool operator<(Vertex & vertex) const;

    std::string getCode() const;

    double getMaxFlow() const;

    Type getType()const;

    vector<Edge *> getAdj() const;

    void setAdj(const std::vector<Edge*>& adjacencyList);

    double getFlow();

    void setFlow(double flow);

    bool isVisited() const;

    Edge *getPath() const;

    std::vector<Edge *> getIncoming() const;

    void setVisited(bool visited);

    void setPath(Edge *path);

    Edge * addEdge(Vertex* dest, int capacity, int direction);
    Edge *addBidirectionalEdge(Vertex *dest, int capacity, int direction);

    bool operator==(const Vertex& other) {
        return this->code == other.code;
    }

protected:

    std::string code;

    double flow;

    double maxFlow;

    std::vector<Edge *> adj;

    bool visited = false;

    double dist = 0;

    Edge *path = nullptr;

    std::vector<Edge *> incoming;

    Type type;
};

/********************** Edge ****************************/

/**
 * @brief Class that defines a graph's edge
 */

class Edge {
public:

    Edge(Vertex* orig, Vertex* dest, int capacity, int direction);

    Vertex * getDest() const;

    int getWeight() const;

    void setCapacity(int newCapacity);

    Vertex * getOrig() const;

    int getCapacity() const;

    int getDirection() const;

    Edge *getReverse() const;

    double getFlow() const;

    void setReverse(Edge *reverse);

    void setFlow(double flow);

    void setWeight(int weight);

protected:
    Vertex * dest;

    int weight;

    bool selected = false;

    Vertex *orig;

    Edge *reverse = nullptr;

    int direction;

    int capacity;

    double flow;
};

#endif //OKDA_VERTEXEDGE_H
