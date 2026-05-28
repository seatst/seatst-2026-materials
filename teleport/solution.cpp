#include "teleport.h"

#include <bits/stdc++.h>
using namespace std;

const int kMaxN = 100005;
const int kLogN = 31 - __builtin_clz(kMaxN);
const int kMaxBit = 18;

struct TrieNode {
  int sz;
  int comp1, comp2;
  int comp2At;

  TrieNode *child[2];

  TrieNode() {
    sz = 0;
    comp1 = comp2 = -1;
    comp2At = -1;
    child[0] = child[1] = 0;
  }

  void push(int val, int comp, int bit=kMaxBit-1) {
    if (sz == 0) {
      comp1 = comp;
    } else if (comp != comp1 && comp2 == -1) {
      comp2 = comp;
      comp2At = sz;
    }
    ++sz;

    if (bit == -1) {
      return;
    }

    int b = (val >> bit & 1);
    if (!child[b]) {
      child[b] = new TrieNode();
    }
    child[b]->push(val, comp, bit - 1);
  }

  void pop(int val, int bit=kMaxBit-1) {
    --sz;
    if (sz == comp2At) {
      comp2At = -1;
      comp2 = -1;
    } else if (sz == 0) {
      comp1 = -1;
    }

    if (bit == -1) {
      return;
    }

    child[val >> bit & 1]->pop(val, bit - 1);
  }

  bool hasDiffComp(int comp) {
    if (comp1 != -1 && comp1 != comp) return true;
    if (comp2 != -1 && comp2 != comp) return true;
    return false;
  }
};

struct Trie {
  TrieNode *root;
  vector<int> vals;

  Trie() {
    root = new TrieNode();
  }

  void push(int val, int comp) {
    vals.push_back(val);
    root->push(val, comp);
  }

  void pop() {
    root->pop(vals.back());
    vals.pop_back();
  }

  pair<int, int> queryMinXORDiffComp(int x, int comp) {
    TrieNode *cur = root;
    int minxor = 0;
    for (int i = kMaxBit - 1; i >= 0; --i) {
      int b = x >> i & 1;
      if (cur->child[b] && cur->child[b]->hasDiffComp(comp)) {
        cur = cur->child[b];
      } else if (cur->child[b^1] && cur->child[b^1]->hasDiffComp(comp)) {
        cur = cur->child[b^1];
        minxor |= 1 << i;
      } else {
        return {-1, -1};
      }
    }
    return {minxor, cur->comp1 != comp ? cur->comp1 : cur->comp2};
  }
} trie1;

struct Trie2Node {
  int maxT1, maxT2;
  int maxT1Comp, maxT2Comp;
  Trie2Node *child[2];

  Trie2Node() {
    maxT1 = maxT2 = -1;
    maxT1Comp = maxT2Comp = -1;
    child[0] = child[1] = 0;
  }

  void push(int val, int comp, int bit, int t) {
    if (maxT1Comp != comp) {
      maxT2Comp = maxT1Comp;
      maxT2 = maxT1;
    }
    maxT1 = t;
    maxT1Comp = comp;

    if (bit == -1) return;

    int b = (val >> bit & 1);
    if (!child[b]) {
      child[b] = new Trie2Node();
    }
    child[b]->push(val, comp, bit - 1, t);
  }

  bool hasDiffComp(int comp, int tmin) {
    if (maxT1Comp != -1 && maxT1Comp != comp && maxT1 >= tmin) return true;
    if (maxT2Comp != -1 && maxT2Comp != comp && maxT2 >= tmin) return true;
    return false;
  }
};

struct Trie2 {
  Trie2Node *root;
  int t;

  Trie2() {
    reset();
  }

  void reset() {
    root = new Trie2Node();
    t = 0;
  }

  int add(int val, int comp) {
    root->push(val, comp, kMaxBit - 1, t);
    return t++;
  }

