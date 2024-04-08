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
    vertexMap.insert({reservoir.getCode(), new Vertex(reservoir)});
    return true;
}

// Implement addVertex function for PumpingStation
bool Graph::addVertex(const PumpingStation& pumpingStation) {
    if (findVertex(pumpingStation.getCode()) != nullptr)
        return false;
    vertexMap.insert({pumpingStation.getCode(), new Vertex(pumpingStation)});
    return true;
}

// Implement addVertex function for City
bool Graph::addVertex(const City& city) {
    if (findVertex(city.getCode()) != nullptr)
        return false;
    vertexMap.insert({city.getCode(), new Vertex(city)});
    return true;
}

/*bool Graph::addBidirectionalEdge(const Station &sourc, const Station &dest, const Connection& w) const {
    auto v1 = findVertex(sourc.getName());
    auto v2 = findVertex(dest.getName());
    if (v1 == nullptr || v2 == nullptr)
        return false;
    auto e1 = v1->addEdge(v2, w.getCapacity(), w.getService());
    auto e2 = v2->addEdge(v1, w.getCapacity(), w.getService());
    e1->setReverse(e2);
    e2->setReverse(e1);
    return true;
}*/

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

double Graph::edmondsKarp(Vertex* s, Vertex* t) {
    if (s == nullptr || t == nullptr ||  s == t)
        throw std::logic_error("Invalid source and/or target vertex");

    for (const auto& v : vertexMap) {
        for (auto e: v.second->getAdj()) {
            e->setFlow(0);
        }
    }

    double maxFlow = 0;
    while(findAugmentingPath(s, t)) {
        double f = findMinResidualAlongPath(s, t);
        augmentFlowAlongPath(s, t, f);
        maxFlow += f;
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
            testAndVisit(q, e, e->getDest(), e->getWeight() - e->getFlow());
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
            f = std::min(f, e->getWeight() - e->getFlow());
            v = e->getOrig();
        }
        else {
            f = std::min(f, e->getFlow());
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

int Graph::maxFlow(Vertex* startStation, Vertex* endStation) {
    double maxFlow = 0;
    while (maxFlow == 0) {
        double newFlow = edmondsKarp(startStation, endStation);
        if (newFlow == 0) {
            break;
        }
        maxFlow += newFlow;
    }
    return (int) maxFlow;
}

/*
vector<vector<int>> Graph::dfs_graphCost(Graph &graph, Vertex &startStation, Vertex &endStation) {
    vector<vector<int>> results;
    vector<string> path;
    double maxFlowSoFar = 0;
    dfs_graphCostHelper(graph, startStation, endStation, path, results, maxFlowSoFar);
    return results;
}*/

/*void Graph::dfs_graphCostHelper(Graph &graph, Vertex &current, Vertex &endStation, vector<string> &path, vector<vector<int>> &results, double &maxFlowSoFar) {
    current.setVisited(true);
    path.push_back(current.getName());


    if (current == endStation) {
        int numEdges = (int) path.size() - 1;
        int numAlpha = 0;
        int numStandard = 0;
        int maxFlow = INT_MAX;

        for (unsigned int i = 0; i < path.size() - 1; i++) {
            Vertex *u = graph.findVertex(path[i]);
            Vertex *v = graph.findVertex(path[i+1]);

            for (auto e : u->getAdj()) {
                if (e->getDest() == v) {
                    if (e->isServiceTypeAlpha()) {
                        numAlpha++;
                    } else {
                        numStandard++;
                    }
                    maxFlow = min(maxFlow, e->getWeight());
                    break;
                }
            }
        }

        maxFlowSoFar = max(maxFlowSoFar, static_cast<double>(maxFlow));

        results.push_back({maxFlow, numEdges, numAlpha, numStandard});

    } else {
        for (auto e : current.getAdj()) {
            Vertex *next = e->getDest();
            if (!next->isVisited()) {
                dfs_graphCostHelper(graph, *next, endStation, path, results, maxFlowSoFar);
            }
        }
    }

    current.setVisited(false);
    path.pop_back();
}*/

/*
vector<int> Graph::graphCost(Graph & graph, Vertex startStation, Vertex endStation) {
    vector<vector<int>> results;
    vector<int> final;
    int totalCost = INT_MAX;
    results = dfs_graphCost(graph, startStation, endStation);

    sort(results.begin(), results.end(), greater<vector<int>>());

    int maxFlow = INT_MIN;
    for (auto elem: results) {
        maxFlow = max(maxFlow, elem[0]);
        if (elem[0] < maxFlow || (elem[2] + elem[3] == 0)) {
            break;
        }
        int temp = elem[2] * 4 + elem[3] * 2; /// SMALL MISTAKE IN DELIVERED VERSION
        if (temp < totalCost) {
            totalCost = temp;
            final = elem;
        }
    }
    if (final.empty()) final = {0, 0, 0, 0};
    final.push_back(totalCost);
    return final;
}
*/
/*
int Graph::getMaxTrains(const std::string &stationName) const {
    Vertex* targetStation = findVertex(stationName);
    if (!targetStation) {
        return -1;
    }

    int maxFlow = 0;

    for (auto const& vertex : vertexMap) {
        Vertex* source = vertex.second;
        if (source != targetStation) {
            maxFlow += fordFulkerson(source, targetStation);
        }
    }

    return maxFlow;
}*/

int Graph::fordFulkerson(Vertex *source, Vertex *target) const {
    int maxFlow = 0;

    while (bfs(source, target)) {
        int bottleneck = getBottleneckCapacity(target);

        updateFlow(target, bottleneck);

        maxFlow += bottleneck;
    }
    return maxFlow;

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

int Graph::getBottleneckCapacity(Vertex* sink) {
    int bottleneck = INT_MAX;

    for (Vertex* v = sink; v->getPath() != nullptr; v = v->getPath()->getOrig()) {
        Edge* e = v->getPath();
        bottleneck = min(bottleneck, e->getWeight());
    }

    return bottleneck;
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

