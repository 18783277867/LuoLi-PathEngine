#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include <unordered_map>
#include <memory>
#include <cstdint>

/**
 * @brief 边结构体，表示图中的一条边
 */
struct Edge {
    int to;           // 目标节点ID
    double weight;    // 边的权重（距离或时间）
    
    Edge(int t, double w) : to(t), weight(w) {}
};

/**
 * @brief 无向带权图类
 * 使用邻接表存储，优化大规模节点检索性能
 */
class Graph {
public:
    Graph();
    ~Graph();
    
    // 禁止拷贝构造和赋值
    Graph(const Graph&) = delete;
    Graph& operator=(const Graph&) = delete;
    
    /**
     * @brief 添加节点
     * @param nodeId 节点ID
     */
    void addNode(int nodeId);
    
    /**
     * @brief 添加无向边
     * @param from 起始节点
     * @param to 目标节点
     * @param weight 边的权重
     */
    void addEdge(int from, int to, double weight);
    
    /**
     * @brief 获取节点的邻接边列表
     * @param nodeId 节点ID
     * @return 邻接边列表的引用
     */
    const std::vector<Edge>& getAdjacentEdges(int nodeId) const;
    
    /**
     * @brief 检查节点是否存在
     * @param nodeId 节点ID
     * @return 是否存在
     */
    bool hasNode(int nodeId) const;
    
    /**
     * @brief 获取图中节点总数
     * @return 节点数量
     */
    size_t getNodeCount() const;
    
    /**
     * @brief 获取图中边的总数
     * @return 边的数量
     */
    size_t getEdgeCount() const;
    
    /**
     * @brief 检查节点是否为孤立点
     * @param nodeId 节点ID
     * @return 是否为孤立点
     */
    bool isIsolated(int nodeId) const;
    
    /**
     * @brief 清空图
     */
    void clear();

private:
    // 邻接表：nodeId -> 邻接边列表
    std::unordered_map<int, std::vector<Edge>> adjacencyList_;
    
    // 节点集合，用于快速查找
    std::unordered_map<int, bool> nodes_;
    
    // 边的数量（无向边算一条）
    size_t edgeCount_;
};

#endif // GRAPH_H
