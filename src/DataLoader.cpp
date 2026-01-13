#include "../include/DataLoader.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <functional>

DataLoader::DataLoader() : lastNodeCount_(0), lastEdgeCount_(0) {
}

DataLoader::~DataLoader() = default;

bool DataLoader::loadFromFile(const std::string& filename, Graph& graph) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "错误：无法打开文件 " << filename << std::endl;
        return false;
    }
    
    graph.clear();
    lastNodeCount_ = 0;
    lastEdgeCount_ = 0;
    
    std::string line;
    int lineNum = 0;
    
    while (std::getline(file, line)) {
        lineNum++;
        
        // 跳过空行和注释行
        if (line.empty() || line[0] == '#') {
            continue;
        }
        
        int from, to;
        double distance, time;
        
        if (parseLine(line, from, to, distance, time)) {
            // 使用距离作为默认权重（最短距离策略）
            graph.addEdge(from, to, distance);
            lastEdgeCount_++;
        } else {
            std::cerr << "警告：第 " << lineNum << " 行格式错误，已跳过" << std::endl;
        }
    }
    
    lastNodeCount_ = graph.getNodeCount();
    file.close();
    
    std::cout << "成功加载数据：" << std::endl;
    std::cout << "  节点数: " << lastNodeCount_ << std::endl;
    std::cout << "  边数: " << lastEdgeCount_ << std::endl;
    
    return true;
}

bool DataLoader::loadFromFileWithDetails(const std::string& filename, 
                                        Graph& graph,
                                        std::unordered_map<std::pair<int, int>, double,
                                                          std::hash<std::pair<int, int>>>& distances,
                                        std::unordered_map<std::pair<int, int>, double,
                                                          std::hash<std::pair<int, int>>>& times) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "错误：无法打开文件 " << filename << std::endl;
        return false;
    }
    
    graph.clear();
    distances.clear();
    times.clear();
    lastNodeCount_ = 0;
    lastEdgeCount_ = 0;
    
    std::string line;
    int lineNum = 0;
    
    while (std::getline(file, line)) {
        lineNum++;
        
        // 跳过空行和注释行
        if (line.empty() || line[0] == '#') {
            continue;
        }
        
        int from, to;
        double distance, time;
        
        if (parseLine(line, from, to, distance, time)) {
            // 跳过无效边（距离或时间为0或负数）
            if (distance > 0 && time > 0) {
                // 存储距离和时间信息
                std::pair<int, int> edge1(from, to);
                std::pair<int, int> edge2(to, from);
                
                distances[edge1] = distance;
                distances[edge2] = distance;
                times[edge1] = time;
                times[edge2] = time;
                
                // 使用距离作为默认权重（后续可以根据策略调整）
                graph.addEdge(from, to, distance);
                lastEdgeCount_++;
            }
        } else {
            std::cerr << "警告：第 " << lineNum << " 行格式错误，已跳过" << std::endl;
        }
    }
    
    lastNodeCount_ = graph.getNodeCount();
    file.close();
    
    std::cout << "成功加载数据：" << std::endl;
    std::cout << "  节点数: " << lastNodeCount_ << std::endl;
    std::cout << "  边数: " << lastEdgeCount_ << std::endl;
    
    return true;
}

bool DataLoader::parseLine(const std::string& line, int& from, int& to, 
                          double& distance, double& time) {
    std::istringstream iss(line);
    
    if (!(iss >> from >> to >> distance >> time)) {
        return false;
    }
    
    // 验证数据有效性
    if (distance < 0 || time < 0) {
        return false;
    }
    
    return true;
}

size_t DataLoader::getLoadedNodeCount() const {
    return lastNodeCount_;
}

size_t DataLoader::getLoadedEdgeCount() const {
    return lastEdgeCount_;
}
