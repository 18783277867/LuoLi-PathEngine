#include "../include/Graph.h"
#include <algorithm>

Graph::Graph() : edgeCount_(0) {
}

Graph::~Graph() {
    clear();
}

void Graph::addNode(int nodeId) {
    if (!hasNode(nodeId)) {
        nodes_[nodeId] = true;
        adjacencyList_[nodeId] = std::vector<Edge>();
    }
}

void Graph::addEdge(int from, int to, double weight) {
    // 确保两个节点都存在
    addNode(from);
    addNode(to);
    
    // 添加无向边（双向）
    adjacencyList_[from].emplace_back(to, weight);
    adjacencyList_[to].emplace_back(from, weight);
    
    edgeCount_++;
}

const std::vector<Edge>& Graph::getAdjacentEdges(int nodeId) const {
    static const std::vector<Edge> empty;
    auto it = adjacencyList_.find(nodeId);
    if (it != adjacencyList_.end()) {
        return it->second;
    }
    return empty;
}

bool Graph::hasNode(int nodeId) const {
    return nodes_.find(nodeId) != nodes_.end();
}

size_t Graph::getNodeCount() const {
    return nodes_.size();
}

size_t Graph::getEdgeCount() const {
    return edgeCount_;
}

bool Graph::isIsolated(int nodeId) const {
    if (!hasNode(nodeId)) {
        return false;  // 节点不存在，不算孤立点
    }
    auto it = adjacencyList_.find(nodeId);
    return it == adjacencyList_.end() || it->second.empty();
}

void Graph::clear() {
    adjacencyList_.clear();
    nodes_.clear();
    edgeCount_ = 0;
}
