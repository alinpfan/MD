#include <iostream>
using namespace std;

// Nod al listei de adiacență
struct AdjNode {
    int vertex;
    AdjNode* next;
};

// Structura grafului
struct Graph {
    int n;          // număr de vârfuri
    AdjNode** adj;  // tablou de liste (pointeri la noduri)
};

// Creează un graf cu n vârfuri, fără muchii
Graph* createGraph(int n) {
    Graph* g = new Graph;
    g->n = n;
    g->adj = new AdjNode*[n];
    for (int i = 0; i < n; ++i)
        g->adj[i] = nullptr;
    return g;
}

// Adaugă muchie neorientată între u și v (0-based)
void addEdge(Graph* g, int u, int v) {
    // Adaugă v la lista lui u
    AdjNode* node = new AdjNode{v, g->adj[u]};
    g->adj[u] = node;
    // Adaugă u la lista lui v (graf neorientat)
    node = new AdjNode{u, g->adj[v]};
    g->adj[v] = node;
}

// Parcurgerea în adâncime (DFS) – iterativă, folosind o stivă
void DFS(Graph* g, int start) {
    bool* visited = new bool[g->n]();  // inițializare cu false
    int* stack = new int[g->n];
    int top = -1;

    stack[++top] = start;
    visited[start] = true;

    cout << "Parcurgere DFS: ";
    while (top >= 0) {
        int v = stack[top--];
        cout << (v + 1) << " ";            // afișare 1-based

        // Parcurge vecinii și introduce-i pe cei nevizitați în stivă
        AdjNode* p = g->adj[v];
        while (p) {
            if (!visited[p->vertex]) {
                visited[p->vertex] = true;
                stack[++top] = p->vertex;
            }
            p = p->next;
        }
    }
    cout << endl;
    delete[] visited;
    delete[] stack;
}

// Parcurgerea în lărgime (BFS) – iterativă, folosind o coadă
void BFS(Graph* g, int start) {
    bool* visited = new bool[g->n]();
    int* queue = new int[g->n];
    int front = 0, rear = 0;

    queue[rear++] = start;
    visited[start] = true;

    cout << "Parcurgere BFS: ";
    while (front < rear) {
        int v = queue[front++];
        cout << (v + 1) << " ";

        AdjNode* p = g->adj[v];
        while (p) {
            if (!visited[p->vertex]) {
                visited[p->vertex] = true;
                queue[rear++] = p->vertex;
            }
            p = p->next;
        }
    }
    cout << endl;
    delete[] visited;
    delete[] queue;
}

// Eliberează memoria alocată
void freeGraph(Graph* g) {
    for (int i = 0; i < g->n; ++i) {
        AdjNode* p = g->adj[i];
        while (p) {
            AdjNode* temp = p;
            p = p->next;
            delete temp;
        }
    }
    delete[] g->adj;
    delete g;
}

// Funcția principală – interfață pentru utilizator
int main() {
    cout << "Parcurgere grafuri – DFS și BFS (liste de adiacență)\n\n";

    int n, m;
    cout << "Număr de vârfuri: ";
    cin >> n;
    if (n <= 0) {
        cout << "Număr invalid.\n";
        return 1;
    }

    Graph* g = createGraph(n);

    cout << "Număr de muchii: ";
    cin >> m;
    if (m < 0) {
        cout << "Număr invalid.\n";
        freeGraph(g);
        return 1;
    }

    cout << "Introduceți muchiile (u v), noduri între 1 și " << n << ":\n";
    for (int i = 0; i < m; ++i) {
        int u, v;
        cin >> u >> v;
        if (u < 1 || u > n || v < 1 || v > n) {
            cout << "Muchie invalidă, reintroduceți.\n";
            --i;
            continue;
        }
        addEdge(g, u - 1, v - 1);
    }

    int start;
    cout << "Vârful de plecare (1 - " << n << "): ";
    cin >> start;
    if (start < 1 || start > n) {
        cout << "Vârf de start invalid.\n";
        freeGraph(g);
        return 1;
    }

    DFS(g, start - 1);
    BFS(g, start - 1);

    freeGraph(g);
    return 0;
}