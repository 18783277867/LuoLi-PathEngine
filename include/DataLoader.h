#ifndef DATALOADER_H
#define DATALOADER_H

#include "Graph.h"
#include "PairHash.h"
#include <string>
#include <memory>
#include <unordered_map>
#include <utility>
#include <functional>

/**
 * @brief 数据加载器类
 * 负责从文件读取地图数据并构建图
 */
class DataLoader {
public:
    DataLoader();
    ~DataLoader();
    
    // 禁止拷贝构造和赋值
    DataLoader(const DataLoader&) = delete;
    DataLoader& operator=(const DataLoader&) = delete;
    
    /**
     * @brief 从文件加载数据并构建图
     * 文件格式：每行一条边，格式为 "from to distance time"
     * @param filename 文件名
     * @param graph 要填充的图对象
     * @return 是否加载成功
     */
    bool loadFromFile(const std::string& filename, Graph& graph);
    
    /**
     * @brief 从文件加载数据并构建图（带距离和时间信息）
     * @param filename 文件名
     * @param graph 要填充的图对象
     * @param distances 输出的距离映射 (from,to) -> distance
     * @param times 输出的时间映射 (from,to) -> time
     * @return 是否加载成功
     */
    bool loadFromFileWithDetails(const std::string& filename, 
                                 Graph& graph,
                                 std::unordered_map<std::pair<int, int>, double, 
                                                   std::hash<std::pair<int, int>>>& distances,
                                 std::unordered_map<std::pair<int, int>, double,
                                                   std::hash<std::pair<int, int>>>& times);
    
    /**
     * @brief 获取最后加载的节点数量
     * @return 节点数量
     */
    size_t getLoadedNodeCount() const;
    
    /**
     * @brief 获取最后加载的边数量
     * @return 边数量
     */
    size_t getLoadedEdgeCount() const;

private:
    size_t lastNodeCount_;
    size_t lastEdgeCount_;
    
    /**
     * @brief 解析一行数据
     * @param line 输入行
     * @param from 输出的起始节点
     * @param to 输出的目标节点
     * @param distance 输出的距离
     * @param time 输出的时间
     * @return 是否解析成功
     */
    bool parseLine(const std::string& line, int& from, int& to, 
                   double& distance, double& time);
};

#endif // DATALOADER_H
