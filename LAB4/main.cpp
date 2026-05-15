#include <iostream>
#include <vector>
#include <queue>
using namespace std;

int main() {
    int n;
    cout << "Numar de varfuri: ";
    while (!(cin >> n) || n <= 0) { cin.clear(); cin.ignore(1000, '\n'); cout << "Reintroduceti: "; }

    vector<vector<int>> adj(n);
    int m;
    cout << "Numar de muchii: ";
    while (!(cin >> m) || m < 0) { cin.clear(); cin.ignore(1000, '\n'); cout << "Reintroduceti: "; }

    cout << "Introduceti muchiile (u v) 1.." << n << ":\n";
    for (int i = 0; i < m; ++i) {
        int u, v;
        cout << "Muchia " << i+1 << ": ";
        while (true) {
            if (!(cin >> u >> v)) { cin.clear(); cin.ignore(1000, '\n'); cout << "Format invalid. Reintroduceti: "; continue; }
            if (u < 1 || u > n || v < 1 || v > n) { cout << "Varfuri inexistente. Reintroduceti: "; continue; }
            break;
        }
        --u; --v;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    int start;
    cout << "\nVarf initial [1.." << n << "] (0=automat): ";
    while (!(cin >> start) || start < 0 || start > n) { cin.clear(); cin.ignore(1000, '\n'); cout << "Reintroduceti: "; }
    --start; // -1 if 0

    vector<int> parent(n, -1);
    vector<bool> vis(n, false);
    queue<int> q1, q2;

    if (start >= 0 && !vis[start]) { vis[start] = true; q1.push(start); }

    while (true) {
        if (q1.empty()) {
            int i = 0;
            while (i < n && vis[i]) ++i;
            if (i == n) break;
            vis[i] = true;
            q1.push(i);
        }
        while (!q1.empty()) {
            int u = q1.front(); q1.pop();
            for (int v : adj[u]) {
                if (!vis[v]) {
                    vis[v] = true;
                    q2.push(v);
                    parent[v] = u;
                }
            }
        }
        swap(q1, q2);
    }

    cout << "\nGraful de acoperire:\n";
    int edges = 0;
    for (int i = 0; i < n; ++i) {
        if (parent[i] != -1) {
            cout << "(" << parent[i]+1 << ", " << i+1 << ")\n";
            ++edges;
        }
    }
    if (!edges) cout << "(graful nu contine muchii)\n";
    else cout << "Numar de muchii pastrate: " << edges << '\n';
}
