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
        transition.push_back(std::vector<double>(states.size(), 0));
    }
    for (int i = 0; i < states.size(); ++i) {
        input >> pass;
        std::vector<double> tmp;
        int ind = 0;
        while (pass != states[ind]){
            ind++;
        }
        for (int j = 0; j < states.size(); ++j) {
            input >> d_pass;
            transition[ind][j] = d_pass;
        }
    }

    input >> pass;
    for (int i = 0; i < alphabet.size(); ++i) {
        input >> pass;
    }
    for (int i = 0; i < states.size(); ++i) {
        emission.push_back(std::vector<double>(alphabet.size(), 0));
    }
    for (int i = 0; i < states.size(); ++i) {
        input >> pass;
        auto ind = 0;
        while (pass != states[ind]){
            ind++;
        }
        for (int j = 0; j < alphabet.size(); ++j) {
            input >> d_pass;
            emission[ind][j] = d_pass;
        }
    }
}

void forward(std::vector<std::vector<double>> &fwd, std::vector<std::string> &states, std::vector<int> &number_alphabet,
             std::vector<std::vector<double>> &emission, std::vector<double> &probs,
             std::vector<std::vector<double>> &transition) {
    fwd.clear();
    for (int i = 0; i < states.size(); ++i) {
        std::vector<double> tmp(number_alphabet.size(), 0);
        fwd.push_back(tmp);
    }
    for (int i = 0; i < number_alphabet.size(); ++i) {
        for (int j = 0; j < states.size(); ++j) {
            if (i == 0) {
                fwd[j][i] = emission[j][number_alphabet[i]];
            } else {
                double sum = 0;
                for (int k = 0; k < states.size(); ++k) {
                    sum += fwd[k][i - 1] * emission[j][number_alphabet[i]] * transition[k][j];
                }
                fwd[j][i] = sum;
            }

        }
    }
}

void backward(std::vector<std::vector<double>> &bwd, std::vector<std::string> &states,
              std::vector<int> &number_alphabet, std::vector<std::vector<double>> &emission,
              std::vector<double> &probs, std::vector<std::vector<double>> &transition) {
    bwd.clear();
    for (int i = 0; i < states.size(); ++i) {
        std::vector<double> tmp(number_alphabet.size(), 0);
        bwd.push_back(tmp);
    }

    for (int i = number_alphabet.size() - 1; i >= 0; --i) {
        for (int j = 0; j < states.size(); ++j) {
            if (i == number_alphabet.size() - 1) {
                bwd[j][i] = 1;
            } else {
                double sum = 0;
                for (int k = 0; k < states.size(); ++k) {
                    sum += bwd[k][i + 1] * emission[k][number_alphabet[i + 1]] * transition[j][k];
                }
                bwd[j][i] = sum;
            }

        }
    }


}


void ksi_probs(std::vector<std::vector<double>> &fwd, std::vector<std::vector<double>> &bwd,
               std::vector<std::vector<std::vector<double>>> &si,
               std::vector<std::string> &states, std::vector<int> &number_alphabet,
               std::vector<std::vector<double>> &emission, std::vector<std::vector<double>> &transition) {
    si.clear();
    for (int i = 0; i < states.size(); ++i) {
        std::vector<std::vector<double>> tmp(number_alphabet.size(), std::vector<double>(states.size(), 0));
        si.push_back(tmp);
    }
    double sum = 0;
    for (int i = 0; i < number_alphabet.size() - 1; ++i) {
        for (int j = 0; j < states.size(); ++j) {
            for (int k = 0; k < states.size(); ++k) {
                si[j][i][k] = (fwd[j][i] * bwd[k][i + 1] * transition[j][k] * emission[k][number_alphabet[i + 1]]);
                sum += si[j][i][k];
            }
        }
    }
    for (int i = 0; i < number_alphabet.size() - 1; ++i) {
        for (int j = 0; j < states.size(); ++j) {
            for (int k = 0; k < states.size(); ++k) {
                si[j][i][k] /= sum;
            }
        }
    }
}


