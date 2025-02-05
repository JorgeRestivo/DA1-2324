#include "Graph.h"

Vertex * Graph::findVertex(const std::string &code) const {
    auto iter = vertexMap.find(code);
    if (iter != vertexMap.end()) {
        return iter->second;
    } else {
        return nullptr;
    }
}

bool Graph::addVertex(const Reservoir& reservoir) {
    if (findVertex(reservoir.getCode()) != nullptr)
        return false;
    vertexMap.insert({reservoir.getCode(), new Vertex(reservoir, Type::RESERVOIR)});
    return true;
}

// Implement addVertex function for PumpingStation
bool Graph::addVertex(const PumpingStation& pumpingStation) {
    if (findVertex(pumpingStation.getCode()) != nullptr)
        return false;
    vertexMap.insert({pumpingStation.getCode(), new Vertex(pumpingStation, Type::PUMPING_STATION)});
    return true;
}

// Implement addVertex function for City
bool Graph::addVertex(const City& city) {
    if (findVertex(city.getCode()) != nullptr)
        return false;
    vertexMap.insert({city.getCode(), new Vertex(city, Type::CITY)});
    return true;
}

bool Graph::addVertex(Vertex* vertex) {
    if (!vertex || findVertex(vertex->getCode()) != nullptr)
        return false;
    vertexMap.insert({vertex->getCode(), vertex});
    return true;
}

void deleteMatrix(int **m, int n) {
    if (m != nullptr) {
        for (int i = 0; i < n; i++)
            if (m[i] != nullptr)
                delete [] m[i];
        delete [] m;
    }
}

void deleteMatrix(double **m, int n) {
    if (m != nullptr) {
        for (int i = 0; i < n; i++)
            if (m[i] != nullptr)
                delete [] m[i];
        delete [] m;
    }
}

void Graph::removeVertex(Vertex* vertex) {
    // Erase the vertex from the vertex map
    auto it = vertexMap.find(vertex->getCode());
    if (it != vertexMap.end()) {
        vertexMap.erase(it);
    }

    // Remove edges connected to the vertex being removed from other vertices' adjacency lists
    for (auto& [code, v] : vertexMap) {
        // Get the list of adjacent vertices (neighbors) of the current vertex
        auto adj = v->getAdj(); // Pass by value instead of by reference

        // Remove the edges connected to the vertex being deleted from the adjacency list
        adj.erase(std::remove_if(adj.begin(), adj.end(), [vertex](Edge* e) {
            return e->getDest() == vertex || e->getOrig() == vertex;
        }), adj.end());

        // Assign the modified adjacency list back to the current vertex
        v->setAdj(adj);
    }

    // Finally, delete the vertex object
    delete vertex;
}




double Graph::edmondsKarp(Vertex* s, Vertex* t) {
    if (s == nullptr || t == nullptr || s == t)
        throw std::logic_error("Invalid source and/or target vertex");

    for (const auto& v : vertexMap) {
        for (auto e : v.second->getAdj()) {
            e->setFlow(0);
        }
    }

    double maxFlow = 0;
    while (findAugmentingPath(s, t)) {
        double f = findMinResidualAlongPath(s, t);
        augmentFlowAlongPath(s, t, f);
        maxFlow += f;
    }

    // Calculate and save incoming flow for each city vertex
    for (const auto& [key, v] : vertexMap) {
        if (v->getType() == Type::CITY) {
            double incomingFlow = 0;
            for (auto e : v->getIncoming()) {
                incomingFlow += e->getFlow();
            }
            v->setFlow(incomingFlow);  // Assuming Vertex class has a setFlow method
        }
    }


    return maxFlow;
}


void Graph::testAndVisit(std::queue< Vertex*> &q, Edge *e, Vertex *w, double residual) {
    if (! w->isVisited() && residual > 0) {
        w->setVisited(true);
        w->setPath(e);
        q.push(w);
    }
}

