#ifndef NSW_H
#define NSW_H

#include <vector>
#include <array>
#include <algorithm>
#include <random>
#include <cmath>
#include <stdexcept>

class Node {
public:
    int idx;
    std::vector<double> vector;
    std::vector<Node*> connections;

    Node(int idx, const std::vector<double>& vec);
};

class NSWGraph {
public:
    explicit NSWGraph(int max_neighbors);
    void addNode(const std::vector<double>& vector);
    void addNodes(const std::vector<std::vector<double>>& vectors);
    std::pair<Node*, double> findNearest(const Node& node);

private:
    std::vector<Node*> nodes;
    int max_neighbors;
    int dimension;
    std::mt19937 rng;

    void addNode(Node* node);
    std::vector<std::pair<Node*, double>> findKNearest(const Node& node, int k);
    static double euclideanDistance(const std::vector<double>& v1, const std::vector<double>& v2);
};

#endif // NSW_H
