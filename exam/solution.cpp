#include <bits/stdc++.h>
using namespace std;

int minimum_dissatisfaction(int N, vector<int> A, vector<int> B) {
    // mathAtEnglish[e] = Mathematics rank of the student whose English rank is e.
    vector<int> mathAtEnglish(N);
    for (int i = 0; i < N; ++i) {
        mathAtEnglish[A[i]] = B[i];
    }

    // active[b] is true iff the point with Mathematics rank b already has
    // English rank <= current x, but b is still above the current y boundary.
    vector<unsigned char> active(N, 0);

    int x = -1, y = -1;       // current rectangle: A <= x and B <= y
    int inside = 0;           // number of students inside the current rectangle
    int answer = 0;

    for (int k = 1; k <= N; ++k) {
        int nx = max(x, A[k - 1]);
        int ny = max(y, B[k - 1]);

        // Add newly covered English ranks. Points whose Mathematics rank is
        // already covered enter immediately; the rest are remembered for when
        // the Mathematics boundary reaches them.
        for (int e = x + 1; e <= nx; ++e) {
            int b = mathAtEnglish[e];
            if (b <= y) {
                ++inside;
            } else {
                active[b] = 1;
            }
        }

        // Add newly covered Mathematics ranks. A point is counted here exactly
        // when its English rank was already covered earlier or in the loop above.
        for (int b = y + 1; b <= ny; ++b) {
            inside += active[b];
        }

        x = nx;
        y = ny;

        answer = max(answer, inside - k);
    }

    return answer;
}
