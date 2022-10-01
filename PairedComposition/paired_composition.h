#include <iostream>
#include <fstream>
#include <string>
#include <utility>
#include <vector>
#include <map>


struct Node {
    std::string prefix1, prefix2, suffix1, suffix2;
    std::vector<int> next_node;

    Node(std::string prefix1, std::string prefix2, std::string suffix1, std::string suffix2,
         std::vector<int> nextNode) : prefix1(std::move(prefix1)), prefix2(std::move(prefix2)),
                                      suffix1(std::move(suffix1)), suffix2(std::move(suffix2)),
                                      next_node(std::move(nextNode)) {}

    std::pair<std::string, std::string> getPrefix() {
        return std::pair(prefix1, prefix2);
    }

    std::pair<std::string, std::string> getSuffix() {
        return std::pair(suffix1, suffix2);
    }

};

std::vector<char> reconstruction(std::vector<Node> *graph, std::vector<char> *result, int k, int d, int ind, Node now) {
    std::vector<char> answer(*result);
    std::vector<char> mAnswer;
    answer[ind + k - 1] = now.getSuffix().first[k - 2];
    answer.push_back(now.getSuffix().second[k - 2]);
    if (answer[ind + k] == 'X' && now.next_node.empty()) {
        answer.clear();
        return answer;
    }
    mAnswer = answer;
    for (auto &i : now.next_node) {
        auto tmp = reconstruction(graph, &answer, k, d, ind + 1, graph->at(i));
        if (mAnswer.size() < tmp.size()) mAnswer = tmp;
    }
    return mAnswer;
}


std::vector<char> reconstruction(std::vector<Node> *graph, std::vector<char> result, int k, int d) {
    std::vector<char> mAnswer;
    for (auto node: *graph) {
        for (int i = 0; i < 2 * k + d; ++i) {
            result.push_back('X');
        }
        for (int i = 0; i < k - 1; ++i) {
            result[i] = node.getPrefix().first[i];
        }
        result[k - 1] = node.getSuffix().first[k - 2];
        for (int i = 0; i < k - 1; ++i) {
            result[k + d + i] = node.getPrefix().second[i];
        }
        result[2 * k + d - 1] = node.getSuffix().second[k - 2];
        if (!node.next_node.empty()) {
            for (int i = 0; i < node.next_node.size(); ++i) {
                auto next = node.next_node.at(0);
                auto tmp = reconstruction(graph, &result, k, d, 1, graph->at(next));
                if (tmp.size() > mAnswer.size()) mAnswer = tmp;
            }
        }
        result.clear();
    }
    return mAnswer;
}

int paired_composition() {
    std::ifstream input;
    input.open("PairedComposition/inputs/input_3.txt");
    int k, d;
    input >> k >> d;
    std::string line;
    std::vector<Node> graph;
    while (std::getline(input, line)) {
        if (!line.empty()) {
            int i = 0;
            std::string index;
            while (line[i] != '|') {
                index += line[i];
                i++;
            }
            i++;
            std::string str;
            while (i < line.size()) {
                str += line[i];
                i++;
            }
            std::string prefix1 = index.substr(0, index.size() - 1), suffix1 = index.substr(1, index.size() - 1);
            std::string prefix2 = str.substr(0, index.size() - 1), suffix2 = str.substr(1, index.size() - 1);
            int x = -1;
            Node node(prefix1, prefix2, suffix1, suffix2, *new std::vector<int>());
            for (int j = 0; j < graph.size(); ++j) {
                if (graph[j].getSuffix() == node.getPrefix()) {
                    x = j;
                }
                if (graph[j].getPrefix() == node.getSuffix()) {
                    node.next_node.push_back(j);
                }
            }
            if (x != -1) graph[x].next_node.push_back(graph.size());
            graph.push_back(node);

        }
    }
    input.close();
    auto rec = reconstruction(&graph, *new std::vector<char>(), k, d);
    for (int i = 0; i < rec.size(); ++i) {
        std::cout << rec[i];
    }
    return 0;
}

