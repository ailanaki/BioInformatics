#pragma once

#include <vector>
#include <string>
#include <fstream>
#include <iostream>


using matrix = std::vector<std::vector<int>>;
using string = std::string;

int correct(char c1) {
    if (c1 == 'A') return 'A';
    if (c1 <= 'I') return c1 - 1;
    if (c1 <= 'N') return c1 - 2;
    if (c1 <= 'T') return c1 - 3;
    if (c1 <= 'W') return c1 - 4;
    return c1 - 5;
}

int match(char c1, char c2, matrix &blosum) {
    return blosum[correct(c1) - 'A'][correct(c2) - 'A'];
}


matrix makeMatrixs(string s1, string s2, matrix &blosum, matrix &I, matrix &D) {
    matrix M;
    int n = s1.length();
    int m = s2.length();
    for (int i = 0; i < n + 1; ++i) {
        std::vector<int> tmp;
        tmp.reserve(m + 1);
        for (int j = 0; j < m + 1; ++j) {
            tmp.push_back(0);
        }
        M.push_back(tmp);
        I.push_back(tmp);
        D.push_back(tmp);
    }
    for (int i = 1; i < n + 1; ++i) {
        I[i][0] = i * (-11) - 1;
    }
    for (int i = 1; i < m + 1; ++i) {
        D[0][i] = i * (-11) - 1;
    }

    for (int i = 1; i < n + 1; ++i) {
        for (int j = 1; j < m + 1; ++j) {
            I[i][j] = std::max(I[i - 1][j] - 1,
                               M[i - 1][j] - 11);
            D[i][j] = std::max(D[i][j - 1] - 1,
                               M[i][j - 1] - 11);

            M[i][j] = std::max(M[i - 1][j - 1] + match(s1[i - 1], s2[j - 1], blosum),
                               std::max(I[i][j], D[i][j]));
        }
    }
    return M;
}


void print(matrix M, matrix I, matrix D, string s1, string s2,
           std::string &builder1, std::string &builder2, matrix &blosum) {
    auto i = s1.length();
    auto j = s2.length();
    auto score = std::max(M[i][j], std::max(I[i][j], D[i][j]));
    char cur = 'M';
    if (score == I[i][j]) {
        cur = 'I';
    } else if (score == D[i][j]) {
        cur = 'D';
    }
    std::cout<< score<<"\n";
    while (j > 0 && i > 0) {
        switch (cur) {
            case 'M':
                if (M[i][j] == D[i][j]) {
                    cur = 'D';
                } else if (M[i][j] == I[i][j]) {
                    cur = 'I';
                } else if (M[i][j] == M[i - 1][j - 1] + match(s1[i - 1], s2[j - 1], blosum)) {
                    builder1 = s1[i - 1] + builder1;
                    builder2 = s2[j - 1] + builder2;
                    cur = 'M';
                    i--;
                    j--;
                } else {
                    std::cout << "error";
                    return;
                }
                break;
            case 'D':
                if (D[i][j] == D[i][j - 1] - 1) {
                    builder1 = "-" + builder1;
                    builder2 = s2[j - 1] + builder2;
                    cur = 'D';
                    j--;
                } else if (D[i][j] == M[i][j - 1] - 11) {
                    builder1 = "-" + builder1;
                    builder2 = s2[j - 1] + builder2;
                    cur = 'M';
                    j--;
                } else {
                    std::cout << "error";
                    return;
                }
                break;
            case 'I':
                if (I[i][j] == I[i - 1][j] - 1) {
                    builder1 = s1[i - 1] + builder1;
                    builder2 = "-" + builder2;
                    i--;
                    cur = 'I';
                } else if (I[i][j] == M[i - 1][j] - 11) {
                    builder1 = s1[i - 1] + builder1;
                    builder2 = "-" + builder2;
                    cur = 'M';
                    i--;
                } else {
                    std::cout << "error";
                    return;
                }
                break;
        }
    }
    while (i > 0) {
        builder1 = s1[i - 1] + builder1;
        builder2 = "-" + builder2;
        i--;
    }
    while (j > 0) {

        builder1 = "-" + builder1;
        builder2 = s2[j - 1] + builder2;
        j--;
    }
}

matrix blosumMatrix() {
    matrix blosum;
    std::ifstream read;
    read.open("AlignTwoStringsUsingAffineGapPenalty/blosum");
    for (int i = 0; i < 20; ++i) {
        std::vector<int> tmp;
        for (int j = 0; j < 20; ++j) {
            int c;
            read >> c;
            tmp.push_back(c);
        }
        blosum.push_back(tmp);
    }
    read.close();
    return blosum;
}

int align() {
    matrix blosum = blosumMatrix();
    matrix I, D;
    std::string s1, s2;
    std::ifstream read;
    read.open("AlignTwoStringsUsingAffineGapPenalty/inputs/input_5.txt");
    read >> s1;
    read >> s2;
    std::string answer1, answer2;
    read.close();
    matrix result = makeMatrixs(s1, s2, blosum, I, D);
    print(result, I, D, s1, s2, answer1, answer2, blosum);
    std::cout << answer1 << "\n" << answer2;
    return 0;
}