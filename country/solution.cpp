#include <bits/stdc++.h>
typedef long long ll;
 
using namespace std;
 
const int N = 1'000'001;
 
int tot[N];
int cur[N];
int same_ct[N];
int ls[N], rs[N];
 
pair<long long, long long> get_sure_pairs(vector<int> ranks) {
    int n = ranks.size();
    ll same = 0, cans = 0;
    for(int i = 0; i <= n; i++) {
        tot[i] = cur[i] = same_ct[i] = ls[i] = rs[i] = 0;
    }
    for(int i = 0; i < n; i++) {
        int x = ranks[i];
        if(x == 0) {
            tot[0]++;
            cur[0]++;
            if(cur[0] == 1) {
                ls[0] = tot[0] - 1;
            }
            rs[0] = tot[0];
        }
        else {
            cur[x-1]--;
            cur[x]++;
            tot[x]++;
            // same
            if(cur[x-1] == 0) {
                same_ct[x] = same_ct[x-1] + 1;
                same_ct[x-1] = 0;
                same += same_ct[x];
            }
            else {
                same_ct[x-1] = -1;
            }
            // cans (short for can same)
            if(cur[x] == 1) {
                ls[x] = ls[x-1] + tot[x] - 1;
            }
            rs[x] = rs[x-1] + tot[x];
            cans += rs[x-1] - ls[x-1];
        }
    }
    return {same, ((long long)n * n - n) / 2 - cans};
}
 
long long count_same_country(int N, vector<int> country_rank) {
    return get_sure_pairs(country_rank).first;
}
 
long long count_diff_country(int N, vector<int> country_rank) {
    return get_sure_pairs(country_rank).second;
}