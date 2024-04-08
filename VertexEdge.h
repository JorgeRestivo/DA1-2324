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

/**
 * @brief Class that defines a graph's vertex
 */

enum class Type {
    CITY,
    RESERVOIR,
    PUMPING_STATION,
    SUPER_SOURCE,
    SUPER_SINK
};

class Vertex {
public:
    /**
     * @brief Constructor of the class, makes a vertex out of a Station object
     * @param station
     */
    friend class Algorithms;
    explicit Vertex(const std::string& code, Type type);
    explicit Vertex(const Reservoir& reservoir, Type type);
    explicit Vertex(const PumpingStation& pumpingStation, Type type);
    explicit Vertex(const City& city, Type type);

    /**
     * @brief Operator < overload
     * @param vertex
     * @return
     */
    bool operator<(Vertex & vertex) const;

    /**
     * @brief Getter for Vertexe's identifier (Station's name)
     * @returns the station's name of the vertex it represents
     */
    std::string getCode() const;

    double getMaxFlow() const;

    Type getType()const;
    /**
     * @brief Getter for the vertex's adjacency list
     * @returns vector of edges representing the adjacency list of vertex in question
     */
    vector<Edge *> getAdj() const;

    void setAdj(const std::vector<Edge*>& adjacencyList);

    double getFlow();

    void setFlow(double flow);


    /**
     * @brief Signals if a determined vertex has already been visited or not in the context of a search algorithm.
     * @returns true if the vertex has already been visited and false otherwise
     */
    bool isVisited() const;

    /**
     * @brief Getter for a Vertexe's path
     * @returns the path from a Vertex do another Vertex defined by an Edge
     */
    Edge *getPath() const;

    /**
     * @brief Getter for incoming edges to the current vertex
     * @returns a vector with all the edges that connect vertexes whose destination is the current vertex
     */
    std::vector<Edge *> getIncoming() const;

    /**
     * @brief Sets a vertex as visited. Used in searching algorithms
     * @param visited
     */
    void setVisited(bool visited);

    /**
     * @brief Sets a vertexe's path
     * @param path
     */
    void setPath(Edge *path);

    /**
     * @brief Auxiliary function to add an outgoing edge to a vertex (this),with a given destination vertex (d) and edge weight (w)
     * @details Time Complexity: O(1)
     * @param dest
     * @param w
     * @param s
     * @returns a pointer to the new edge created
     */
    Edge * addEdge(Vertex* dest, int capacity, int direction);
    Edge *addBidirectionalEdge(Vertex *dest, int capacity, int direction);

    /**
     * @brief Operator == Overload
     */
    bool operator==(const Vertex& other) {
        return this->code == other.code;
    }

protected:

    /// Identifier
    std::string code;

    double flow;

    double maxFlow;

    /// Outgoing edges
    std::vector<Edge *> adj;

    /// Auxiliary fields

    /// marks if a vertex has been visited or not
    bool visited = false;

    /// marks the distance between the current vertex and another vertex
    double dist = 0;

    /// A path from a vertex to another
    Edge *path = nullptr;

    /// incoming edges
    std::vector<Edge *> incoming;

    /// required by MutablePriorityQueue and UFDS
    int queueIndex = 0;

    Type type;
};

/********************** Edge ****************************/

/**
 * @brief Class that defines a graph's edge
 */

class Edge {
public:

    /**
     * @brief Constructor of the class, makes an adge out of two vertexes and a specified service type associated with the line segment the edge defines
     * @param orig
     * @param dest
     * @param w
     * @param service
     */
    Edge(Vertex* orig, Vertex* dest, int capacity, int direction);

    /**
     * @brief Getter for destination station
     * @returns the vertex that represents the destination station
     */
    Vertex * getDest() const;

    /**
     * @brief Getter for edge weight
     * @returns the value that correspond's to the current edge's weight
     */
    int getWeight() const;

    void setCapacity(int newCapacity);

    /**
     * @brief Getter for origin station
     * @returns the vertex that represents the source station
     */
    Vertex * getOrig() const;

    int getCapacity() const;

    int getDirection() const;

    /**
     * @brief Getter for the inverted edge of the current edge.
     * @returns The current edge but with the opposite direction (swapped destination and source stations)
     */
    Edge *getReverse() const;

    /**
     * @brief Getter for an edge's (line segment) flow
     * @returns the value of the flow
     */
    double getFlow() const;

    /**
     * @brief Sets the reverse edge
     * @param reverse
     */
    void setReverse(Edge *reverse);

    /**
     * @brief Set's the line segment's flow
     * @param flow
     */
    void setFlow(double flow);

    /**
     * @brief Set's the line segment's weight
     * @param weight
     */
    void setWeight(int weight);

protected:
    /// destination vertex
    Vertex * dest;

    /// edge weight, can also be used for capacity
    int weight;

    /// auxiliary fields
    bool selected = false;

    /// used for bidirectional edges
    Vertex *orig;

    /// The reverse edge of the current one
    Edge *reverse = nullptr;

    /// type of direction
    int direction;

    int capacity;

    /// for flow-related problems
    double flow;
};

#endif //OKDA_VERTEXEDGE_H
