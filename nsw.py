from __future__ import print_function, division
import random
import math
from array import array

class Node(object):
    def __init__(self, idx, vector):
        self.idx = idx
        self.vector = array('d', vector)
        self.connections = []

class NSWGraph(object):
    def __init__(self, max_neighbors):
        if max_neighbors < 1:
            raise ValueError("max_neighbors must be greater than or equal to 1.")
        self.nodes = []
        self.dimension = None
        self.max_neighbors = max_neighbors

    def add_node(self, vector):
        node = Node(len(self.nodes), vector)
        self._add_node(node)

    def add_nodes(self, vectors):
        nodes = [Node(i, vector) for i, vector in enumerate(vectors)]
        random.shuffle(nodes)
        for node in nodes:
            self._add_node(node)

    def _add_node(self, node):
        self.nodes.append(node)

        if self.dimension is None:
            self.dimension = len(node.vector)

        if len(self.nodes) <= self.max_neighbors:
            for existing_node in self.nodes[:-1]:
                existing_node.connections.append(node)
                node.connections.append(existing_node)
        else:
            nearest_neighbors = self._find_k_nearest(node, self.max_neighbors)
            for nearest_neighbor, _ in nearest_neighbors:
                nearest_neighbor.connections.append(node)
                node.connections.append(nearest_neighbor)

    def _find_k_nearest(self, node, k):
        nearest_node, _ = self.find_nearest(node)
        candidates = nearest_node.connections.copy()
        nearest_nodes = [(nearest_node, euclidean_distance(node.vector, nearest_node.vector))]

        while len(nearest_nodes) < k * 1.8 and candidates:
            current_node = candidates.pop()
            distance = euclidean_distance(node.vector, current_node.vector)
            nearest_nodes.append((current_node, distance))
            candidates.extend(current_node.connections)

        nearest_nodes.sort(key=lambda x: x[1])
        return nearest_nodes[:k]

    def find_nearest(self, node):
        if len(self.nodes) == 1:
            return self.nodes[0], euclidean_distance(node.vector, self.nodes[0].vector)
        else:
            best_distance = float('inf')
            best_node = self.nodes[0]

            while True:
                connections = [(n, euclidean_distance(node.vector, n.vector)) for n in best_node.connections]
                current_node, current_distance = min(connections, key=lambda x: x[1])
                if current_distance < best_distance:
                    best_distance = current_distance
                    best_node = current_node
                else:
                    break

            return best_node, best_distance

def euclidean_distance(v1, v2):
    return math.sqrt(sum((x - y) ** 2 for x, y in zip(v1, v2)))