  pair<int, int> queryMinXorDiffComp(int x, int comp, int tmin) {
    Trie2Node *cur = root;
    int minxor = 0;
    for (int i = kMaxBit - 1; i >= 0; --i) {
      int b = (x >> i & 1);
      if (cur->child[b] && cur->child[b]->hasDiffComp(comp, tmin)) {
        cur = cur->child[b];
      } else if (cur->child[b^1] && cur->child[b^1]->hasDiffComp(comp, tmin)) {
        cur = cur->child[b^1];
        minxor |= 1 << i;
      } else {
        return {-1, -1};
      }
    }

    return {minxor, cur->maxT1Comp != comp ? cur->maxT1Comp : cur->maxT2Comp};
  }
} trie2;

int N;
int pref[kMaxN];
vector<int> adj[kMaxN];

int par[kMaxN];
int root(int u) {
  if (u == par[u]) return u;
  return par[u] = root(par[u]);
}

void merge(int u, int v) {
  par[root(u)] = root(v);
}

pair<int, int> cheapest[kMaxN];
void compareEdge(int u, pair<int, int> costComp) {
  if (costComp.first == -1) return;
  if (cheapest[u].first == -1 || cheapest[u] > costComp) {
    cheapest[u] = costComp;
  }
}

void dfs(int u) {
  int comp = root(u);
  trie1.push(pref[u], comp);
  int t = trie2.add(pref[u], comp);

  compareEdge(comp, trie1.queryMinXORDiffComp(pref[u], comp));


  for (int v : adj[u]) {
    dfs(v);
  }

  compareEdge(comp, trie2.queryMinXorDiffComp(pref[u], comp, t));

  trie1.pop();
}

namespace Tree {

vector<pair<int, int>> adj[kMaxN];

void addEdge(int u, int v, int w) {
  adj[u].push_back({v, w});
  adj[v].push_back({u, w});
}

int depth[kMaxN];
int par[kLogN][kMaxN];
int maxi[kLogN][kMaxN];
void dfs(int u, int p=-1) {
  for (auto [v, w] : adj[u]) {
    if (v == p) continue;
    par[0][v] = u;
    maxi[0][v] = w;
    depth[v] = depth[u] + 1;
    dfs(v, u);
  }
}

void build() {
  dfs(0);

  for (int k = 1; k < kLogN; ++k) {
    for (int i = 0; i < N; ++i) {
      par[k][i] = par[k - 1][par[k - 1][i]];
      maxi[k][i] = max(maxi[k - 1][i], maxi[k - 1][par[k - 1][i]]);
    }
  }
}

int maxPath(int u, int v) {
  if (depth[u] < depth[v]) swap(u, v);

  int ret = 0;
  while (depth[u] != depth[v]) {
    int k = __builtin_ctz(depth[u] - depth[v]);
    ret = max(ret, maxi[k][u]);
    u = par[k][u];
  }

  if (u == v) return ret;

  for (int k = kLogN - 1; k >= 0; --k) {
    if (par[k][u] == par[k][v]) continue;
    ret = max(ret, maxi[k][u]);
    ret = max(ret, maxi[k][v]);
    u = par[k][u];
    v = par[k][v];
  }

  ret = max(ret, maxi[0][u]);
  ret = max(ret, maxi[0][v]);
  return ret;
}

} // namespace Tree

void init(int N, std::vector<int> P, std::vector<int> W) {
  ::N = N;
  for (int i = 0; i < N; ++i) par[i] = i;
  for (int i = 1; i < N; ++i) {
    adj[P[i]].push_back(i);
    pref[i] = pref[P[i]] ^ W[i];
  }

  for (int mergeCnt = 0; mergeCnt < N - 1; ) {
    for (int i = 0; i < N; ++i) cheapest[i] = {-1, -1};
    trie2.reset();

    dfs(0);

    for (int i = 0; i < N; ++i) {
      if (cheapest[i].first == -1) continue;
      if (root(i) == root(cheapest[i].second)) continue;
      merge(i, cheapest[i].second);
      ++mergeCnt;
      Tree::addEdge(i, cheapest[i].second, cheapest[i].first);
    }
  }

  Tree::build();
}

int minimum_energy(int U, int V) {
  return Tree::maxPath(U, V);
}