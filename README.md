# 高性能路径规划引擎 (C++)

一个基于堆优化的Dijkstra算法实现的高性能路径规划引擎，支持最短距离和最少时间等多种策略的动态切换。

## 项目特性

### 算法实现
- **纯C++手工构建无向带权图模型**：使用邻接表存储结构，优化大规模节点检索性能
- **堆优化的Dijkstra算法**：使用优先队列（最小堆）实现，路径规划响应速度达到微秒级
- **优化的邻接表存储**：针对大规模节点检索进行了专门优化

### 面向对象架构
- **严格遵循OOP设计模式**：封装地图引擎核心类，解耦数据读取、图构建与路径搜索模块
- **多策略动态切换**：支持最短距离、最少时间等多种策略，通过策略模式实现灵活切换
- **内存管理**：使用智能指针管理资源，避免内存泄漏

### 健壮性保障
- **边缘情况处理**：处理孤立点、环路等多种极端拓扑情况
- **大量测试数据验证**：通过单元测试和集成测试验证算法在复杂地图数据下的稳定性

## 项目结构

```
project/
├── include/              # 头文件目录
│   ├── Graph.h          # 无向带权图类
│   ├── PathFinder.h     # 路径规划引擎核心类
│   ├── DataLoader.h     # 数据加载器类
│   ├── Strategy.h       # 策略接口和实现
│   └── MapEngine.h      # 地图引擎核心类
├── src/                  # 源文件目录
│   ├── Graph.cpp
│   ├── PathFinder.cpp
│   ├── DataLoader.cpp
│   ├── MapEngine.cpp
│   └── main.cpp         # 主程序
├── tests/                # 测试文件目录
│   └── test.cpp         # 单元测试
├── data/                 # 数据文件目录
│   └── test_data.txt    # 测试数据
├── CMakeLists.txt       # CMake构建文件
└── README.md            # 项目说明文档
```

## 编译和运行

### 前置要求
- C++17或更高版本的编译器（GCC 7+, Clang 5+, MSVC 2017+）
- CMake 3.10或更高版本

### 编译步骤

#### Windows (使用Visual Studio)
```bash
mkdir build
cd build
cmake .. -G "Visual Studio 16 2019" -A x64
cmake --build . --config Release
```

#### Linux/macOS
```bash
mkdir build
cd build
cmake ..
make -j4
```

### 运行

#### 运行主程序
```bash
# Windows
.\bin\path_planning_engine.exe [数据文件路径]

# Linux/macOS
./bin/path_planning_engine [数据文件路径]
```

#### 运行测试
```bash
# Windows
.\bin\path_planning_test.exe

# Linux/macOS
./bin/path_planning_test
```

## 数据文件格式

数据文件为文本格式，每行表示一条边：

```
from to distance time
```

示例：
```
1 2 10.0 5.0
1 3 15.0 8.0
2 3 5.0 3.0
```

- `from`: 起始节点ID
- `to`: 目标节点ID
- `distance`: 边的距离
- `time`: 通过该边所需的时间

注意：
- 以 `#` 开头的行被视为注释
- 空行会被忽略
- 图是无向的，每条边会自动创建双向连接

## 使用示例

### 基本使用

```cpp
#include "MapEngine.h"

// 创建地图引擎
MapEngine engine;

// 加载地图数据
engine.loadMap("data/test_data.txt");

// 设置策略（最短距离）
engine.setStrategy(PathStrategy::SHORTEST_DISTANCE);

// 查找路径
PathResult result = engine.findPath(1, 5);

if (result.found) {
    // 打印路径
    for (int node : result.path) {
        std::cout << node << " ";
    }
    std::cout << std::endl;
    std::cout << "总权重: " << result.totalWeight << std::endl;
    std::cout << "执行时间: " << result.elapsedMicroseconds << " 微秒" << std::endl;
}
```

### 策略切换

```cpp
// 最短距离策略
engine.setStrategy(PathStrategy::SHORTEST_DISTANCE);
PathResult result1 = engine.findPath(1, 5);

// 最少时间策略
engine.setStrategy(PathStrategy::LEAST_TIME);
PathResult result2 = engine.findPath(1, 5);
```

## 核心类说明

### Graph
无向带权图类，使用邻接表存储结构。

主要方法：
- `addNode(int nodeId)`: 添加节点
- `addEdge(int from, int to, double weight)`: 添加无向边
- `getAdjacentEdges(int nodeId)`: 获取节点的邻接边列表
- `isIsolated(int nodeId)`: 检查节点是否为孤立点

### PathFinder
路径规划引擎核心类，实现堆优化的Dijkstra算法。

主要方法：
- `setStrategy(std::unique_ptr<Strategy> strategy)`: 设置路径规划策略
- `findPath(int from, int to)`: 查找最短路径

### MapEngine
地图引擎核心类，封装数据读取、图构建与路径搜索模块。

主要方法：
- `loadMap(const std::string& filename)`: 从文件加载地图数据
- `setStrategy(PathStrategy strategy)`: 设置路径规划策略
- `findPath(int from, int to)`: 查找路径

### Strategy
策略接口类，支持不同路径规划策略的实现。

已实现的策略：
- `ShortestDistanceStrategy`: 最短距离策略
- `LeastTimeStrategy`: 最少时间策略

## 性能特性

- **微秒级响应**：路径规划响应速度达到微秒级
- **大规模支持**：优化的邻接表存储结构支持大规模节点检索
- **内存高效**：使用智能指针和高效的数据结构，避免不必要的内存分配

## 测试覆盖

项目包含完整的单元测试，覆盖以下场景：
- 基本路径查找
- 策略切换
- 孤立点处理
- 起点终点相同
- 不存在节点
- 环路处理
- 性能测试

## 技术栈

- **语言**: C++17
- **算法**: Dijkstra算法（堆优化）
- **数据结构**: 邻接表、优先队列（最小堆）
- **设计模式**: 策略模式、单例模式
- **构建工具**: CMake

## 许可证

本项目仅供学习和研究使用。

## 作者

罗莉：高性能路径规划引擎项目
