#include "../include/MapEngine.h"
#include <iostream>
#include <iomanip>
#include <chrono>

void printPathResult(const PathResult& result, int from, int to) {
    std::cout << "\n========== 路径规划结果 ==========" << std::endl;
    std::cout << "起始节点: " << from << std::endl;
    std::cout << "目标节点: " << to << std::endl;
    
    if (result.found) {
        std::cout << "路径找到: 是" << std::endl;
        std::cout << "路径: ";
        for (size_t i = 0; i < result.path.size(); ++i) {
            std::cout << result.path[i];
            if (i < result.path.size() - 1) {
                std::cout << " -> ";
            }
        }
        std::cout << std::endl;
        std::cout << "总权重: " << std::fixed << std::setprecision(2) 
                  << result.totalWeight << std::endl;
        std::cout << "执行时间: " << result.elapsedMicroseconds << " 微秒" << std::endl;
    } else {
        std::cout << "路径找到: 否" << std::endl;
        std::cout << "原因: 无法从节点 " << from << " 到达节点 " << to << std::endl;
    }
    std::cout << "==================================\n" << std::endl;
}

int main(int argc, char* argv[]) {
    std::cout << "========================================" << std::endl;
    std::cout << "   高性能路径规划引擎 (C++)" << std::endl;
    std::cout << "   基于堆优化的Dijkstra算法" << std::endl;
    std::cout << "========================================" << std::endl;
    
    // 创建地图引擎
    MapEngine engine;
    
    // 加载地图数据
    std::string dataFile = "data/test_data.txt";
    if (argc > 1) {
        dataFile = argv[1];
    }
    
    std::cout << "\n正在加载地图数据: " << dataFile << std::endl;
    if (!engine.loadMap(dataFile)) {
        std::cerr << "错误: 无法加载地图数据文件" << std::endl;
        return 1;
    }
    
    std::cout << "\n地图加载完成！" << std::endl;
    std::cout << "节点数: " << engine.getNodeCount() << std::endl;
    std::cout << "边数: " << engine.getEdgeCount() << std::endl;
    
    // 测试用例1: 最短距离策略
    std::cout << "\n【测试1】最短距离策略" << std::endl;
    engine.setStrategy(PathStrategy::SHORTEST_DISTANCE);
    std::cout << "当前策略: " << engine.getCurrentStrategyName() << std::endl;
    
    PathResult result1 = engine.findPath(1, 5);
    printPathResult(result1, 1, 5);
    
    // 测试用例2: 最少时间策略
    std::cout << "【测试2】最少时间策略" << std::endl;
    engine.setStrategy(PathStrategy::LEAST_TIME);
    std::cout << "当前策略: " << engine.getCurrentStrategyName() << std::endl;
    
    PathResult result2 = engine.findPath(1, 5);
    printPathResult(result2, 1, 5);
    
    // 测试用例3: 处理孤立点
    std::cout << "【测试3】孤立点检测" << std::endl;
    // 假设节点999是孤立点（如果存在）
    if (engine.isIsolated(999)) {
        std::cout << "节点 999 是孤立点" << std::endl;
    } else {
        std::cout << "节点 999 不是孤立点或不存在" << std::endl;
    }
    
    // 测试用例4: 起点和终点相同
    std::cout << "\n【测试4】起点和终点相同" << std::endl;
    PathResult result4 = engine.findPath(1, 1);
    printPathResult(result4, 1, 1);
    
    // 测试用例5: 不存在的节点
    std::cout << "【测试5】不存在的节点" << std::endl;
    PathResult result5 = engine.findPath(1, 99999);
    printPathResult(result5, 1, 99999);
    
    // 测试用例6: 大规模路径搜索性能测试
    std::cout << "【测试6】性能测试 - 多次路径搜索" << std::endl;
    auto start = std::chrono::high_resolution_clock::now();
    
    const int testCount = 100;
    for (int i = 0; i < testCount; ++i) {
        engine.findPath(1, 5);
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    double avgTime = static_cast<double>(duration.count()) / testCount;
    
    std::cout << "执行 " << testCount << " 次路径搜索" << std::endl;
    std::cout << "总耗时: " << duration.count() << " 微秒" << std::endl;
    std::cout << "平均耗时: " << std::fixed << std::setprecision(2) 
              << avgTime << " 微秒/次" << std::endl;
    
    std::cout << "\n========================================" << std::endl;
    std::cout << "测试完成！" << std::endl;
    std::cout << "========================================" << std::endl;
    
    return 0;
}
