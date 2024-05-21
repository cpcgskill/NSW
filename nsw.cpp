#include "nsw.h"

Node::Node(int idx, const Vector& vec) : idx(idx), vector(vec) {}

NSWGraph::NSWGraph(int max_neighbors) : max_neighbors(max_neighbors), dimension(-1), rng(std::random_device()()) {
    if (max_neighbors < 1) {
        throw std::invalid_argument("max_neighbors must be greater than or equal to 1.");
    }
}

void NSWGraph::addNode(const Vector& vector) {
    Node* node = new Node(nodes.size(), vector);
    addNodeInternal(node);
}

void NSWGraph::addNodes(const std::vector<Vector>& vectors) {
    std::vector<Node*> new_nodes;
    for (const auto& vec : vectors) {
        new_nodes.push_back(new Node(new_nodes.size(), vec));
    }
    std::shuffle(new_nodes.begin(), new_nodes.end(), rng);
    for (auto node : new_nodes) {
        addNodeInternal(node);
    }
}

void NSWGraph::addNodeInternal(Node* node) {
    nodes.push_back(node);

    if (dimension == -1) {
        dimension = node->vector.size();
    }

    if (nodes.size() <= max_neighbors) {
        for (auto existing_node : nodes) {
            if (existing_node != node) {
                existing_node->connections.push_back(node);
                node->connections.push_back(existing_node);
            }
        }
    } else {
        auto nearest_neighbors = findKNearest(*node, max_neighbors);
        for (const auto& [nearest_neighbor, _] : nearest_neighbors) {
            nearest_neighbor->connections.push_back(node);
            node->connections.push_back(nearest_neighbor);
        }
    }
}

NSWGraph::NodeDistanceVector NSWGraph::findKNearest(const Node& node, int k) {
    auto nearest = findNearest(node);
    Node* nearest_node = nearest.first;
    std::vector<Node*> candidates = nearest_node->connections;
    NodeDistanceVector nearest_nodes = {{nearest_node, nearest.second}};

    while (nearest_nodes.size() < k * 1.8 && !candidates.empty()) {
        Node* current_node = candidates.back();
        candidates.pop_back();
        double distance = euclideanDistance(node.vector, current_node->vector);
        nearest_nodes.push_back({current_node, distance});
        for (auto conn : current_node->connections) {
            candidates.push_back(conn);
        }
    }

    std::sort(nearest_nodes.begin(), nearest_nodes.end(), [](const auto& a, const auto& b) {
        return a.second < b.second;
    });

    if (nearest_nodes.size() > k) {
        nearest_nodes.resize(k);
    }

    return nearest_nodes;
}

std::pair<Node*, double> NSWGraph::findNearest(const Node& node) {
    if (nodes.size() == 1) {
        return {nodes[0], euclideanDistance(node.vector, nodes[0]->vector)};
    }

    double best_distance = std::numeric_limits<double>::infinity();
    Node* best_node = nodes[0];

    while (true) {
        double current_distance = euclideanDistance(node.vector, best_node->vector);
        if (current_distance < best_distance) {
            best_distance = current_distance;
            best_node = best_node;
        } else {
            break;
        }

        auto connections = best_node->connections;
        for (auto conn : connections) {
            double conn_distance = euclideanDistance(node.vector, conn->vector);
            if (conn_distance < current_distance) {
                current_distance = conn_distance;
                best_node = conn;
            }
        }
    }

    return {best_node, best_distance};
}

double NSWGraph::euclideanDistance(const Vector& v1, const Vector& v2) {
    double sum = 0.0;
    for (size_t i = 0; i < v1.size(); ++i) {
        sum += std::pow(v1[i] - v2[i], 2);
    }
    return std::sqrt(sum);
}
