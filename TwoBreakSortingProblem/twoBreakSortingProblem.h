#pragma once

#include <fstream>
#include <vector>
#include <iostream>

void chromosomeToCycle(std::vector<int> &chromosome, std::vector<int> &nodes) {
    nodes.clear();
    for (auto block: chromosome) {
        if (block > 0) {
            nodes.push_back(2 * block - 1);
            nodes.push_back(2 * block);
        } else {
            nodes.push_back(-2 * block);
            nodes.push_back(-2 * block - 1);
        }
    }
}

void cycleToChromosome(std::vector<int> &chromosome, std::vector<int> &nodes) {
    chromosome.clear();
    for (int i = 0; i < nodes.size(); i += 2) {
        if (nodes[i] < nodes[i + 1]) {
            chromosome.push_back(nodes[i + 1] / 2);
        } else {
            chromosome.push_back(-nodes[i] / 2);
        }
    }
}


void coloredEdges(std::vector<std::vector<int>> &p, std::vector<std::pair<int, int>> &edges) {
    edges.clear();
    for (auto chromosome: p) {
        std::vector<int> nodes;
        chromosomeToCycle(chromosome, nodes);
        for (int i = 1; i < nodes.size(); i += 2) {
            if (i != nodes.size() - 1) {
                edges.emplace_back(nodes[i], nodes[i + 1]);
            } else {
                edges.emplace_back(nodes[i], nodes[0]);
            }
        }
    }
}


int indNextEdge(std::pair<int, int> current, std::vector<std::pair<int, int>> &edges) {
    if (edges.empty()) return -1;
    auto ind = 0;
    while (current.first != edges[ind].first && current.first != edges[ind].second &&
           current.second != edges[ind].first && current.second != edges[ind].second) {
        ind++;
        if (ind == edges.size()) return -1;
    }
    return ind;
}


void findCycles(std::vector<std::pair<int, int>> &edges, std::vector<std::vector<std::pair<int, int>>> &cycles) {
    cycles.clear();
    while (!edges.empty()) {
        auto start = edges[0];
        edges.erase(edges.begin());
        std::vector<std::pair<int, int>> cycle;
        cycle.push_back(start);
        auto current = indNextEdge(start, edges);
        while (current != -1) {
            auto pair = edges[current];
            cycle.push_back(pair);
            edges.erase(edges.begin() + current);
            current = indNextEdge(pair, edges);
        }
        if (cycle.size() > 2) cycles.push_back(cycle);
    }
}

void twoBreakOnGenomeGraph(std::vector<std::pair<int, int>> &genomeGraph, int i1, int i2, int i3, int i4) {
    for (auto & i : genomeGraph) {
        if (i.first == i1 && i.second == i2) {
            i = std::pair(i1, i3);
        } else if (i.first == i2 && i.second == i1) {
            i = std::pair(i3, i1);
        }
        if (i.first == i3 && i.second == i4) {
            i = std::pair(i2, i4);
        } else if (i.first == i4 && i.second == i3) {
            i = std::pair(i4, i2);
        }
    }
}


int findNextEdge(int second, std::vector<std::pair<int, int>> &edges) {
    if (edges.empty()) return -1;
    auto ind = 0;
    auto val = second;
    if (val % 2 == 0) val -= 1;
    else val += 1;

    while (val != edges[ind].second && val != edges[ind].first) {
        ind += 1;
        if (ind == edges.size()) return -1;
    }
    if (val == edges[ind].second) {
        auto tmp = edges[ind].first;
        edges[ind].first = edges[ind].second;
        edges[ind].second = tmp;
    }
    return ind;
}


void graphToGenome(std::vector<std::pair<int, int>> &genomeGraph,
                   std::vector<std::vector<int>> &q) {
    std::vector<std::vector<int>> cycles;
    q.clear();
    while (!genomeGraph.empty()) {
        std::vector<int> cycle;
        auto current = 0;
        while (current != -1) {
            cycle.push_back(genomeGraph[current].first);
            cycle.push_back(genomeGraph[current].second);
            auto second = genomeGraph[current].second;
            genomeGraph.erase(genomeGraph.begin() + current);
            int next_edge = findNextEdge(second, genomeGraph);
            current = next_edge;
        }
        cycles.push_back(cycle);
    }
    for (auto cycle: cycles) {
        std::vector<int> tmp;
        tmp.push_back(cycle[cycle.size() - 1]);
        for (int i = 0; i < cycle.size() - 1; ++i) {
            tmp.push_back(cycle[i]);
        }
        std::vector<int> chromosome;
        cycleToChromosome(chromosome, tmp);
        q.push_back(chromosome);
    }
}


