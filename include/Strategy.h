#ifndef STRATEGY_H
#define STRATEGY_H

/**
 * @brief 路径规划策略枚举
 */
enum class PathStrategy {
    SHORTEST_DISTANCE,  // 最短距离
    LEAST_TIME          // 最少时间
};

/**
 * @brief 策略接口类
 * 用于计算边的权重，支持不同策略的动态切换
 */
class Strategy {
public:
    virtual ~Strategy() = default;
    
    /**
     * @brief 计算边的权重
     * @param distance 距离
     * @param time 时间
     * @return 计算后的权重值
     */
    virtual double calculateWeight(double distance, double time) const = 0;
    
    /**
     * @brief 获取策略名称
     * @return 策略名称字符串
     */
    virtual const char* getName() const = 0;
};

/**
 * @brief 最短距离策略
 */
class ShortestDistanceStrategy : public Strategy {
public:
    double calculateWeight(double distance, double time) const override {
        return distance;  // 直接使用距离作为权重
    }
    
    const char* getName() const override {
        return "Shortest Distance";
    }
};

/**
 * @brief 最少时间策略
 */
class LeastTimeStrategy : public Strategy {
public:
    double calculateWeight(double distance, double time) const override {
        return time;  // 直接使用时间作为权重
    }
    
    const char* getName() const override {
        return "Least Time";
    }
};

#endif // STRATEGY_H
