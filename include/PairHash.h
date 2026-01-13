#ifndef PAIRHASH_H
#define PAIRHASH_H

#include <utility>
#include <functional>

// 哈希函数用于pair<int, int>
namespace std {
    template<>
    struct hash<std::pair<int, int>> {
        size_t operator()(const std::pair<int, int>& p) const {
            return std::hash<int>()(p.first) ^ (std::hash<int>()(p.second) << 1);
        }
    };
}

#endif // PAIRHASH_H
