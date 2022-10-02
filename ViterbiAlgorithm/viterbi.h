#pragma once
#include <vector>
#include <fstream>

void algorithm(std::vector<char> &s,
               std::vector<int> &y, std::vector<std::vector<double>> &a, std::vector<std::vector<double>> &b,
               std::vector<int> &result) {
    auto k = s.size();
    auto t = y.size();
    std::vector<std::vector<double>> tState;
    std::vector<std::vector<int>> tIndex;
    for (int i = 0; i < k + 1; ++i) {
        std::vector<int> tmp(t + 1, 0);
        std::vector<double> tmp_d(t + 1, 0);
        tState.push_back(tmp_d);
        tIndex.push_back(tmp);
    }
    for (int j = 1; j <= k; ++j) {
        tState[j][1] = b[j - 1][0] / k;
    }
    for (int i = 2; i <= t; ++i) {
        for (int j = 1; j < k + 1; ++j) {
            int d = 0;
            for (int l = 1; l < k + 1; ++l) {
                auto x = tState[l][i - 1] * a[l - 1][j - 1] * b[j - 1][y[i - 1]];
                if (x > tState[j][i]) {
                    d = l;
                    tState[j][i] = x;
                }
            }
            tIndex[j][i] = d;
        }
    }
    int k_1 = 0;
    double max = -1;
    for (int i = 0; i <= t + 1; ++i) {
        result.push_back(0);
    }
    for (int i = 1; i <= k; ++i) {
        if (tState[i][t] > max) {
            k_1 = i;
            max = tState[i][t];
        }
    }
    result[t + 1] = k_1 ;
    for (int i = (int) t + 1; i >= 1; i--) {
        result[i - 1] = tIndex[result[i]][i - 1];
    }
}

void read(std::string *string, std::vector<std::string> *alphabet,
          std::vector<std::string> *states, std::vector<std::vector<double>> *transition,
          std::vector<std::vector<double>> *emission, std::vector<int> *number_alphabet, const std::string& read = "ViterbiAlgorithm/inputs/input_3.txt") {
    std::ifstream input(read);
    std::string pass;
    input >> pass;
    string->clear();
    string->append(pass);
    input >> pass;
    input >> pass;
    while (pass != "--------") {
        alphabet->push_back(pass);
        input >> pass;
    }

    for (char i: *string) {
        for (int j = 0; j < alphabet->size(); ++j) {
            if (i == alphabet->at(j)[0]) {
                number_alphabet->push_back(j);
                break;
            }
        }
    }

    input >> pass;
    while (pass != "--------") {
        states->push_back(pass);
        input >> pass;
    }
    for (int i = 0; i < states->size(); ++i) {
        input >> pass;
    }
    double d_pass;
    for (int i = 0; i < states->size(); ++i) {
        input >> pass;
        std::vector<double> tmp;
        for (int j = 0; j < states->size(); ++j) {
            input >> d_pass;
            tmp.push_back(d_pass);
        }
        transition->push_back(tmp);
    }

    input >> pass;

    for (int i = 0; i < alphabet->size(); ++i) {
        input >> pass;
    }

    for (int i = 0; i < states->size(); ++i) {
        input >> pass;
        std::vector<double> tmp;
        for (int j = 0; j < alphabet->size(); ++j) {
            input >> d_pass;
            tmp.push_back(d_pass);
        }
        emission->push_back(tmp);
    }


}

void vitebi() {
    std::string string;
    std::vector<std::string> alphabet, states;
    std::vector<int> number_alphabet;
    std::vector<std::vector<double>> transition, emission;
    read(&string, &alphabet, &states, &transition, &emission, &number_alphabet);
    std::vector<int> result;
    std::vector<char> c_string, c_states;
    for (char &i: string) {
        c_string.push_back(i);
    }
    for (std::string &i: states) {
        c_states.push_back(i[0]);
    }
    algorithm(c_states, number_alphabet, transition, emission, result);
    for (int i = 2; i < result.size(); ++i) {
        std::cout << states[result.at(i) - 1];
    }
}
