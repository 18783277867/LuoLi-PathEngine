#include "../include/MapEngine.h"
#include <iostream>
#include <functional>
#include <set>

MapEngine::MapEngine() : currentStrategy_(PathStrategy::SHORTEST_DISTANCE) {
    pathFinder_ = std::make_unique<PathFinder>(graph_);
}

MapEngine::~MapEngine() = default;

bool MapEngine::loadMap(const std::string& filename) {
    // 清空现有数据
    graph_.clear();
    distances_.clear();
    times_.clear();
    
    // 加载数据（带距离和时间信息）
    bool success = dataLoader_.loadFromFileWithDetails(filename, graph_, distances_, times_);
    
    if (success) {
        // 根据当前策略重建图权重
        rebuildGraphWeights();
        
        // 重新创建PathFinder（因为图已经改变）
        pathFinder_ = std::make_unique<PathFinder>(graph_);
        
        // 设置策略
        setStrategy(currentStrategy_);
    }
    
    return success;
}

void MapEngine::setStrategy(PathStrategy strategy) {
    currentStrategy_ = strategy;
    
    std::unique_ptr<Strategy> strategyImpl;
    
    switch (strategy) {
        case PathStrategy::SHORTEST_DISTANCE:
            strategyImpl = std::make_unique<ShortestDistanceStrategy>();
            break;
        case PathStrategy::LEAST_TIME:
            strategyImpl = std::make_unique<LeastTimeStrategy>();
            break;
        default:
            strategyImpl = std::make_unique<ShortestDistanceStrategy>();
            break;
    }
    
    // 重建图权重
    rebuildGraphWeights();
    
    // 重新创建PathFinder
    pathFinder_ = std::make_unique<PathFinder>(graph_);
    pathFinder_->setStrategy(std::move(strategyImpl));
}

void MapEngine::rebuildGraphWeights() {
    // 清空现有边
    graph_.clear();
    
    // 使用集合记录已添加的边，避免重复（无向图）
    std::set<std::pair<int, int>> addedEdges;
    
    // 根据当前策略重新添加边
    for (const auto& distPair : distances_) {
        int from = distPair.first.first;
        int to = distPair.first.second;
        double distance = distPair.second;
        double time = times_[distPair.first];
        
        // 跳过无效边（距离或时间为0或负数）
        if (distance <= 0 || time <= 0) {
            continue;
        }
        
        // 创建边的键（确保from < to，避免重复）
        std::pair<int, int> edgeKey = (from < to) ? 
            std::make_pair(from, to) : std::make_pair(to, from);
        
        // 如果边已添加，跳过
        if (addedEdges.find(edgeKey) != addedEdges.end()) {
            continue;
        }
        
        // 根据策略计算权重
        double weight;
        switch (currentStrategy_) {
            case PathStrategy::SHORTEST_DISTANCE:
                weight = distance;
                break;
            case PathStrategy::LEAST_TIME:
                weight = time;
                break;
            default:
                weight = distance;
                break;
        }
        
        // 添加边（addEdge会自动创建双向连接）
        graph_.addEdge(from, to, weight);
        addedEdges.insert(edgeKey);
    }
}

PathResult MapEngine::findPath(int from, int to) {
    if (!pathFinder_) {
        PathResult result;
        result.found = false;
        return result;
    }
    
    return pathFinder_->findPath(from, to);
}

const char* MapEngine::getCurrentStrategyName() const {
    if (!pathFinder_) {
        return "Unknown";
    }
    return pathFinder_->getCurrentStrategyName();
}

size_t MapEngine::getNodeCount() const {
    return graph_.getNodeCount();
}

size_t MapEngine::getEdgeCount() const {
    return graph_.getEdgeCount();
}

bool MapEngine::isIsolated(int nodeId) const {
    return graph_.isIsolated(nodeId);
}
