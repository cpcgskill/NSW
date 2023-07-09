from __future__ import print_function, division
import random
import time
from array import array
from multiprocessing import Pool
from nsw import Node, NSWGraph
import matplotlib.pyplot as plt


def test(n, k=1000):
    start_init_time = time.time()
    graph = NSWGraph(10)
    points = [array('d', [random.random(), random.random(), random.random()]) for _ in range(n)]
    graph.add_nodes(points)
    end_init_time = time.time()

    init_time = float(end_init_time - start_init_time)
    print("Initialization Time for {} Nodes: {} seconds".format(n, init_time))

    start_query_time = time.time()
    for i in range(k):
        query_point = array('d', [random.random(), random.random(), random.random()])
        query_node = Node(None, query_point)
        nearest_node, distance = graph.find_nearest(query_node)
    end_query_time = time.time()

    query_time = float(end_query_time - start_query_time) / 100
    print("`find_nearest` ran {} times, Average Running Time: {} seconds".format(k, query_time))

    return init_time, query_time


def plot_results():
    ns = list(range(1, 100001, 1000))

    # Create a multiprocessing pool and map the test function to the values in ns
    with Pool() as pool:
        results = pool.map(test, ns)

    # Unpack the results into initialization times and query times
    init_times, query_times = zip(*results)

    plt.figure(figsize=(10, 4))

    plt.subplot(1, 2, 1)
    plt.plot(ns, init_times)
    plt.xlabel('Number of Nodes (n)')
    plt.ylabel('Initialization Time (seconds)')
    plt.title('Performance of Initialization')

    plt.subplot(1, 2, 2)
    plt.plot(ns, query_times)
    plt.xlabel('Number of Nodes (n)')
    plt.ylabel('Average Query Time (seconds)')
    plt.title('Performance of find_nearest')

    plt.tight_layout()
    plt.show()


if __name__ == '__main__':
    plot_results()
