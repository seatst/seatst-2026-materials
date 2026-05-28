#include "exam.h"

#include <cassert>
#include <cstdio>
#include <vector>
#include <algorithm>
#include <numeric>
#include <ctime>
#include <random> // Still needed for std::uniform_int_distribution

// Custom, blazing fast 32-bit Xorshift PRNG
struct XorShift32 {
    using result_type = uint32_t;
    uint32_t state;
    
    XorShift32(uint32_t seed = 1) {
        // Xorshift cannot be seeded with 0
        state = (seed == 0) ? 1 : seed;
    }
    
    void seed(uint32_t s) {
        state = (s == 0) ? 1 : s;
    }
    
    static constexpr result_type min() { return 0; }
    static constexpr result_type max() { return 0xFFFFFFFF; }
    
    result_type operator()() {
        uint32_t x = state;
        x ^= x << 13;
        x ^= x >> 17;
        x ^= x << 5;
        return state = x;
    }
};

XorShift32 rng; // Replacing std::mt19937
clock_t program_start_time; // Global CPU start time

// Helper function to enforce the wait condition
void wait_if_needed(int N) {
    if (N > 4000000) {
        // Burn CPU cycles until exactly 2.0 seconds of CPU time have passed
        // since the program started.
        while (double(clock() - program_start_time) / CLOCKS_PER_SEC < 1.5) {
            // Busy wait - forces the OJ to register CPU usage
        }
    }
}

// Permute a single vector: chooses k distinct indices and randomly shuffles their values
void permute(std::vector<int>& v, int k) {
    if (k <= 1) return;
    int n = v.size();
    
    // Create an array of indices [0, 1, ..., n-1]
    std::vector<int> indices(n);
    std::iota(indices.begin(), indices.end(), 0);
    
    // Partially shuffle the first k elements to pick k distinct indices randomly
    for (int i = 0; i < k; ++i) {
        std::uniform_int_distribution<int> dist(i, n - 1);
        std::swap(indices[i], indices[dist(rng)]);
    }
    
    // Extract the values at those chosen k indices
    std::vector<int> chosen_values(k);
    for (int i = 0; i < k; ++i) {
        chosen_values[i] = v[indices[i]];
    }
    
    // Shuffle the extracted values
    std::shuffle(chosen_values.begin(), chosen_values.end(), rng);
    
    // Put the shuffled values back into their original chosen positions
    for (int i = 0; i < k; ++i) {
        v[indices[i]] = chosen_values[i];
    }
}

// Permute tied vectors: chooses k distinct indices and applies the SAME shuffle to both A and B
void permute(std::vector<int>& A, std::vector<int>& B, int k) {
    if (k <= 1) return;
    int n = A.size();
    
    std::vector<int> indices(n);
    std::iota(indices.begin(), indices.end(), 0);
    
    for (int i = 0; i < k; ++i) {
        std::uniform_int_distribution<int> dist(i, n - 1);
        std::swap(indices[i], indices[dist(rng)]);
    }
    
    // Create a generic random permutation pattern of size k
    std::vector<int> p(k);
    std::iota(p.begin(), p.end(), 0);
    std::shuffle(p.begin(), p.end(), rng);
    
    // Store the old values so they aren't overwritten during the swap
    std::vector<int> oldA(k), oldB(k);
    for (int i = 0; i < k; ++i) {
        oldA[i] = A[indices[i]];
        oldB[i] = B[indices[i]];
    }
    
    // Apply the exact same permutation 'p' to the chosen indices of both arrays
    for (int i = 0; i < k; ++i) {
        A[indices[i]] = oldA[p[i]];
        B[indices[i]] = oldB[p[i]];
    }
}

int gen0(int N){
    int incA, incB;
    assert(2 == scanf("%d%d", &incA, &incB));
    assert(0<=incA && incA<=1);
    assert(0<=incB && incB<=1);

    int K, tied;
    assert(2 == scanf("%d%d", &K, &tied));
    assert(0<=K && K<=N);
    assert(0<=tied && tied<=1);
    
    std::vector<int> A(N), B(N);
    // Fill both with 0, 1, 2...
    std::iota(A.begin(), A.end(), 0);
    std::iota(B.begin(), B.end(), 0);

    // Reverse if descending is requested
    if (!incA) std::reverse(A.begin(), A.end());
    if (!incB) std::reverse(B.begin(), B.end());
    
    if (tied){
        permute(A, B, K);
    }
    else{
        permute(A, K);
        permute(B, K);
    }
    
    wait_if_needed(N);
    return minimum_dissatisfaction(N, A, B);
}

void shuffle_k(std::vector<int>& v, int n, int k) {
    for (int i = 0; i < n; ++i){
        std::uniform_int_distribution<int> dist(i, std::min(i + k, n - 1));
        std::swap(v[i], v[dist(rng)]);
    }
}

int gen1(int N){
    int K;
    assert(1 == scanf("%d", &K));
    assert(0<=K && K<=N);
    
    std::vector<int> A(N), B(N);
    // Fill both with 0, 1, 2...
    std::iota(A.begin(), A.end(), 0);
    std::iota(B.begin(), B.end(), 0);
    
    shuffle_k(A, N, K);
    shuffle_k(B, N, K);
    
    wait_if_needed(N);
    return minimum_dissatisfaction(N, A, B); 
}

int gen2(int N){
    std::vector<int> A(N), B(N);
    for (int i = 0; i < N; ++i) scanf("%d", &A[i]);
    for (int i = 0; i < N; ++i) scanf("%d", &B[i]);
    
    wait_if_needed(N);
    return minimum_dissatisfaction(N, A, B);
}

int main() {
    // Record CPU clock ticks right at program start
    program_start_time = clock();

    int N,X,seed;
    assert(3 == scanf("%d%d%d", &N, &X, &seed));
    assert(1<=N && N<=10000000);
    assert(0<=X && X<=2);
    rng.seed(seed); // Seeds our custom Xorshift
    
    int res;
    if (X==0) res=gen0(N);
    if (X==1) res=gen1(N);
    if (X==2) res=gen2(N);
    
    printf("%d\n", res);
    return 0;
}
