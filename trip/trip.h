#pragma once
#include <vector>
#include <queue>
#include <cmath>
#include <algorithm>
#include <random>
#include <cassert>
// 1. Declare the struct
struct GraphTestCase {
    int N;
    int M;
    int A;
    int B;
    std::vector<int> U; 
    std::vector<int> V; 
};

// 2. The Namespace
namespace AdversarialGenerator {

    // Helper for consistent edge normalization (optional, but good practice)
    inline void add_edge(GraphTestCase& tc, int u, int v) {
        tc.U.push_back(std::min(u, v));
        tc.V.push_back(std::max(u, v));
    }

    // ========================================================================
    // GENERATOR 1: The Serialized Double Pyramid (Queue-Undo Killer)
    // ========================================================================
    inline GraphTestCase generate_pyramid(int target_N) {
        GraphTestCase tc;
        tc.N = target_N;
        int W = std::sqrt(tc.N); 
        
        tc.U.reserve(tc.N + 5);
        tc.V.reserve(tc.N + 5);
        
        int current_id = 1;

        std::vector<std::pair<int, int>> splits;
        std::queue<std::pair<int, int>> q;
        q.push({0, W - 1});
        while (!q.empty()) {
            auto [start, end] = q.front(); 
            q.pop();
            if (start == end) continue;
            splits.push_back({start, end});
            int mid = start + (end - start) / 2;
            q.push({start, mid});
            q.push({mid + 1, end});
        }

        std::vector<int> interval_node(W, -1);
        interval_node[0] = current_id++; 
        
        for (int k = 1; k < W; k++) {
            auto [start, end] = splits[k - 1];
            int mid = start + (end - start) / 2;
            
            int parent = interval_node[start];
            int left_child = current_id++;
            int right_child = current_id++;
            
            interval_node[start] = left_child;
            interval_node[mid + 1] = right_child;
            
            add_edge(tc, parent, left_child);
            add_edge(tc, parent, right_child);
            
            for (int i = 0; i < W; i++) {
                if (interval_node[i] != -1 && i != start && i != mid + 1) {
                    int new_node = current_id++;
                    add_edge(tc, interval_node[i], new_node);
                    interval_node[i] = new_node;
                }
            }
        }

        tc.A = interval_node[0];
        tc.B = interval_node[W - 1];

        std::vector<int> active_bottom = interval_node; 
        std::vector<int> node_map(tc.N + 5, 0); 
        
        for (int k = 1; k < W; k++) {
            auto [start, end] = splits[k - 1]; 
            int mid = start + (end - start) / 2;
            
            int left_rep = mid;
            int right_rep = mid + 1;
            
            int u = active_bottom[left_rep];
            int v = active_bottom[right_rep];
            
            int merged_node = current_id++;
            add_edge(tc, u, merged_node);
            add_edge(tc, v, merged_node);
            
            for (int i = 0; i < W; i++) {
                if (active_bottom[i] == u || active_bottom[i] == v) {
                    active_bottom[i] = merged_node;
                }
            }
            
            std::vector<int> unique_active;
            for (int i = 0; i < W; i++) {
                if (active_bottom[i] != merged_node) {
                    unique_active.push_back(active_bottom[i]);
                }
            }
            std::sort(unique_active.begin(), unique_active.end());
            unique_active.erase(std::unique(unique_active.begin(), unique_active.end()), unique_active.end());
            
            for (int old_node : unique_active) {
                int new_node = current_id++;
                if(new_node > tc.N) break;
                add_edge(tc, old_node, new_node);
                node_map[old_node] = new_node; 
            }
            
            for (int i = 0; i < W; i++) {
                int mapped_val = node_map[active_bottom[i]];
                if (mapped_val != 0) {
                    active_bottom[i] = mapped_val; 
                }
            }

            for (int old_node : unique_active) {
                node_map[old_node] = 0; 
            }
            
            if (tc.U.size() >= tc.N || current_id > tc.N) break;
        }

        int last_node = active_bottom[0];
        while (current_id <= tc.N) {
            add_edge(tc, last_node, current_id);
            last_node = current_id++;
        }

        tc.M = tc.U.size(); 
        return tc;
    }

    // ========================================================================
    // GENERATOR 2: The Stalactite Maze (Greedy/Heuristic Killer)
    // ========================================================================
    inline GraphTestCase generate_stalactite(int n, int m, int w) {
        GraphTestCase tc;
        tc.N = n;
        tc.U.reserve(m + 5);
        tc.V.reserve(m + 5);
        
        std::mt19937 rng(0);
        int numm = m;
        
        std::vector<std::vector<int>> lay(n + 5);
        
        lay[0].push_back(1);
        lay[1].push_back(2);
        add_edge(tc, 1, 2);
        numm--;
        
        int lft = w - 1;
        int num = 3, curl = 1;
        
        // 1. Build the hanging chains
        while(num <= n){
            if(lft == 0){
                curl++;
                lft = w;
            }
            lft--;
            add_edge(tc, num, std::max(1, num - w));
            numm--;
            
            lay[curl].push_back(num);
            num++;
        }
        
        // Ensure w is safe to prevent out-of-bounds
        int target_chain = std::min(w - 1, (int)lay[curl/2].size() - 1);
        if(target_chain < 1) target_chain = 1;
        
        tc.A = lay[curl/2][0];
        tc.B = lay[curl/2][target_chain];
        
        // 2. Build the horizontal cross-edges (The Maze)
        for(int i = 0; i < target_chain; i++){
            if(numm <= 0) break;
            int x = 0;
            
            // Top half cross-edge
            std::uniform_int_distribution<int> top_dist_norm(1, curl/2 - 1);
            std::uniform_int_distribution<int> top_dist_odd(3 * curl / 8, curl/2 - 1);
            
            x = top_dist_norm(rng);
            if(i & 1) x = top_dist_odd(rng);
            
            // Safety check in case graph is too shallow
            if(!lay[x].empty() && i + 1 < lay[x].size()) {
                add_edge(tc, lay[x][i], lay[x][i+1]);
                numm--;
            }
            
            if(numm <= 0) break;
            
            // Bottom half cross-edge
            std::uniform_int_distribution<int> bot_dist_norm(curl/2 + 1, curl - 1);
            std::uniform_int_distribution<int> bot_dist_even(curl/2 + 1, 5 * curl / 8);
            
            x = bot_dist_norm(rng);
            if(!(i & 1)) x = bot_dist_even(rng);
            
            if(!lay[x].empty() && i + 1 < lay[x].size()) {
                add_edge(tc, lay[x][i], lay[x][i+1]);
                numm--;
            }
        }
        
        std::uniform_int_distribution<int> b_dist(1, n);
        while(tc.B == tc.A) {
            tc.B = b_dist(rng);
        }
        
        tc.M = tc.U.size();
        return tc;
    }
}

int min_distinct(int N, int M, int A, int B, std::vector<int> U, std::vector<int> V);
