#include <bits/stdc++.h>

using namespace std;

double scoring_f(int K){
    if(1280 < K)
        return 0;
    else if(384 < K && K <= 1280)
        return 0.3 - 0.1 * ((double)(K - 384)) / (double)(896);
    else if(256 < K && K <= 384)
        return 0.6 - 0.3 * ((double)(K - 256)) / (double)(128);
    else if(215 < K && K <= 256)
        return 1 - 0.40 * ((double)(K - 215)) / (double)(41);
    else if(K <= 215)
        return 1;
    else
        return 1;
}

void exit_WA(std::string message = "") {
    std::cout << "WA"  << endl << "0" << endl << message << endl;
}

void exit_AC(std::string message = "") {
    std::cout << "AC" << endl << "1" << endl << message << endl;
}

void exit_PAR(double score, int total, std::string message = "") { // score should between 0 and 1
    std::cout << "OK" << endl << setprecision(10) << score * (double)total << endl << message << endl;
}

void check(bool res) {
    if (!res)
        exit_WA();
}

namespace checker_stuff{
    vector <bitset<346000>> state;
}

bitset <346000> new_bitset(){
    bitset <346000> tmp;
    tmp.reset();
    return tmp;
}

int main(int argc, char *argv[]) {
    ifstream inf(argv[1]);
    ifstream ans(argv[2]);
    ifstream ouf(argv[3]);
    int K;
    ouf >> K;
    check(0 <= K && K <= 1024);

    for (int i = 0; i < 128; i++){
        checker_stuff::state.push_back(new_bitset());
    }

    vector <vector<int>> all_input;
    for (int i = 0; i < 128; i++){
        all_input.push_back({i});
    }
    for (int i = 0; i < 128; i++){
        for (int j = i + 1; j < 128; j++){
            for (int k = j + 1; k < 128; k++){
                all_input.push_back({i, j, k});
            }
        }
    }

    for (int i = 0; i < (int)all_input.size(); i++){
        for(auto j:all_input[i]){
            checker_stuff::state[j][i] = 1;
        }
    }

    for(int t = 1; t <= K; t++){
        string gate_type;
        ouf >> gate_type;
        if (gate_type == "AND"){
            int g1, g2;
            ouf >> g1 >> g2;
            //g1--, g2--;
            check((0 <= g1) && (g1 < (int)checker_stuff::state.size()));
            check((0 <= g2) && (g2 < (int)checker_stuff::state.size()));
            checker_stuff::state.push_back(new_bitset());
            checker_stuff::state.back() = checker_stuff::state[g1] & checker_stuff::state[g2];
        }
        else if(gate_type == "OR"){
            int g1, g2;
            ouf >> g1 >> g2;
            //g1--, g2--;
            check((0 <= g1) && (g1 < (int)checker_stuff::state.size()));
            check((0 <= g2) && (g2 < (int)checker_stuff::state.size()));
            checker_stuff::state.push_back(new_bitset());
            checker_stuff::state.back() = checker_stuff::state[g1] | checker_stuff::state[g2];
        }
        else if(gate_type == "NOT"){
            int g1;
            ouf >> g1;
            //g1--;
            check((0 <= g1) && (g1 < (int)checker_stuff::state.size()));
            checker_stuff::state.push_back(new_bitset());
            checker_stuff::state.back() = ~checker_stuff::state[g1];
        }
        else{
            exit_WA("Wrong output format");
        }
    }

    string dummy;
    if (ouf >> dummy) {
        exit_WA();
    }

    bool subtask_1 = 1, subtask_2 = 1;

    for (int i = 0; i < (int)all_input.size(); i++){
        if(all_input[i][0] == 0){
            subtask_1 &= (checker_stuff::state.back()[i] == (bool)(all_input[i].size() == 3));
        }
        else{
            subtask_2 &= (checker_stuff::state.back()[i] == (bool)(all_input[i].size() == 3));
        }
    }

    subtask_2 &= subtask_1;

    int st;
    inf >> st;

    if(st == 1){
        if(!subtask_1)
            exit_WA();
        if(K <= 215)
            exit_AC();
        exit_PAR(scoring_f(K), 10);
    }
    if(st == 2){
        if(!subtask_2)
            exit_WA();
        if(K <= 215)
            exit_AC();
        exit_PAR(scoring_f(K), 90);
    }
}
