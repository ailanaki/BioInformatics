#pragma once
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <algorithm>

void split_string(const std::string &str, std::vector<std::string> *k_mers, int len) {
    for (int i = 0; i < str.length() - len + 1; ++i) {
        k_mers->push_back(str.substr(i, len));
    }
}

int hamming(int k, const std::string &k_mer, const std::string &pattern) {
    int i = 0;
    while (k != -1 && i < pattern.length()) {
        if (k_mer[i] != pattern[i]) {
            k--;
        }
        i++;
    }
    if (k >= 0) return true;
    return false;
}

void find_pattern(std::vector<std::string> *k_mers, const std::string &pattern,
                  std::vector<int> *result, int k) {
    for (int i = 0; i < k_mers->size(); ++i) {
        std::string k_mer = k_mers->at(i);
        if (hamming(k, k_mer, pattern)){
            result->push_back(i);
        }
    }
}

void search(std::vector<int> *result, const std::vector<std::string> &patterns,
            const std::string &str, int k) {
    std::vector<std::string> k_mers;
    int len = 0;
    for (const auto& pattern: patterns){
        if (pattern.length() > len){
            len = pattern.length();
            k_mers.clear();
            split_string(str, &k_mers, len);
        }
        find_pattern(&k_mers, pattern, result, k);
    }
}

void read_line(std::string line, std::vector<std::string> *old_patterns) {
    int i = 0;
    std::string pattern = "";
    while (i < line.length()) {
        if (std::isspace(line[i])) {
            old_patterns->push_back(pattern);
            pattern = "";
        } else {
            pattern = pattern + line[i];
        }
        i++;
    }
    if (!std::empty(pattern))old_patterns->push_back(pattern);
}

struct Comp{
    bool operator()(std::string& a, std::string& b){
        return a.length() < b.length();
    }
};
int multiple_approximate_pattern_matching() {
    std::vector<std::string> patterns;
    std::vector<int> p;
    std::ifstream input;
    input.open("AllApproximateOccurrences/inputs/rosalind_ba9o.txt");
    std::string str;
    input >> str;
    std::string line;
    std::getline(input, line);
    std::getline(input, line);
    read_line(line, &patterns);
    int k;
    input >> k;
    std::vector<int> answer;
    std::sort(patterns.begin(), patterns.end(), Comp());
    search(&answer, patterns, str, k);
    std::sort(answer.begin(), answer.end());
    for (auto ind: answer) {
        std::cout << ind << " ";
    }
    return 0;
};
