#include "../include/PathFinder.h"
#include <queue>
#include <vector>
#include <unordered_map>
#include <limits>
#include <algorithm>
#include <chrono>

PathFinder::PathFinder(const Graph& graph) 
    : graph_(graph), strategy_(std::make_unique<ShortestDistanceStrategy>()) {
}

PathFinder::~PathFinder() = default;

void PathFinder::setStrategy(std::unique_ptr<Strategy> strategy) {
    if (strategy) {
        strategy_ = std::move(strategy);
    }
}

PathResult PathFinder::findPath(int from, int to) {
    // 记录开始时间
    auto start = std::chrono::high_resolution_clock::now();
    
    PathResult result = dijkstra(from, to);
    
    // 计算执行时间（微秒）
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    result.elapsedMicroseconds = duration.count();
    
    return result;
}

PathResult PathFinder::dijkstra(int from, int to) {
    PathResult result;
    
    // 验证节点是否存在
    if (!graph_.hasNode(from) || !graph_.hasNode(to)) {
        result.found = false;
        return result;
    }
    
    // 如果起点和终点相同
    if (from == to) {
        result.path = {from};
        result.totalWeight = 0.0;
        result.found = true;
        return result;
    }
    
    // 如果起点是孤立点
    if (graph_.isIsolated(from)) {
        result.found = false;
        return result;
    }
    
    // 距离数组：nodeId -> 最短距离（动态添加节点）
    std::unordered_map<int, double> dist;
    // 父节点数组：用于重构路径
    std::unordered_map<int, int> parent;
    // 已访问标记
    std::unordered_map<int, bool> visited;
    
    // 初始化起点距离为0
    dist[from] = 0.0;
    visited[from] = false;
    
    // 最小堆（优先队列）：使用自定义比较函数
    auto cmp = [](const HeapNode& a, const HeapNode& b) {
        return a.distance > b.distance;  // 最小堆
    };
    std::priority_queue<HeapNode, std::vector<HeapNode>, decltype(cmp)> pq(cmp);
    
    pq.push(HeapNode(from, 0.0));
    
    while (!pq.empty()) {
        HeapNode current = pq.top();
        pq.pop();
        
        int u = current.nodeId;
        double d = current.distance;
        
        // 如果已经访问过，跳过（处理重复节点）
        if (visited[u]) {
            continue;
        }
        
        visited[u] = true;
        
        // 如果到达目标节点
        if (u == to) {
            break;
        }
        
        // 遍历邻接边
        const auto& edges = graph_.getAdjacentEdges(u);
        for (const Edge& edge : edges) {
            int v = edge.to;
            double weight = edge.weight;
            
            // 如果节点v还没有被访问过，初始化其距离
            if (dist.find(v) == dist.end()) {
                dist[v] = std::numeric_limits<double>::max();
                visited[v] = false;
            }
            
            // 使用策略计算权重（这里权重已经在图中按策略计算好了）
            // 但为了灵活性，我们仍然支持策略接口
            double newDist = d + weight;
            
            if (!visited[v] && newDist < dist[v]) {
                dist[v] = newDist;
                parent[v] = u;
                pq.push(HeapNode(v, newDist));
            }
        }
    }
    
    // 检查是否找到路径
    if (dist.find(to) == dist.end() || dist[to] == std::numeric_limits<double>::max()) {
        result.found = false;
        return result;
    }
    
    // 重构路径
    result.path = reconstructPath(from, to, parent);
    result.totalWeight = dist[to];
    result.found = true;
    
    return result;
}

std::vector<int> PathFinder::reconstructPath(int from, int to, 
                                             const std::unordered_map<int, int>& parent) const {
    std::vector<int> path;
    
    if (from == to) {
        path.push_back(from);
        return path;
    }
    
    // 从终点回溯到起点
    int current = to;
    while (current != from) {
        path.push_back(current);
        auto it = parent.find(current);
        if (it == parent.end()) {
            // 路径不完整，返回空路径
            return std::vector<int>();
        }
        current = it->second;
    }
    path.push_back(from);
    
    // 反转路径（从起点到终点）
    std::reverse(path.begin(), path.end());
    
    return path;
}

const char* PathFinder::getCurrentStrategyName() const {
    return strategy_ ? strategy_->getName() : "Unknown";
}
