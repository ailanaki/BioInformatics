#pragma once

#include <fstream>

void read_b_w(std::string &string, std::vector<std::string> &alphabet,
              std::vector<std::string> &states, std::vector<std::vector<double>> &transition,
              std::vector<std::vector<double>> &emission, std::vector<int> &number_alphabet) {
    std::ifstream input("Baum-Welch/inputs/input_1.txt");
    std::string pass;
    int t;
    input >> t;
    input >> pass;
    input >> pass;
    string = pass;
    input >> pass;
    input >> pass;
    while (pass != "--------") {
        alphabet.push_back(pass);
        input >> pass;
    }

    for (char i: string) {
        for (int j = 0; j < alphabet.size(); ++j) {
            if (i == alphabet[j][0]) {
                number_alphabet.push_back(j);
                break;
            }
        }
    }

    input >> pass;
    while (pass != "--------") {
        states.push_back(pass);
        input >> pass;
    }
    for (int i = 0; i < states.size(); ++i) {
        input >> pass;
    }
    double d_pass;
    for (int i = 0; i < states.size(); ++i) {
        input >> pass;
        std::vector<double> tmp;
        for (int j = 0; j < states.size(); ++j) {
            input >> d_pass;
            tmp.push_back(d_pass);
        }
        transition.push_back(tmp);
    }

    input >> pass;
    for (int i = 0; i < alphabet.size(); ++i) {
        input >> pass;
    }
    for (int i = 0; i < states.size(); ++i) {
        input >> pass;
        std::vector<double> tmp;
        for (int j = 0; j < alphabet.size(); ++j) {
            input >> d_pass;
            tmp.push_back(d_pass);
        }
        emission.push_back(tmp);
    }
}

void get_alpha(std::vector<std::vector<double>> &a, std::vector<std::vector<double>> &b,
               std::vector<int> &o, std::vector<std::vector<double>> &alpha, int n, int j, int t) {
    alpha[j][1] = b[j][o[0]];
    double sum = 0;
    for (int i = 0; i < n; ++i) {
        sum += alpha[i][t - 1] * a[i][j] * b[j][o[t]];
    }
    a[j][t] = sum;
}

void get_beta(std::vector<std::vector<double>> &a, std::vector<std::vector<double>> &b,
              std::vector<int> &o, std::vector<std::vector<double>> &beta, int T, int i, int t, int n) {
    beta[i][T] = 1;
    double sum = 0;
    for (int j = n - 1; j > 0; --j) {
        sum += a[i][j] * b[j][o[t + 1]] * beta[j][t + 1];
    }
    beta[i][t] = sum;
}

void baumWelch() {
    std::string string;
    std::vector<std::string> alphabet, states;
    std::vector<int> number_alphabet;
    std::vector<std::vector<double>> transition, emission;

    read_b_w(string, alphabet, states, transition, emission, number_alphabet);
    auto psy = 123;
    int n = states.size();
    std::vector<double> prob(n, 1 / n);
    std::vector<std::vector<double>> alpha(n, std::vector<double>(n, 0));
    std::vector<std::vector<double>> beta(n, std::vector<double>(n, 0));
    //while (psy > 0) {
        for (int t = 0; t < number_alphabet.size(); ++t) {
            for (int i = 0; i < n; ++i) {
                get_alpha(transition, emission, number_alphabet, alpha, n, i, t);
                get_beta(transition, emission, number_alphabet, beta, number_alphabet.size(), i, t, n);
            }
        }
    //}
}