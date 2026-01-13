#include "../include/MapEngine.h"
#include <iostream>
#include <iomanip>
#include <cassert>
#include <vector>
#include <chrono>

void testBasicPathFinding() {
    std::cout << "【测试1】基本路径查找..." << std::endl;
    
    MapEngine engine;
    assert(engine.loadMap("data/test_data.txt"));
    
    // 测试最短距离路径
    engine.setStrategy(PathStrategy::SHORTEST_DISTANCE);
    PathResult result = engine.findPath(1, 5);
    
    assert(result.found);
    assert(result.path.size() >= 2);
    assert(result.path[0] == 1);
    assert(result.path[result.path.size() - 1] == 5);
    assert(result.totalWeight > 0);
    
    std::cout << "✓ 基本路径查找测试通过" << std::endl;
}

void testStrategySwitching() {
    std::cout << "【测试2】策略切换..." << std::endl;
    
    MapEngine engine;
    assert(engine.loadMap("data/test_data.txt"));
    
    // 最短距离策略
    engine.setStrategy(PathStrategy::SHORTEST_DISTANCE);
    PathResult result1 = engine.findPath(1, 5);
    assert(result1.found);
    
    // 最少时间策略
    engine.setStrategy(PathStrategy::LEAST_TIME);
    PathResult result2 = engine.findPath(1, 5);
    assert(result2.found);
    
    // 两种策略的结果可能不同
    std::cout << "✓ 策略切换测试通过" << std::endl;
}

void testIsolatedNode() {
    std::cout << "【测试3】孤立点处理..." << std::endl;
    
    MapEngine engine;
    assert(engine.loadMap("data/test_data.txt"));
    
    // 节点8是孤立点
    assert(engine.isIsolated(8));
    
    // 从孤立点出发无法到达其他节点
    PathResult result = engine.findPath(8, 1);
    assert(!result.found);
    
    std::cout << "✓ 孤立点处理测试通过" << std::endl;
}

void testSameNode() {
    std::cout << "【测试4】起点终点相同..." << std::endl;
    
    MapEngine engine;
    assert(engine.loadMap("data/test_data.txt"));
    
    PathResult result = engine.findPath(1, 1);
    assert(result.found);
    assert(result.path.size() == 1);
    assert(result.path[0] == 1);
    assert(result.totalWeight == 0.0);
    
    std::cout << "✓ 起点终点相同测试通过" << std::endl;
}

void testNonExistentNode() {
    std::cout << "【测试5】不存在节点..." << std::endl;
    
    MapEngine engine;
    assert(engine.loadMap("data/test_data.txt"));
    
    // 不存在的节点
    PathResult result = engine.findPath(1, 99999);
    assert(!result.found);
    
    std::cout << "✓ 不存在节点测试通过" << std::endl;
}

void testCycle() {
    std::cout << "【测试6】环路处理..." << std::endl;
    
    MapEngine engine;
    assert(engine.loadMap("data/test_data.txt"));
    
    // 测试包含环路的路径（5->6->7->5形成环路）
    PathResult result = engine.findPath(5, 7);
    assert(result.found);
    // 算法应该能找到最短路径，不会陷入环路
    
    std::cout << "✓ 环路处理测试通过" << std::endl;
}

void testPerformance() {
    std::cout << "【测试7】性能测试..." << std::endl;
    
    MapEngine engine;
    assert(engine.loadMap("data/test_data.txt"));
    
    auto start = std::chrono::high_resolution_clock::now();
    
    const int testCount = 1000;
    for (int i = 0; i < testCount; ++i) {
        engine.findPath(1, 5);
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    double avgTime = static_cast<double>(duration.count()) / testCount;
    
    std::cout << "执行 " << testCount << " 次搜索，平均耗时: " 
              << std::fixed << std::setprecision(2) << avgTime << " 微秒" << std::endl;
    
    // 验证平均耗时在合理范围内（微秒级）
    assert(avgTime < 1000.0);  // 平均应该小于1毫秒
    
    std::cout << "✓ 性能测试通过" << std::endl;
}

int main() {
    std::cout << "========================================" << std::endl;
    std::cout << "   路径规划引擎单元测试" << std::endl;
    std::cout << "========================================" << std::endl;
    
    try {
        testBasicPathFinding();
        testStrategySwitching();
        testIsolatedNode();
        testSameNode();
        testNonExistentNode();
        testCycle();
        testPerformance();
        
        std::cout << "\n========================================" << std::endl;
        std::cout << "所有测试通过！" << std::endl;
        std::cout << "========================================" << std::endl;
        
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "测试失败: " << e.what() << std::endl;
        return 1;
    }
}
