#ifndef MAPENGINE_H
#define MAPENGINE_H

#include "Graph.h"
#include "PathFinder.h"
#include "DataLoader.h"
#include "Strategy.h"
#include "PairHash.h"
#include <string>
#include <memory>
#include <unordered_map>
#include <utility>
#include <functional>

/**
 * @brief 地图引擎核心类
 * 封装数据读取、图构建与路径搜索模块，支持多策略动态切换
 */
class MapEngine {
public:
    MapEngine();
    ~MapEngine();
    
    // 禁止拷贝构造和赋值
    MapEngine(const MapEngine&) = delete;
    MapEngine& operator=(const MapEngine&) = delete;
    
    /**
     * @brief 从文件加载地图数据
     * @param filename 数据文件路径
     * @return 是否加载成功
     */
    bool loadMap(const std::string& filename);
    
    /**
     * @brief 设置路径规划策略
     * @param strategy 策略类型
     */
    void setStrategy(PathStrategy strategy);
    
    /**
     * @brief 查找路径
     * @param from 起始节点
     * @param to 目标节点
     * @return 路径结果
     */
    PathResult findPath(int from, int to);
    
    /**
     * @brief 获取当前策略名称
     * @return 策略名称
     */
    const char* getCurrentStrategyName() const;
    
    /**
     * @brief 获取图中节点数量
     * @return 节点数量
     */
    size_t getNodeCount() const;
    
    /**
     * @brief 获取图中边的数量
     * @return 边的数量
     */
    size_t getEdgeCount() const;
    
    /**
     * @brief 检查节点是否为孤立点
     * @param nodeId 节点ID
     * @return 是否为孤立点
     */
    bool isIsolated(int nodeId) const;

private:
    Graph graph_;
    std::unique_ptr<PathFinder> pathFinder_;
    DataLoader dataLoader_;
    
    // 存储距离和时间信息（用于策略切换）
    std::unordered_map<std::pair<int, int>, double, 
                      std::hash<std::pair<int, int>>> distances_;
    std::unordered_map<std::pair<int, int>, double,
                      std::hash<std::pair<int, int>>> times_;
    
    PathStrategy currentStrategy_;
    
    /**
     * @brief 根据当前策略重建图的权重
     */
    void rebuildGraphWeights();
};

#endif // MAPENGINE_H
