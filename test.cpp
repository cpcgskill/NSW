#include <iostream>
#include <vector>
#include <chrono>
#include <random>
#include <algorithm>
#include <fstream>
#include "nsw.h"

void test(int n, int k, std::ofstream& init_file, std::ofstream& query_file) {
    auto start_init_time = std::chrono::high_resolution_clock::now();

    NSWGraph graph(10);
    std::vector<std::vector<double>> points(n, std::vector<double>(3));
    std::mt19937 rng(std::random_device{}());
    std::uniform_real_distribution<> dist(0.0, 1.0);

    for (auto& point : points) {
        std::generate(point.begin(), point.end(), [&]() { return dist(rng); });
    }

    graph.addNodes(points);

    auto end_init_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> init_time = end_init_time - start_init_time;
    init_file << n << " " << init_time.count() << "\n";

    auto start_query_time = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < k; ++i) {
        std::vector<double> query_point(3);
        std::generate(query_point.begin(), query_point.end(), [&]() { return dist(rng); });
        Node query_node(-1, query_point);
        auto nearest = graph.findNearest(query_node);
    }

    auto end_query_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> query_time = end_query_time - start_query_time;
    query_file << n << " " << query_time.count() / k << "\n";
}

int main() {
    std::ofstream init_file("init_times.txt");
    std::ofstream query_file("query_times.txt");

    int k = 10000;
    for (int n = 1; n <= 100001; n += 1000) {
        test(n, k, init_file, query_file);
    }

    init_file.close();
    query_file.close();

    return 0;
}
