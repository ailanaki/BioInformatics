#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <fstream>

const std::vector<char> _dna = {'A', 'C', 'G', 'T'};

void makeKmers(const std::string &generate, int k, std::vector<std::string> *kmers) {
    if (generate.size() == k) kmers->push_back(generate);
    else {
        for (auto c : _dna) makeKmers(generate + c, k, kmers);
    }
}

int distance(const std::string &string, const std::string &pattern, int k) {
    int n = 0;
    for (int i = 0; i < string.size() - k + 1; ++i) {
        int count = 0;
        for (int j = 0; j < pattern.size(); ++j) {
            if (pattern[j] == string[i + j]) {
                count++;
            }
        }
        if (count > n) n = count;
        if (n == pattern.size()) return n;
    }
    return n;
}


std::string search(const std::vector<std::string> &strings, const std::vector<std::string> &kmers, int k) {
    std::vector<int> score(kmers.size(), 0);
    for (auto &string :strings) {
        for (int i = 0; i < kmers.size(); ++i) {
            score[i] += distance(string, kmers[i], k);
        }
    }
    auto max = std::max_element(score.begin(), score.end());
    return kmers.at(std::distance(score.begin(), max));
}


int median_string() {
    int k = 0;
    std::vector<std::string> strings, kmers;
    std::ifstream read;
    read.open("MedianString/inputs/input_5.txt");
    read >> k;
    std::string tmp;
    while (!read.eof()) {
        read >> tmp;
        strings.push_back(tmp);
    }
    read.close();
    makeKmers("", k, &kmers);
    std::cout << search(strings, kmers, k);
    return 0;
}
