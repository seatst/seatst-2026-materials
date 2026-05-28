#include "teleport.h"

#include <cassert>
#include <cstdio>
#include <vector>

int main() {
  int N;
  assert(1 == scanf("%d", &N));

  std::vector<int> P(N, -1), W(N, -1);
  for (int i = 1; i < N; ++i) {
    assert(1 == scanf("%d", &P[i]));
  }
  for (int i = 1; i < N; ++i) {
    assert(1 == scanf("%d", &W[i]));
  }

  int Q;
  assert(1 == scanf("%d", &Q));

  std::vector<int> U(Q), V(Q);
  for (int j = 0; j < Q; ++j) {
    assert(2 == scanf("%d %d", &U[j], &V[j]));
  }

  init(N, P, W);

  std::vector<int> answer(Q);
  for (int j = 0; j < Q; ++j) {
    answer[j] = minimum_energy(U[j], V[j]);
  }

  for (int j = 0; j < Q; ++j) {
    printf("%d\n", answer[j]);
  }
  return 0;
}
