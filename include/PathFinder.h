#ifndef PATHFINDER_H
#define PATHFINDER_H

#include "Graph.h"
#include "Strategy.h"
#include <vector>
#include <unordered_map>
#include <memory>
#include <chrono>

/**
 * @brief 路径结果结构体
 */
struct PathResult {
    std::vector<int> path;      // 路径节点序列
    double totalWeight;          // 总权重（距离或时间）
    bool found;                  // 是否找到路径
    int64_t elapsedMicroseconds; // 执行时间（微秒）
    
    PathResult() : totalWeight(0.0), found(false), elapsedMicroseconds(0) {}
};

/**
 * @brief 路径规划引擎核心类
 * 使用堆优化的Dijkstra算法实现高性能路径搜索
 */
class PathFinder {
public:
    /**
     * @brief 构造函数
     * @param graph 图的引用
     */
    explicit PathFinder(const Graph& graph);
    
    ~PathFinder();
    
    // 禁止拷贝构造和赋值
    PathFinder(const PathFinder&) = delete;
    PathFinder& operator=(const PathFinder&) = delete;
    
    /**
     * @brief 设置路径规划策略
     * @param strategy 策略指针（PathFinder会管理其生命周期）
     */
    void setStrategy(std::unique_ptr<Strategy> strategy);
    
    /**
     * @brief 查找最短路径
     * @param from 起始节点
     * @param to 目标节点
     * @return 路径结果
     */
    PathResult findPath(int from, int to);
    
    /**
     * @brief 获取当前使用的策略名称
     * @return 策略名称
     */
    const char* getCurrentStrategyName() const;

private:
    /**
     * @brief 堆优化的Dijkstra算法实现
     * @param from 起始节点
     * @param to 目标节点
     * @return 路径结果
     */
    PathResult dijkstra(int from, int to);
    
    /**
     * @brief 重构路径
     * @param from 起始节点
     * @param to 目标节点
     * @param parent 父节点数组
     * @return 路径节点序列
     */
    std::vector<int> reconstructPath(int from, int to, 
                                     const std::unordered_map<int, int>& parent) const;
    
    const Graph& graph_;                    // 图的引用
    std::unique_ptr<Strategy> strategy_;   // 当前策略
    
    // 最小堆节点（用于优先队列）
    struct HeapNode {
        int nodeId;
        double distance;
        
        HeapNode(int id, double dist) : nodeId(id), distance(dist) {}
        
        // 用于最小堆比较
        bool operator>(const HeapNode& other) const {
            return distance > other.distance;
        }
    };
};

#endif // PATHFINDER_H