void twoBreakOnGenome(std::vector<std::vector<int>> &p, int i1, int i2, int i3, int i4,
                      std::vector<std::vector<int>> &q) {
    std::vector<std::pair<int, int>> genomeGraph;
    coloredEdges(p, genomeGraph);
    twoBreakOnGenomeGraph(genomeGraph, i1, i2, i3, i4);
    graphToGenome(genomeGraph, q);
}

bool find(std::pair<int, int> pair, std::vector<std::pair<int, int>> &array) {
    for (auto &i: array) {
        if (pair.first == i.first && pair.second == i.second ||
            pair.second == i.first && pair.first == i.second) {
            return true;
        }
    }
    return false;
}


void shortestRearangment(std::vector<std::vector<int>> &p, std::vector<std::vector<int>> &q,
                         std::vector<std::vector<std::vector<int>>> &result) {
    result.push_back(p);
    std::vector<std::pair<int, int>> red;
    std::vector<std::pair<int, int>> blue;
    std::vector<std::pair<int, int>> graph;
    coloredEdges(p, red);
    coloredEdges(q, blue);
    for (auto &i: blue) {
        graph.push_back(i);
    }
    for (auto &i: red) {
        graph.push_back(i);
    }
    std::vector<std::vector<std::pair<int, int>>> cycles;
    findCycles(graph, cycles);
    while (!cycles.empty()) {
        auto cycle = cycles[0];
        for (int i = 0; i < cycle.size() - 1; ++i) {
            if (cycle[i].first == cycle[i + 1].first || cycle[i].first == cycle[i + 1].second) {
                auto tmp = cycle[i].first;
                cycle[i].first = cycle[i].second;
                cycle[i].second = tmp;
            }
            if (cycle[i + 1].second == cycle[i].first || cycle[i + 1].second == cycle[i].second) {
                auto tmp = cycle[i + 1].first;
                cycle[i + 1].first = cycle[i + 1].second;
                cycle[i + 1].second = tmp;
            }
        }

        auto ind = 0;
        while (!find(cycle[ind], red)) {
            ind++;
        }
        int i1 = cycle[ind].first, i2 = cycle[ind].second;
        int i3, i4;
        auto it = red.begin();
        //TODO:  вынести в функцию
        for (int i = 0; i < red.size(); ++i) {
            if (it->first == cycle[ind].first && it->second == cycle[ind].second ||
                    it->first == cycle[ind].second && it->second == cycle[ind].first){
                red.erase(it);
                break;
            }
            it++;
        }
        if (ind + 2 != cycle.size()) {
            i3 = cycle[ind + 2].first, i4 = cycle[ind + 2].second;
            it = red.begin();
            for (int i = 0; i < red.size(); ++i) {
                if (it->first == cycle[ind + 2].first && it->second == cycle[ind + 2].second ||
                        it->first == cycle[ind + 2].second && it->second == cycle[ind + 2].first){
                    red.erase(it);
                    break;
                }
                it++;
            }
        } else {
            i3 = cycle[0].first, i4 = cycle[0].second;
            red.erase(red.begin());
        }

        red.emplace_back(i1, i4);
        red.emplace_back(i2, i3);
        graph.clear();
        for (auto &i: blue) {
            graph.push_back(i);
        }
        for (auto &i: red) {
            graph.push_back(i);
        }
        findCycles(graph, cycles);
        std::vector<int> genome;
        std::vector<std::vector<int>> p_next;
        twoBreakOnGenome(p, i1, i2, i4, i3, p_next);
        p = p_next;
        result.push_back(p);
    }

}

//TODO: изменить
void read_genome(std::string &line, std::vector<std::vector<int>> &genome) {
    std::vector<int> chromosome;
    std::string elem;
    for (auto ch: line) {
        if (ch == '(') {
            continue;

        } else if (ch != ' ' && ch != ')' && ch != ',') {
            elem += ch;
        } else if (ch == ')') {
            chromosome.push_back(std::atoi(elem.c_str()));
            genome.push_back(chromosome);
            chromosome.clear();
            elem = "";

        } else {
            chromosome.push_back(std::atoi(elem.c_str()));
            elem = "";
        }
    }
}


void twoBreakSorting() {
    std::ifstream input("TwoBreakSortingProblem/inputs/input_1.txt");
    std::string first, second;
    std::vector<std::vector<int>> p, q;
    std::getline(input, first);
    std::getline(input, second);
    read_genome(first, p);
    read_genome(second, q);
    std::vector<std::vector<std::vector<int>>> result;
    shortestRearangment(p, q, result);
    for (auto & line: result) {
        for (auto & l: line) {
            std::cout << "(";
            for (int i = 0; i < l.size(); ++i) {
                if (i!= 0){
                    std::cout <<" ";
                }
                if (l[i] > 0){
                    std::cout << "+";
                }
                std::cout << l[i];
            }
            std::cout <<")";
        }
        std::cout << "\n";
    }
}