#pragma once
#include <iostream>
#include <utility>
#include <vector>
#include <algorithm>
#include "../suffArray.h"
#include <fstream>

void suffix_array() {
    std::ifstream input;
    input.open("SuffixArray/inputs/input_1.txt");
    std::string str;
    std::vector<int> p;
    std::vector<int> lcp;
    std::vector<std::string> strings;
    std::string line;
    input >> str;
    tolower(str);
    suffArray(p, str);
    while (std::getline(input, line)) {
        if (!line.empty()) {
            strings.push_back(line);
        }
    }
    std::vector<int> answer;
    for (auto string:strings){
        tolower(string);
        auto pair =  elementary_search(str, string, p);
        for (int i = ++pair.first; i < pair.second; ++i) {
            answer.push_back(p[i]);
        }
    }
    std::sort(answer.begin(), answer.end());
    for (auto ind: answer){
        std::cout << ind <<" ";
    }
}

