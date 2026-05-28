#include <vector>
#include <cstring>
#include <algorithm>

using namespace std;

const int maxn = 5000005;

int n, m, a, b;
vector<int> adj[maxn];
int lay[maxn], v[maxn];
int par[maxn], sz[maxn];
vector<int> para, parb;

// Flattened Layer / BFS Queue Structures
int bfs_q[maxn];
int layer_st[maxn], layer_en[maxn];
int kp[maxn]; // parent in krt

int find(int x) {
    int root = x;
    while (root != par[root]) root = par[root];
    
    int curr = x;
    while (curr != root) {
        int nxt = par[curr];
        par[curr] = root;
        curr = nxt;
    }
    return root;
}

void merge(int x, int y) {
    x = find(x);
    y = find(y);
    if (x == y) return;
    if (sz[x] > sz[y]) swap(x, y);
    par[x] = y;
    sz[y] += sz[x];
}

int min_distinct(int N, int M, int A, int B, std::vector<int> U, std::vector<int> V) {
    n = N; m = M; a = A + 1; b = B + 1;
    if (m > maxn || n > maxn) return 0;

    // Fast Cleanup for multiple executions
    para.clear();
    parb.clear();
    for (int i = 0; i <= n; i++) {
        adj[i].clear();
        par[i] = i;
        sz[i] = 1;
        layer_st[i] = -1;
        layer_en[i] = -1;
    }

    memset(v, 0, (n + 1) * sizeof(int));
    memset(kp, -1, (n + 1) * sizeof(int));

    for (int i = 0; i < m; i++) {
        int x = U[i] + 1, y = V[i] + 1;
        adj[x].push_back(y);
        adj[y].push_back(x);
    }

    // Array-based Queue
    int head = 0, tail = 0;
    bfs_q[tail++] = 1;
    v[1] = 1;
    lay[1] = 1;

    // BFS creates a naturally sorted layer array
    while (head < tail) {
        int x = bfs_q[head++];
        int y = lay[x];

        // Track offsets for the flattened layer slice
        if (layer_st[y] == -1) layer_st[y] = head - 1;
        layer_en[y] = head;

        for (int i : adj[x]) {
            if (!v[i]) {
                v[i] = 1;
                lay[i] = y + 1;
                bfs_q[tail++] = i;
            }
        }
    }

    memset(v, 0, (n + 1) * sizeof(int));

    if (lay[a] < lay[b]) swap(a, b);

    // build krt of layers [1..lay[a]]
    for (int i = lay[a]; i > 0; i--) {
        if (layer_st[i] == -1) continue;

        for (int idx = layer_st[i]; idx < layer_en[i]; idx++) {
            int j = bfs_q[idx];
            for (int k : adj[j]) {
                if (lay[k] == lay[j]) merge(k, j);
            }
        }

        for (int idx = layer_st[i]; idx < layer_en[i]; idx++) {
            int j = bfs_q[idx];
            for (int k : adj[j]) {
                if (lay[k] < lay[j]) {
                    int root_j = find(j); // Cache inner lookup
                    if (kp[root_j] == -1) {
                        kp[root_j] = k;
                    } else {
                        merge(kp[root_j], k);
                    }
                }
            }
        }
    }

    int cur = a;
    while (cur != -1) {
        para.push_back(cur);
        cur = kp[find(cur)];
    }

    cur = b;
    while (cur != -1) {
        parb.push_back(cur);
        cur = kp[find(cur)];
    }

    int lca = 0;
    while (!para.empty() && !parb.empty() && find(para.back()) == find(parb.back())) {
        lca++;
        para.pop_back();
        parb.pop_back();
    }

    int ans = lay[a] - lca + 1;

    for (int i = lay[a] + 1; i <= n; i++) {
        if (layer_st[i] == -1) continue;

        for (int idx = layer_st[i]; idx < layer_en[i]; idx++) {
            int j = bfs_q[idx];
            for (int k : adj[j]) {
                if (lay[k] == lay[j]) merge(j, k);
            }
        }

        for (int idx = layer_st[i]; idx < layer_en[i]; idx++) {
            int j = bfs_q[idx];
            for (int k : adj[j]) {
                if (lay[k] < lay[j]) {
                    int root_j = find(j); // Cache inner lookup
                    if (kp[root_j] == -1) {
                        kp[root_j] = k;
                    } else {
                        int x = kp[root_j], y = k;
                        int root_x = find(x), root_y = find(y); // Cache initial lookup
                        
                        while (root_x != root_y) {
                            int nx = kp[root_x];
                            int ny = kp[root_y];
                            merge(root_x, root_y);
                            x = nx;
                            y = ny;
                            if (x == -1 || y == -1) break; // Defensive guard
                            root_x = find(x);
                            root_y = find(y);
                        }
                    }
                }
            }
        }

        while (!para.empty() && !parb.empty() && find(para.back()) == find(parb.back())) {
            lca++;
            para.pop_back();
            parb.pop_back();
        }

        ans = min(ans, i - lca + 1);
    }

    return ans;
}