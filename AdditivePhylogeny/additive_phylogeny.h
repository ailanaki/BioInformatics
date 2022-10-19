#pragma once

#include <vector>
#include <fstream>
#include <iostream>
#include <set>

void read_matrix(std::vector<std::vector<int>> &graph) {
    std::ifstream input("AdditivePhylogeny/inputs/input_1.txt");
    int n = 0;
    input >> n;
    for (int i = 0; i < n; ++i) {
        std::vector<int> tmp;
        int one;
        for (int j = 0; j < n; ++j) {
            input >> one;
            tmp.push_back(one);
        }
        graph.push_back(tmp);
    }

}

int Limb(std::vector<std::vector<int>> &graph, int n) {
    int min = (graph[n - 1][0] + graph[n - 1][1] - graph[0][1]) / 2;
    for (int i = 0; i < n - 1; ++i) {
        for (int j = 0; j < n - 1; ++j) {
            if (i != n - 1 && i != j && j != n - 1) {
                if (min > (graph[n - 1][i] + graph[n - 1][j] - graph[i][j]) / 2) {
                    min = (graph[n - 1][i] + graph[n - 1][j] - graph[i][j]) / 2;
                }
            }
        }

    }
    return min;
}


void add(int node, int next, int length, std::vector<std::vector<std::pair<int, int>>> &tree) {
    tree[node].emplace_back(next, length);
    if (tree.size() < next) {
        tree.emplace_back();
    }
    tree[next].emplace_back(node, length);
}

void remove_node(int one, int two, int length, std::vector<std::vector<std::pair<int, int>>> &tree) {
    auto it = tree[one].begin();
    while (it->first != two && it->second != length) {
        it++;
    }
    tree[one].erase(it);
}

void remove(int one, int two, int length, std::vector<std::vector<std::pair<int, int>>> &tree) {
    remove_node(one, two, length, tree);
    remove_node(two, one, length, tree);
}

void get_path(int one, int two, std::vector<std::pair<int, int>> &path,
              std::vector<std::vector<std::pair<int, int>>> &tree) {
    std::set<std::pair<int, int>> been;
    std::vector<std::vector<std::pair<int, int>>> paths;
    std::vector<std::pair<int, int>> tmp;
    for (auto i: tree[one]) {
        tmp.push_back(i);
    }
    paths.push_back(tmp);
    while (true) {
        path.clear();
        path = paths[0];
        paths.erase(paths.begin());
        auto last_node = path[path.size() - 1].first;
        if (last_node == two) return;
        for (auto i: tree[last_node]) {
            auto tmp_path = path;
            if (been.count(i) == 0) {
                tmp_path.push_back(i);
                paths.push_back(tmp_path);
                been.insert(i);
            }
        }
    }

}

int get_new(std::vector<std::vector<std::pair<int, int>>> &tree) {
    tree.emplace_back();
    return tree.size() - 1;
}

void print(std::vector<std::vector<std::pair<int, int>>> &tree) {
    for (int i = 0; i < tree.size(); ++i) {
        for (auto node: tree[i]) {
            std::cout << i << " -> " << node.first << ":" << node.second << "\n";
        }
    }
}

void alghoritm(std::vector<std::vector<int>> &graph, int n, int real_n,
               std::vector<std::vector<std::pair<int, int>>> &result) {
    if (n == 2) {
        for (int i = 0; i < real_n; ++i) {
            result.emplace_back();
        }
        add(0, 1, graph[0][1], result);
        return;
    }

    auto limbLength = Limb(graph, n);
    for (int j = 0; j < n - 1; ++j) {
        graph[j][n - 1] = graph[j][n - 1] - limbLength;
        graph[n - 1][j] = graph[n - 1][j] - limbLength;

    }
    auto i = -1, j = -1;
    for (int k = 0; k < n - 1; ++k) {
        for (int t = 0; t < n - 1; ++t) {
            if (graph[k][t] == graph[k][n - 1] + graph[n - 1][t]) {
                i = k;
                j = t;
                break;
            }
        }
        if (i != -1) {
            break;
        }
    }

    auto x = graph[i][n - 1];
    for (int k = 0; k < n; ++k) {
        graph[k].erase(graph[k].end() - 1);
    }
    graph.erase(graph.end() - 1);
    std::vector<std::pair<int, int>> path;
    alghoritm(graph, n - 1, real_n, result);
    get_path(i, j, path, result);
    auto length = 0;
    auto new_node = get_new(result);
    auto previous_node = i;
    for (auto node: path) {
        length += node.second;
        if (length > x) {
            remove(previous_node, node.first, node.second, result);
            add(previous_node, new_node, node.second - (length - x), result);
            add(new_node, node.first, length - x, result);
            add(new_node, n - 1, limbLength, result);
            break;
        }
        if (length == x) {
            add(node.first, new_node, x, result);
        }
        previous_node = node.first;
    }

}

void additive_phylogeny() {
    std::vector<std::vector<int>> graph;
    std::vector<std::vector<std::pair<int, int>>> result;
    read_matrix(graph);
    alghoritm(graph, graph.size(), graph.size(), result);
    print(result);
}
