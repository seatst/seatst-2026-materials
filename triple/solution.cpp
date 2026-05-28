#include <bits/stdc++.h>

using namespace std;


int id = 0;

int add_not(int in_1){
  cout << "NOT " << in_1 << '\n';
  return ++id;
}

int add_or(int in_1, int in_2){
  cout << "OR " << in_1 << ' ' << in_2 << '\n';
  return ++id;
}

int add_and(int in_1, int in_2){
  cout << "AND " << in_1 << ' ' << in_2 << '\n';
  return ++id;
}

int construct_3N(vector <int> input){
    vector <int> pre, X, all_X;
    
    pre.push_back(input[0]);
    
    for (int i = 1; i < (int)input.size() - 1; i++){
        pre.push_back(add_or(pre.back(), input[i]));
    }

    for (int i = 1; i < (int)input.size(); i++){
        X.push_back(add_and(input[i], pre[i - 1]));
    }

    all_X.push_back(X[0]);

    for (int i = 2; i < (int)input.size(); i++){
        all_X.push_back(add_or(all_X.back(), X[i - 1]));
    }
    return all_X.back();
}

int construct_group(vector <int> input, int b){
  vector <vector<int>> grid;
  for(auto i:input){
    if(grid.empty() || (int)grid.back().size() == b)
      grid.push_back({i});
    else
      grid.back().push_back(i);
  }
  vector <int> vert_input, hori_input;
  for(auto i:grid){
    vector <int> pre = {i[0]};
    for(int j = 1; j < (int)i.size(); j++){
      pre.push_back(add_or(i[j], pre.back()));
    }
    vert_input.push_back(pre.back());
  }
  if((int)grid.size() == 1)
    hori_input = grid[0];
  else{
    for(int i = 0; i < b; i++){
      vector <int> pre = {grid[0][i]};
      for(int j = 1; j < (int)grid.size(); j++){
        if((int)grid[j].size() > i)
          pre.push_back(add_or(grid[j][i], pre.back()));
      }
      hori_input.push_back(pre.back());
    }
  }
  return add_or(construct_3N(vert_input), construct_3N(hori_input));
}

void construct_circuit(int N){
  vector <int> input;
  int merge = N - 64;
  for(int i = 1; i <= merge; i++){
    input.push_back(add_or(2 * i - 1, 2 * i));
  }
  for(int i = 2 * merge + 1; i <= N; i++){
    input.push_back(i);
  }
  construct_group(input, 8);
  return;
}

int main(){
  cout << "215\n";
  id = 128;
  construct_circuit(128);
  return 0;
}
