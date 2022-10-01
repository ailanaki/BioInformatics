#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <time.h>
#include <map>

double pr(const std::vector<std::vector<double>> &P, const std::string &string) {
    double answer = 1;
    for (int i = 0; i < string.size(); ++i) {
        char c = string[i];
        int ind;
        for (int j = 0; j < 4; ++j) {
            if (_dna[j] == c) ind = j;
        }
        answer *= P[ind][i];
    }
    return answer;
}

std::string mostProbable(std::string genome, int k, std::vector<std::vector<double>> profile) {
    float score = 0;
    std::string bestKmer;
    for (int i = 0; i < 1 + genome.size() - k; ++i) {
        auto kmer = genome.substr(i, k);
        auto kmerScore = pr(profile, kmer);
        if (score < kmerScore) {
            score = kmerScore;
            bestKmer = kmer;
        }
    }
    return bestKmer;
}

std::vector<std::vector<double>> buildProfileMatrix(std::vector<std::string> motifs) {
    std::vector<std::vector<int>> F(4, std::vector<int>(motifs[0].size(), 0));
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < motifs[0].size(); ++j) {
            for (int k = 0; k < motifs.size(); ++k) {
                if (_dna[i] == motifs[k][j]) F[i][j] += 1;
            }
        }
    }
    for (int i = 0; i < F.size(); ++i) {
        for (int j = 0; j < F[0].size(); ++j) {
            F[i][j] += 1;
        }
    }
    std::vector<std::vector<double>> P(4, std::vector<double>(F[0].size(), 0));
    for (int i = 0; i < F.size(); ++i) {
        for (int j = 0; j < F[0].size(); ++j) {
            P[i][j] = (float) F[i][j] / motifs.size();
        }
    }
    return P;
}
std::string consensus(const std::vector<std::vector<double>> &profile , int len) {
    std::string answer;
    float score = 0;
    int sym = 0;
    for (int i = 0; i < len; ++i) {
        score = 0;
        for (int j = 0; j < profile.size(); ++j) {
            if (profile[j][i] > score) {
                score = profile[j][i];
                sym = j;
            }
        }
        answer += _dna[sym];
    }
    return answer;
}

int score(std::vector<std::string> motifs) {
    int answer = 0;
    auto t = motifs.size();
    auto k = motifs[0].size();
    auto best = consensus(buildProfileMatrix(motifs), k);
    for (int j = 0; j < t; ++j) {
        for (int i = 0; i < k; ++i) {
            if (motifs[j][i] != best[i]) answer++;
        }
    }
    return answer;
}

std::vector<std::string> randomizedMotifSearch(std::vector<std::string> DNAs, int k, int t) {
    std::vector<std::string> motifs, bestMotifs;
    for (auto dna : DNAs) {
        int randNumber = rand() % (dna.length() - k + 1);
        bestMotifs.push_back(dna.substr(randNumber, k));
    }
    motifs = bestMotifs;
    std::vector<std::vector<double>> profile;
    do {
        profile = buildProfileMatrix(motifs);
        std::vector<std::string> newMotifs;
        for (auto dna : DNAs) {
            newMotifs.push_back(mostProbable(dna, k, profile));
        }
        motifs = newMotifs;
        if (score(motifs) < score(bestMotifs)) {
            bestMotifs = motifs;
        }
        else {
            return bestMotifs;
        }

    } while (true);
}

int motif_search() {
    std::ifstream input;
    input.open("RandomizedMotifSearch/inputs/input_1.txt");
    srand((unsigned)time(NULL));
    int k, t;
    input >> k >> t;
    std::vector<std::string> strings;
    std::string line;
    while (std::getline(input, line)) {
        if (!line.empty()) {
            strings.push_back(line);
        }
    }
    std::vector<std::string> bestMotifs;
    for (int i =  0; i < 1000; ++i) {
        std::vector<std::string> motifs = randomizedMotifSearch(strings, k, t);
        if (i == 0)
            bestMotifs = motifs;

        if (score(motifs) < score(bestMotifs)) {
            bestMotifs = motifs;
        }
    }

    for (auto s : bestMotifs){
        std::cout << s << std::endl;
    }
    input.close();
    return 0;
}
