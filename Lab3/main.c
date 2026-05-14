#include <stdio.h>
#include <stdlib.h>

#define INF 1000000000

typedef struct EdgeNode {
    int to, weight;
    struct EdgeNode* next;
} EdgeNode;

void readGraph(int* n, EdgeNode*** adj) {
    printf("Numar de varfuri: ");
    scanf("%d", n);
    if (*n <= 0) { printf("Valoare invalida.\n"); exit(1); }
    *adj = (EdgeNode**)malloc(*n * sizeof(EdgeNode*));
    for (int i = 0; i < *n; i++) (*adj)[i] = NULL;
    int m;
    printf("Numar de arce: ");
    scanf("%d", &m);
    if (m < 0) { printf("Valoare invalida.\n"); exit(1); }
    printf("Introduceti arcele (u v pondere) cu varfuri 1..%d:\n", *n);
    for (int i = 0; i < m; i++) {
        int u, v, w;
        scanf("%d %d %d", &u, &v, &w);
        if (u < 1 || u > *n || v < 1 || v > *n) {
            printf("Arc invalid, reintroduceti.\n");
            i--;
            continue;
        }
        EdgeNode* e = (EdgeNode*)malloc(sizeof(EdgeNode));
        e->to = v-1; e->weight = w; e->next = (*adj)[u-1];
        (*adj)[u-1] = e;
    }
}

void freeGraph(int n, EdgeNode** adj) {
    for (int i = 0; i < n; i++) {
        EdgeNode* p = adj[i];
        while (p) {
            EdgeNode* t = p->next;
            free(p);
            p = t;
        }
    }
    free(adj);
}

void ford(int n, EdgeNode** adj, int start, int* dist, int* pred) {
    for (int i = 0; i < n; i++) { dist[i] = INF; pred[i] = -1; }
    dist[start] = 0;
    for (int k = 0; k < n-1; k++) {
        int changed = 0;
        for (int u = 0; u < n; u++) {
            if (dist[u] == INF) continue;
            EdgeNode* p = adj[u];
            while (p) {
                int v = p->to, w = p->weight;
                if (dist[u] + w < dist[v]) {
                    dist[v] = dist[u] + w;
                    pred[v] = u;
                    changed = 1;
                }
                p = p->next;
            }
        }
        if (!changed) break;
    }
}

void bellmanKalaba(int n, EdgeNode** adj, int dest, int* dist, int* succ) {
    int *V_old = malloc(n * sizeof(int));
    int *V_new = malloc(n * sizeof(int));
    for (int i = 0; i < n; i++) { V_old[i] = INF; succ[i] = -1; }
    V_old[dest] = 0;
    for (int u = 0; u < n; u++) {
        EdgeNode* p = adj[u];
        while (p) {
            if (p->to == dest && p->weight < V_old[u]) {
                V_old[u] = p->weight;
                succ[u] = dest;
            }
            p = p->next;
        }
    }
    int converged = 0;
    for (int iter = 1; iter <= n-1 && !converged; iter++) {
        for (int i = 0; i < n; i++) V_new[i] = V_old[i];
        for (int i = 0; i < n; i++) {
            if (i == dest) continue;
            EdgeNode* p = adj[i];
            while (p) {
                int j = p->to, w = p->weight;
                if (V_old[j] != INF && w + V_old[j] < V_new[i]) {
                    V_new[i] = w + V_old[j];
                    succ[i] = j;
                }
                p = p->next;
            }
        }
        V_new[dest] = 0;
        converged = 1;
        for (int i = 0; i < n; i++)
            if (V_new[i] != V_old[i]) { converged = 0; break; }
        int *tmp = V_old; V_old = V_new; V_new = tmp;
    }
    for (int i = 0; i < n; i++) {
        dist[i] = V_old[i];
        if (i == dest) succ[i] = -1;
        else if (dist[i] != INF) {
            EdgeNode* p = adj[i];
            while (p) {
                int j = p->to, w = p->weight;
                if (dist[j] != INF && w + dist[j] == dist[i]) {
                    succ[i] = j;
                    break;
                }
                p = p->next;
            }
        }
    }
    free(V_old); free(V_new);
}

void print_path_rev(int* pred, int start, int end) {
    if (start == end) { printf("%d", start+1); return; }
    if (pred[end] == -1) { printf("nu exista"); return; }
    print_path_rev(pred, start, pred[end]);
    printf(" -> %d", end+1);
}

void print_path_fwd(int* succ, int start, int dest) {
    if (start == dest) { printf("%d", start+1); return; }
    if (succ[start] == -1) { printf("nu exista"); return; }
    printf("%d -> ", start+1);
    print_path_fwd(succ, succ[start], dest);
}

int main() {
    printf("Algoritmi: Ford (Bellman-Ford) si Bellman-Calaba (drum minim)\n\n");
    int n;
    EdgeNode** adj;
    readGraph(&n, &adj);

    printf("\n--- Algoritmul Ford (drumuri minime de la o sursa) ---\n");
    int start;
    printf("Varf initial (sursa) [1..%d]: ", n);
    scanf("%d", &start);
    if (start < 1 || start > n) { printf("Sursa invalida.\n"); freeGraph(n, adj); return 1; }
    start--;
    int* distFord = malloc(n * sizeof(int));
    int* predFord = malloc(n * sizeof(int));
    ford(n, adj, start, distFord, predFord);
    printf("\nDistante minime de la %d:\n", start+1);
    for (int i = 0; i < n; i++) {
        printf("   la %d: ", i+1);
        if (distFord[i] == INF) printf("∞");
        else printf("%d", distFord[i]);
        printf("\tDrum: ");
        if (distFord[i] == INF) printf("inexistent");
        else print_path_rev(predFord, start, i);
        printf("\n");
    }

    printf("\n--- Algoritmul Bellman-Calaba (drumuri minime la o destinatie) ---\n");
    int dest;
    printf("Varf final (destinatie) [1..%d]: ", n);
    scanf("%d", &dest);
    if (dest < 1 || dest > n) {
        printf("Destinatie invalida.\n");
        free(distFord); free(predFord); freeGraph(n, adj);
        return 1;
    }
    dest--;
    int* distBK = malloc(n * sizeof(int));
    int* succBK = malloc(n * sizeof(int));
    bellmanKalaba(n, adj, dest, distBK, succBK);
    printf("\nDistante minime catre %d:\n", dest+1);
    for (int i = 0; i < n; i++) {
        printf("   de la %d: ", i+1);
        if (distBK[i] == INF) printf("∞");
        else printf("%d", distBK[i]);
        printf("\tDrum: ");
        if (distBK[i] == INF) printf("inexistent");
        else print_path_fwd(succBK, i, dest);
        printf("\n");
    }

    free(distFord); free(predFord); free(distBK); free(succBK);
    freeGraph(n, adj);
    return 0;
}
