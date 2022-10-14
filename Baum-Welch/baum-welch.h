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

void forward(std::vector<std::vector<double>> &fwd, std::vector<std::string> &states, std::vector<int> &number_alphabet,
             std::vector<std::vector<double>> &emission, std::vector<double> &probs,
             std::vector<std::vector<double>> &transition, std::vector<double> &norms) {
    fwd.clear();
    for (int i = 0; i < states.size(); ++i) {
        std::vector<double> tmp(number_alphabet.size(), 0);
        fwd.push_back(tmp);
    }
    for (int i = 0; i < states.size(); ++i) {
        fwd[i][0] = probs[i] * emission[i][number_alphabet[0]];
    }

    auto sum_alpha = 0.0;
    for (int i = 0; i < states.size(); ++i) {
        sum_alpha += fwd[i][0];
    }

    norms[0] = sum_alpha;

    for (int i = 0; i < states.size(); ++i) {
        fwd[i][0] /= sum_alpha;
    }

    for (int t = 0; t < number_alphabet.size() - 1; ++t) {
        for (int i = 0; i < states.size(); ++i) {
            auto s = 0.0;
            for (int j = 0; j < states.size(); ++j) {
                s += fwd[j][t] * transition[j][i];

            }
            s *= emission[i][number_alphabet[t + 1]];
            fwd[i][t + 1] = s;
        }
        sum_alpha = 0.0;
        for (int i = 0; i < states.size(); ++i) {
            sum_alpha += fwd[i][t + 1];
        }
        norms[t + 1] = sum_alpha;

        for (int i = 0; i < states.size(); ++i) {
            fwd[i][t + 1] /= sum_alpha;
        }
    }
}

void
backward(std::vector<std::vector<double>> &bwd, std::vector<std::string> &states,
         std::vector<int> &number_alphabet, std::vector<std::vector<double>> &emission,
         std::vector<double> &norms, std::vector<std::vector<double>> &transition) {
    for (int i = 0; i < states.size(); ++i) {
        bwd[i][number_alphabet.size() - 1] = 1;
    }
    for (int t = number_alphabet.size() - 2; t > 0; t--) {
        for (int i = 0; i < states.size(); ++i) {
            auto s = 0.0;
            for (int j = 0; j < states.size(); ++j) {
                s += bwd[j][t + 1] * transition[i][j] * emission[j][number_alphabet[t + 1]];
            }
            bwd[i][t] = s;
        }
        for (int i = 0; i < states.size(); ++i) {
            bwd[i][t] /= norms[t + 1];
        }
    }
}


void si_probs(std::vector<std::vector<double>> &fwd, std::vector<std::vector<double>> &bwd,
              std::vector<std::vector<std::vector<double>>> &si,
              std::vector<std::string> &states, std::vector<int> &number_alphabet,
              std::vector<std::vector<double>> &emission, std::vector<std::vector<double>> &transition) {
    si.clear();
    for (int i = 0; i < states.size(); ++i) {
        std::vector<std::vector<double>> tmp(number_alphabet.size(), std::vector<double>(states.size(), 0));
        si.push_back(tmp);
    }
    for (int i = 0; i < number_alphabet.size() - 1; ++i) {
        double val = 0;
        for (auto &j: fwd) {
            val += j[i];
        }
        for (int j = 0; j < states.size(); ++j) {
            for (int k = 0; k < states.size(); ++k) {
                si[j][i][k] =
                        (fwd[j][i] * bwd[k][i + 1] * transition[j][k] * emission[k][number_alphabet[i + 1]]) / val;
            }
        }
    }
}


void gamma_probs(std::vector<std::vector<double>> &fwd, std::vector<std::vector<double>> &bwd,
                 std::vector<std::vector<double>> &gamma,
                 std::vector<std::string> &states, std::vector<int> &number_alphabet,
                 std::vector<std::vector<double>> &emission) {
    gamma.clear();
    for (int i = 0; i < states.size(); ++i) {
        std::vector<double> tmp(number_alphabet.size(), 0);
        gamma.push_back(tmp);
    }
    for (int i = 0; i < number_alphabet.size(); ++i) {
        double val = 0;
        for (auto &j: fwd) {
            val += j[i];
        }
        for (int j = 0; j < states.size(); ++j) {
            gamma[j][i] = (fwd[j][i] * bwd[j][i]) / val;
        }
    }
}

void baumWelch() {
    std::string string;
    std::vector<std::string> alphabet, states;
    std::vector<int> number_alphabet;
    std::vector<std::vector<double>> transition, emission;

    read_b_w(string, alphabet, states, transition, emission, number_alphabet);
    int n = states.size();
    double diff = 0;
    std::vector<double> probs(n, (double) 1 / n);
    std::vector<double> norms(n, 0);
    std::vector<std::vector<double>> fwd(states.size(), std::vector<double>(number_alphabet.size(), 0)), bwd(states.size(), std::vector<double>(number_alphabet.size(), 0)), gamma(states.size(), std::vector<double>(number_alphabet.size(), 0));
    std::vector<std::vector<std::vector<double>>> si(number_alphabet.size(), std::vector<std::vector<double>>(states.size(), std::vector<double>(states.size(), 0)));
    for (int it = 0; it < 2000; ++it) {
        forward(fwd, states, number_alphabet, emission, probs, transition, norms);
        backward(bwd, states, number_alphabet, emission, norms, transition);

        for (int t = 0; t < number_alphabet.size() - 1; ++t) {
            for (int i = 0; i < states.size(); ++i) {
                gamma[i][t] = fwd[i][t] * bwd[i][t];
            }
        }
        for (int t = 0; t < number_alphabet.size() - 1; t++) {
            for (int i = 0; i < states.size(); ++i) {
                for (int j = 0; j < states.size(); ++j) {
                    si[t][i][j] = fwd[i][t] * bwd[j][t + 1] * transition[i][j] * emission[j][number_alphabet[t + 1]];
                    si[t][i][j] /= norms[t + 1];
                }
            }
        }


        std::vector<std::vector<double>> a(states.size(), std::vector<double>(states.size(), 0));
        std::vector<std::vector<double>> b(states.size(), std::vector<double>(alphabet.size(), 0));
        for (int i = 0; i < states.size(); ++i) {
            probs[i] = gamma[i][0];
        }
        for (int i = 0; i < states.size(); ++i) {
            auto sum = 0.0;
            for (int t = 0; t < number_alphabet.size(); ++t) {
                sum += gamma[i][t];
            }
            for (int j = 0; j < states.size(); ++j) {
                auto val = 0.0;
                for (int t = 0; t < number_alphabet.size(); ++t) {
                    val += si[t][i][j];
                }
                a[i][j] = sum / val;
            }
        }

        for (int i = 0; i < states.size(); ++i) {
            for (int j = 0; j < alphabet.size(); ++j) {
                auto sum = 0.0;
                auto del = 0.0;
                for (int t = 0; t < number_alphabet.size(); ++t) {
                    if (number_alphabet[t] == j) { sum += gamma[i][t]; }
                    del += gamma[i][t];
                }
                b[i][j] = sum / del;
            }
        }
        transition = a;
        emission = b;

    }

    for (int i = 0; i < transition.size(); ++i) {
        for (int j = 0; j < transition[i].size(); ++j) {
            std::cout << transition[i][j] << " ";
        }
        std::cout << "\n";
    }
    std::cout << "\n";

    for (int i = 0; i < emission.size(); ++i) {
        for (int j = 0; j < emission[i].size(); ++j) {
            std::cout << emission
            [i][j] << " ";
        }
        std::cout << "\n";
    }
    std::cout << "\n";

}