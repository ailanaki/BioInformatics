#pragma once
#include <string>
#include "../suffArray.h"
#include "vector"
#include <iostream>
#include <fstream>

int longest_repeat() {
    std::ifstream input;
    input.open("LongestRepeat/inputs/input_1.txt");
    std::string str;
    input >> str;
    tolower(str);
    std::string tmp;
    std::vector<int> suff;
    std::vector<int> lcp;
    suffArray(suff, str);
    LCP(lcp, suff, str);
    int n = str.length();
    int max = 0, maxi = -1;
    for (int i = 0; i < n; ++i) {
        if (lcp[i] > max) {
            max = lcp[i];
            maxi = suff[i];
        }
    }
    if (maxi != -1) {
        for (int i = maxi; i < max + maxi; ++i) {
            std::cout << char(std::toupper(str[i]));
        }
    } else {
        std::cout << "";
    }
    return 0;
}