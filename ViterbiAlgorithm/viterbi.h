#include <vector>
#include <fstream>

void algorithm(std::vector<char> s, std::vector<std::string> p,
               std::vector<char> y, std::vector<std::vector<double>> a, std::vector<std::vector<double>> b,
               std::vector<int> *result){

    int k = s.size();
    int t = y.size();
    std::vector<std::vector<double>> Tstate;
    std::vector<std::vector<double>> Tindex;
    for (int i = 0; i < k; ++i) {
        std::vector<double> tmp;
        for (int j = 0; j < t; ++j) {
            tmp.push_back(0);
        }
        Tindex.push_back(tmp);
        Tstate.push_back(tmp);
    }
    for (int j = 0; j < k; ++j) {
        Tstate[j][0]= b[j][std::abs(y[0] - 'x')] * 0.5;
        Tindex[j][0] = 0;
    }

    for (int i = 1; i < t; ++i) {
        for (int j = 0; j < k; ++j) {
            for (int k_1 = 0; k_1 < k; ++k_1) {
                if (Tstate[k_1][i - 1] * a[k][j] * b[j][y[i]] > Tindex[j][i]){
                    Tindex[j][i] = Tstate[k_1][i - 1] * a[k_1][j] * b[j][y[i]];
                    Tstate[j][i] = Tstate[Tindex[j][i]][i - 1] * a[k_1][j]*b[j][y[i]];
                }
            }

        }
    }
    std::vector<int> answer;
    auto max = Tstate[k - 1][t - 1];
    int ind = k;
    for (int i = 0; i < k; ++i) {
        if (max < Tstate[i][t - 1]){
            max = Tstate[i][t - 1];
            ind = i;
        }
    }
    for (int i = 0; i < t; ++i) {
        answer.push_back(0);
    }
    answer[t - 1] = ind;
    for (int i = t; i >= 2 ; i--) {
        answer[i - 1] = Tindex[answer[i]][i];
    }
    result->clear();
    for (int i : answer) {
        result->push_back(i);
    }
}

void read(std::string * string, std::vector<std::string> * alphabet,
          std::vector<std::string> * states, std::vector<std::vector<double>> * transition,
          std::vector<std::vector<double>> * emission){
    std::ifstream input;
    input.open("ViterbiAlgorithm/inputs/input_1.txt");
    std::string pass;
    input >> pass;
    string->clear();
    string->append(pass);
    input >> pass;
    input >> pass;
    while(pass!="--------"){
        alphabet->push_back(pass);
        input >> pass;
    }

    input >> pass;
    while(pass!="--------"){
        states->push_back(pass);
        input >> pass;
    }
    for (int i = 0; i < states->size(); ++i) {
        input >>pass;
    }
    double d_pass;
    for (int i = 0; i < states->size(); ++i) {
        input >> pass;
        std::vector<double> tmp;
        for (int j = 0; j < states->size() ; ++j) {
            input >> d_pass;
            tmp.push_back(d_pass);
        }
        transition->push_back(tmp);
    }

    input >> pass;

    for (int i = 0; i < alphabet->size(); ++i) {
        input >>pass;
    }

    for (int i = 0; i < states->size(); ++i) {
        input >> pass;
        std::vector<double> tmp;
        for (int j = 0; j < alphabet->size() ; ++j) {
            input >> d_pass;
            tmp.push_back(d_pass);
        }
        emission->push_back(tmp);
    }


}

void vitebi() {
    std::string string;
    std::vector<std::string> alphabet, states;
    std::vector<std::vector<double>> transition, emission;
    read(&string, &alphabet, &states, &transition, &emission);
    std::vector<int> result;
    std::vector<char> s_string, c_states;
    for (char & i : string) {
        s_string.push_back(i);
    }
    for(std::string &i: states){
        c_states.push_back(i[0]);
    }
    algorithm(c_states, alphabet, s_string, transition, emission, &result);
    for (int & i : result) {
        std::cout <<i <<" ";
    }
}