void gamma_probs(std::vector<std::vector<double>> &fwd, std::vector<std::vector<double>> &bwd,
                 std::vector<std::vector<double>> &gamma, std::vector<std::string> &states,
                 std::vector<int> &number_alphabet) {
    gamma.clear();
    for (int i = 0; i < states.size(); ++i) {
        std::vector<double> tmp(number_alphabet.size(), 0);
        gamma.push_back(tmp);
    }
    double sum = 0;
    for (int i = 0; i < number_alphabet.size(); ++i) {
        for (int j = 0; j < states.size(); ++j) {
            gamma[j][i] = (fwd[j][i] * bwd[j][i]);
            sum += gamma[j][i];
        }
    }
    for (int i = 0; i < number_alphabet.size(); ++i) {
        for (int j = 0; j < states.size(); ++j) {
            gamma[j][i] /= sum;
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
    std::vector<double> probs(n, (double) 1 / n);
    std::vector<std::vector<double>> fwd, bwd, gamma;
    std::vector<std::vector<std::vector<double>>> ksi;
    for (int t = 0; t < 100; ++t) {
        forward(fwd, states, number_alphabet, emission, probs, transition);
        backward(bwd, states, number_alphabet, emission, probs, transition);
        ksi_probs(fwd, bwd, ksi, states, number_alphabet, emission, transition);
        gamma_probs(fwd, bwd, gamma, states, number_alphabet);
        std::vector<std::vector<double>> a(states.size(), std::vector<double>(states.size(), 0));
        std::vector<std::vector<double>> b(states.size(), std::vector<double>(alphabet.size(), 0));
        for (int j = 0; j < states.size(); ++j) {
            double sum = 0;
            for (int i = 0; i < number_alphabet.size(); ++i) {
                for (int k = 0; k < states.size(); ++k) {
                    sum += ksi[j][i][k];
                }
            }
            for (int i = 0; i < states.size(); ++i) {
                for (int k = 0; k < number_alphabet.size() - 1; ++k) {
                    a[j][i] += ksi[j][k][i];
                }
                if (sum == 0) {
                    a[j][i] = 0;
                } else {
                    a[j][i] = a[j][i] / sum;
                }
            }
        }
        for (int j = 0; j < states.size(); ++j) {
            for (int i = 0; i < alphabet.size(); ++i) {
                std::vector<int> ind;
                double den = 0, num = 0;
                for (int k = 0; k < number_alphabet.size(); ++k) {
                    if (number_alphabet[k] == i) {
                        ind.push_back(k);
                    }
                    den += gamma[j][k];
                }

                for (auto id: ind) {
                    num += gamma[j][id];

                }
                if (den == 0) {
                    b[j][i] = 0;
                } else {
                    b[j][i] = num / den;
                }
            }
        }
        transition = a;
        emission = b;
    }
    std::cout << " ";
    std::cout.setf(std::ios::fixed);
    std::cout.precision(3);

    for (int i = 0; i < states.size(); ++i) {
        std::cout << states[i] << " ";
    }
    std::cout << "\n";

    for (int i = 0; i < transition.size(); ++i) {
        std::cout << states[i] << " ";
        for (int j = 0; j < transition[i].size(); ++j) {
            std::cout << transition[i][j] << " ";
        }
        std::cout << "\n";
    }
    std::cout << "--------\n";
    std::cout << " ";
    for (int i = 0; i < alphabet.size(); ++i) {
        std::cout << alphabet[i] << " ";
    }
    std::cout << "\n";
    for (int i = 0; i < emission.size(); ++i) {
        std::cout << states[i] << " ";
        for (int j = 0; j < emission[i].size(); ++j) {
            std::cout << emission
            [i][j] << " ";
        }
        std::cout << "\n";
    }
    std::cout << "\n";

}