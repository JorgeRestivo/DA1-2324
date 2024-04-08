
#include "VertexEdge.h"
#include "VertexEdge.h"

/************************* Vertex  **************************/

Vertex::Vertex(const std::string& code, Type type) : code(code), type(type) {}

Vertex::Vertex(const Reservoir& reservoir, Type type) : code(reservoir.getCode()), type(type) {}

Vertex::Vertex(const PumpingStation& pumpingStation, Type type) : code(pumpingStation.getCode()), type(type) {}

Vertex::Vertex(const City& city, Type type) : code(city.getCode()), type(type) {}

Edge* Vertex::addEdge(Vertex* dest, int capacity, int direction) {
    auto newEdge = new Edge(this, dest, capacity, direction);
    adj.push_back(newEdge);
    dest->incoming.push_back(newEdge);
    return newEdge;
}

Edge* Vertex::addBidirectionalEdge(Vertex* dest, int capacity, int direction) {
    // Add edge from current vertex to destination vertex
    Edge* edge1 = addEdge(dest, capacity, direction);
    // Add edge from destination vertex to current vertex (if bidirectional)
    if (direction == 0) {
        dest->addEdge(this, capacity, direction);
    }
    return edge1;
}



bool Vertex::operator<(Vertex & vertex) const {
    return this->dist < vertex.dist;
}

std::string Vertex::getCode() const {
    return this->code;
}

vector<Edge *> Vertex::getAdj() const {
    return this->adj;
}

bool Vertex::isVisited() const {
    return this->visited;
}

Edge *Vertex::getPath() const {
    return this->path;
}

std::vector<Edge *> Vertex::getIncoming() const {
    return this->incoming;
}

Type Vertex::getType() const{
    return this->type;
}

double Vertex::getMaxFlow() const {
    return maxFlow;
}

double Vertex::getFlow() {
    return flow;
}

void Vertex::setVisited(bool visited) {
    this->visited = visited;
}

void Vertex::setAdj(const std::vector<Edge*>& adjacencyList) {
    adj = adjacencyList;
}

void Vertex::setFlow(double flow) {
    this->flow = flow;
}

void Vertex::setPath(Edge *path) {
    this->path = path;
}

/********************** Edge  ****************************/

Edge::Edge(Vertex* orig, Vertex* dest, int capacity, int direction)
        : orig(orig), dest(dest), capacity(capacity), direction(direction) {}

Vertex * Edge::getDest() const {
    return this->dest;
}

int Edge::getWeight() const {
    return this->weight;
}

Vertex * Edge::getOrig() const {
    return this->orig;
}

Edge *Edge::getReverse() const {
    return this->reverse;
}

double Edge::getFlow() const {
    return flow;
}

int Edge::getCapacity() const {
    return capacity;
}

void Edge::setCapacity(int newCapacity) {
    capacity = newCapacity;
}

int Edge::getDirection() const {
    return direction;
}

void Edge::setReverse(Edge *reverse) {
    this->reverse = reverse;
}

void Edge::setFlow(double flow) {
    this->flow = flow;
}

void Edge::setWeight(int weight) {
    this->weight = weight;
}