#pragma once
#ifndef NSW_H
#define NSW_H

#include <vector>
#include <algorithm>
#include <random>
#include <cmath>
#include <stdexcept>

/**
 * @class Node
 * @brief 表示NSW（Navigable Small World）图中的一个节点。
 */
class Node {
public:
    int idx; ///< 节点的索引
    using Vector = std::vector<double>; ///< 向量类型别名
    Vector vector; ///< 表示节点的向量
    std::vector<Node*> connections; ///< 与其他节点的连接

    /**
     * @brief 构造一个Node对象。
     * @param idx 节点的索引。
     * @param vec 表示节点的向量。
     */
    Node(int idx, const Vector& vec);
};

/**
 * @class NSWGraph
 * @brief 表示一个NSW（Navigable Small World）图。
 *
 * 该实现基于以下论文：
 * Yury Malkov 和 Dmitry Yashunin, "Efficient and robust approximate nearest neighbor search using Hierarchical Navigable Small World graphs,"
 * IEEE Transactions on Pattern Analysis and Machine Intelligence, 42(4), 824-836, 2018.
 * 论文链接: https://arxiv.org/abs/1603.09320
 */
class NSWGraph {
public:
    using Vector = std::vector<double>; ///< 向量类型别名
    using NodeDistancePair = std::pair<Node*, double>; ///< 节点和距离的对类型别名
    using NodeDistanceVector = std::vector<NodeDistancePair>; ///< 节点和距离对的向量类型别名

    /**
     * @brief 构造一个NSWGraph对象。
     * @param max_neighbors 每个节点的最大邻居数。
     * @throws std::invalid_argument 如果max_neighbors小于1。
     */
    explicit NSWGraph(int max_neighbors);

    /**
     * @brief 向图中添加一个节点。
     * @param vector 表示要添加的节点的向量。
     */
    void addNode(const Vector& vector);

    /**
     * @brief 向图中添加多个节点。
     * @param vectors 表示要添加的节点的向量集合。
     */
    void addNodes(const std::vector<Vector>& vectors);

    /**
     * @brief 查找给定节点的最近邻节点。
     * @param node 要查找最近邻的节点。
     * @return 一个包含最近邻节点和距离的对。
     */
    std::pair<Node*, double> findNearest(const Node& node);

private:
    std::vector<Node*> nodes;
    int max_neighbors;
    int dimension;
    std::mt19937 rng;

    void addNodeInternal(Node* node);
    NodeDistanceVector findKNearest(const Node& node, int k);
    static double euclideanDistance(const Vector& v1, const Vector& v2);
};

#endif // NSW_H