bool Graph::findAugmentingPath(Vertex *s, Vertex *t) {
    for(auto v : vertexMap) {
        v.second->setVisited(false);
    }
    s->setVisited(true);
    std::queue<Vertex *> q;
    q.push(s);
    while( ! q.empty() && ! t->isVisited()) {
        auto v = q.front();
        q.pop();
        for(auto e: v->getAdj()) {
            testAndVisit(q, e, e->getDest(), e->getCapacity() - e->getFlow());
        }
        for(auto e: v->getIncoming()) {
            testAndVisit(q, e, e->getOrig(), e->getFlow());
        }
    }
    return t->isVisited();
}

double Graph::findMinResidualAlongPath(Vertex *s, Vertex *t) {
    double f = INF;
    for (auto v = t; v != s; ) {
        auto e = v->getPath();
        if (e->getDest() == v) {
            f = std::min(f, static_cast<double>(e->getCapacity() - e->getFlow()));
            v = e->getOrig();
        }
        else {
            f = std::min(f, static_cast<double>(e->getFlow()));
            v = e->getDest();
        }
    }

    return f;
}

void Graph::augmentFlowAlongPath(Vertex *s, Vertex *t, double f) {
    for (auto v = t; v != s; ) {
        auto e = v->getPath();
        double flow = e->getFlow();
        if (e->getDest() == v) {
            e->setFlow(flow + f);
            v = e->getOrig();
        }
        else {
            e->setFlow(flow - f);
            v = e->getDest();
        }
    }
}

double Graph::getMaxFlowToCity(const std::string& cityCode) {
    Vertex* cityVertex = findVertex(cityCode);
    if (cityVertex) {
        return cityVertex->getFlow();  // Assuming getFlow returns the max flow for the city
    } else {
        throw std::invalid_argument("City not found in the graph");
    }
}

void Graph::resetFlows(Graph& graph) {
    for (auto& [_, vertex] : graph.vertexMap) {
        for (auto& edge : vertex->getAdj()) {
            edge->setFlow(0);
        }
    }
}

bool Graph::bfs(Vertex *source, Vertex* sink) const {
    for (auto& vertex : vertexMap) {
        Vertex* v = vertex.second;
        v->setVisited(false);
        v->setPath(nullptr);
    }

    std::queue<Vertex*> queue;

    source->setVisited(true);
    queue.push(source);

    while (!queue.empty()) {
        Vertex* v = queue.front();
        queue.pop();

        for (Edge* e : v->getAdj()) {
            Vertex* w = e->getDest();
            if (!w->isVisited() && e->getWeight() > 0) {
                w->setVisited(true);
                w->setPath(e);
                if (w == sink) {
                    return true;
                }
                queue.push(w);
            }
        }

    }

    return false;
}

void Graph::updateFlow(Vertex* sink, int bottleneck) {
    for (Vertex *v = sink; v->getPath() != nullptr; v = v->getPath()->getOrig()) {
        Edge *e = v->getPath();
        e->setWeight(e->getWeight() - bottleneck);
        Edge *reverse = e->getReverse();
        if (reverse != nullptr) {
            reverse->setWeight(reverse->getWeight() + bottleneck);
        }
    }
}


Graph::~Graph() {
    deleteMatrix(distMatrix, (int) vertexMap.size());
    deleteMatrix(pathMatrix, (int) vertexMap.size());
}

const unordered_map<std::string, Vertex *> & Graph::getVertexMap() const {
    return this->vertexMap;
}

void Graph::printGraph(const Graph& graph) {
    const auto& vertexMap = graph.getVertexMap();

    for (const auto& entry : vertexMap) {
        const std::string& vertexName = entry.first;
        const Vertex* vertex = entry.second;

        std::cout << "Vertex: " << vertexName << ", Adj Size: " << vertex->getIncoming().size() << std::endl;

        const auto& edges = vertex->getAdj();
        for (const auto& edge : edges) {
            std::cout << "    Edge to: " << edge->getDest()->getCode() << ", Weight: " << edge->getWeight() << std::endl;
        }
    }
}
