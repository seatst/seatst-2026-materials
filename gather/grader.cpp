#include <bits/stdc++.h>
#include "gather.h"

using namespace std;

namespace gRadEr_STufF{
    void RadixSort(vector<int>& data) {
        if (data.empty()) return;

        size_t n = data.size();
        vector<int> buffer(n);

        uint32_t* arr = reinterpret_cast<uint32_t*>(data.data());
        uint32_t* temp = reinterpret_cast<uint32_t*>(buffer.data());

        for (size_t i = 0; i < n; i++) {
            arr[i] ^= 0x80000000;
        }

        const int bits = 16;
        const int mask = (1 << bits) - 1;
        const int num_buckets = 1 << bits;
    
        vector<int> count(num_buckets);
        for (size_t i = 0; i < n; i++) {
            count[arr[i] & mask]++;
        }
        for (int i = 1; i < num_buckets; i++) {
            count[i] += count[i - 1];
        }
        for (size_t i = n; i > 0; i--) {
            temp[--count[arr[i - 1] & mask]] = arr[i - 1];
        }

        count.clear();
        count.resize(num_buckets);
        for (size_t i = 0; i < n; i++){
            count[(temp[i] >> bits) & mask]++;
        }
        for (int i = 1; i < num_buckets; i++) {
            count[i] += count[i - 1];
        }
        for (size_t i = n; i > 0; i--) {
            arr[--count[(temp[i - 1] >> bits) & mask]] = temp[i - 1];
        }
        for (size_t i = 0; i < n; i++) {
            arr[i] ^= 0x80000000;
        }
    }
    
    long long calc(long long p, vector <int> x, vector <int> C){
        long long ret = 0;

        int ptr_l = lower_bound(x.begin(), x.end(), p) - x.begin() - 1, ptr_r;
        ptr_r = ptr_l + 1;
        int i = 0;
        while(!(ptr_l == -1 && ptr_r == (int)x.size())){
            if(ptr_l == -1){
                ret += (long long)(x[ptr_r] - p) * (long long)(C[i]);
                i++;
                ptr_r++;
            }
            else if(ptr_r == (int)x.size()){
                ret += (long  long)(p - x[ptr_l]) * (long long)(C[i]);
                i++;
                ptr_l--;
            }
            else{
                if((p - x[ptr_l]) < (x[ptr_r] - p)){
                    ret += (long long)(p - x[ptr_l]) * (long long)(C[i]);
                    i++;
                    ptr_l--;
                }
                else{
                    ret += (long long)(x[ptr_r] - p) * (long long)(C[i]);
                    i++;
                    ptr_r++;
                }
            }
        }
        return ret;
    }

    mt19937_64 gen;

    int num(long long L, long long R){
        return gen() % (R - L + 1) + L;
    }

    string input_secret_string = "NinOMaeInANIs";
};


int main(){
    string s;
    cin >> s;
    assert(s == gRadEr_STufF::input_secret_string);
    long long seed;
    cin >> seed;
    gRadEr_STufF::gen.seed(seed);
    int N, X, C, D, tp;
    cin >> N >> X >> C >> D >> tp;
    assert(0 <= tp && tp <= 2);
    if(tp == 2){
        cout << gRadEr_STufF::calc(car_gathering(3, {-1, 2, 3}, {1, 1, 2}), {-1, 2, 3}, {1, 1, 2}) << '\n';
        return 0;
    }
    assert(1 <= N && N <= 10'000'000);
    assert(0 <= X && X <= 1'000'000'000);
    assert(0 <= C && C <= 100);
    assert(1 <= D && D <= N);
    vector<int> final_x, final_c;
    for (int i = 0; i < D; i++){
        if(tp == 0){
            final_x.push_back(gRadEr_STufF::num(-X, X));
        }
        else{
            if(gRadEr_STufF::num(0, 1)){
                final_x.push_back(-gRadEr_STufF::num(X / 2, X));
            }
            else{
                final_x.push_back(gRadEr_STufF::num(X / 2, X));
            }
        }
    }
    for (int i = D; i < N; i++){
        final_x.push_back(final_x[gRadEr_STufF::num(0, D - 1)]);
    }

    gRadEr_STufF::RadixSort(final_x);
    vector<int> C_cnt;
    for (int i = 0; i < C; i++){
        C_cnt.push_back(gRadEr_STufF::num(0, N));
    }
    C_cnt.push_back(N);
    sort(C_cnt.begin(), C_cnt.end());
    for (int i = 0, pre = 0; i <= C;i++){
        for (int j = 0; j < C_cnt[i] - pre; j++){
            final_c.push_back(i);
        }
        pre = C_cnt[i];
    }

    assert((int)final_x.size() == N);
    assert((int)final_c.size() == N);
    for (int i = 0; i < N; i++){
        assert(-X <= final_x[i] && final_x[i] <= X);
        if (tp == 1){
            assert(abs(final_x[i]) >= X / 2);
        }
        if(i != 0){
            assert(final_x[i] >= final_x[i - 1]);
        }
    }
    for (int i = 0; i < N; i++){
        assert(0 <= final_c[i] && final_c[i] <= C);
        if(i != 0){
            assert(final_c[i] >= final_c[i - 1]);
        }
    }

    cout << gRadEr_STufF::calc(car_gathering(N, final_x, final_c), final_x, final_c) << '\n';

    return 0;
}
