
#include "VertexEdge.h"
#include "VertexEdge.h"

/************************* Vertex  **************************/

Vertex::Vertex(const Reservoir& reservoir) : id(reservoir.getId()) {}

Vertex::Vertex(const PumpingStation& pumpingStation) : id(pumpingStation.getId()) {}

Vertex::Vertex(const City& city) : id(city.getId()) {}

Edge* Vertex::addEdge(Vertex* dest, int capacity, int direction) {
    auto newEdge = new Edge(this, dest, capacity, direction);
    adj.push_back(newEdge);
    dest->incoming.push_back(newEdge);
    return newEdge;
}


bool Vertex::operator<(Vertex & vertex) const {
    return this->dist < vertex.dist;
}

int Vertex::getId() const {
    return this->id;
}

const vector<Edge *> Vertex::getAdj() const {
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

void Vertex::setVisited(bool visited) {
    this->visited = visited;
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