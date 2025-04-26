#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <limits.h>

typedef struct AdjList {
    int nextV;
    int edgeCap; 
    struct AdjList *Next;
} AdjList;
typedef AdjList *AdjPtr;

int getIndex(char name[4], char (*refList)[4]);
void createGraph(AdjPtr *graph, int r_num, char (*refList)[4]);
AdjPtr *createResidualGraph(AdjPtr *graph, int nodeCount);
bool dfs(AdjPtr *residualGraph, int *visited, int src, int dest, int *parent);
void updateResidualGraph(AdjPtr *residualGraph, int *parent, int src, int dest, int pathFlow);
int fordFulkerson(AdjPtr *graph, int nodeCount, int src, int dest);

int listTop = 0;

int main() {
    char src[4], dest[4];
    int routeCnt;
    scanf("%3s %3s", src, dest);
    scanf("%d", &routeCnt);

    char (*refList)[4] = calloc(2 * routeCnt, sizeof(char[4]));
    AdjPtr *graph = calloc(2 * routeCnt, sizeof(AdjPtr));

    createGraph(graph, routeCnt, refList);

    int srcIndex = getIndex(src, refList);
    int destIndex = getIndex(dest, refList);
    int nodeCount = listTop;

    int maxFlow = fordFulkerson(graph, nodeCount, srcIndex, destIndex);
    printf("%d", maxFlow);

    free(refList);
    for (int i = 0; i < nodeCount; i++) {
        AdjPtr temp = graph[i];
        while (temp) {
            AdjPtr toFree = temp;
            temp = temp->Next;
            free(toFree);
        }
    }
    free(graph);
    return 0;
}

int getIndex(char name[4], char (*refList)[4]) {
    for (int i = 0; i < listTop; i++) {
        if (strcmp(name, refList[i]) == 0) {
            return i;
        }
    }
    strcpy(refList[listTop], name);
    return listTop++;
}

void createGraph(AdjPtr *graph, int r_num, char (*refList)[4]) {
    int src, dest, cap;
    char srcStr[4], destStr[4];
    for (int i = 0; i < r_num; i++) {
        scanf("%3s %3s %d", srcStr, destStr, &cap);
        src = getIndex(srcStr, refList);
        dest = getIndex(destStr, refList);
        AdjPtr edge = calloc(1, sizeof(AdjList));
        edge->nextV = dest;
        edge->edgeCap = cap;
        edge->Next = graph[src];
        graph[src] = edge;
    }
}

AdjPtr* createResidualGraph(AdjPtr *graph, int nodeCount) {
    AdjPtr *residual = calloc(nodeCount, sizeof(AdjPtr));
    for (int u = 0; u < nodeCount; u++) {
        for (AdjPtr e = graph[u]; e; e = e->Next) {
            AdjPtr fe = calloc(1, sizeof(AdjList));
            fe->nextV = e->nextV;
            fe->edgeCap = e->edgeCap;
            fe->Next = residual[u];
            residual[u] = fe;
            AdjPtr re = calloc(1, sizeof(AdjList));
            re->nextV = u;
            re->edgeCap = 0;
            re->Next = residual[e->nextV];
            residual[e->nextV] = re;
        }
    }
    return residual;
}

bool dfs(AdjPtr *residualGraph, int *visited, int src, int dest, int *parent) {
    visited[src] = 1;
    if (src == dest) return true;
    for (AdjPtr e = residualGraph[src]; e; e = e->Next) {
        int v = e->nextV;
        if (!visited[v] && e->edgeCap > 0) {
            parent[v] = src;
            if (dfs(residualGraph, visited, v, dest, parent))
                return true;
        }
    }
    return false;
}

void updateResidualGraph(AdjPtr *residualGraph, int *parent, int src, int dest, int pathFlow) {
    for (int v = dest; v != src; v = parent[v]) {
        int u = parent[v];
        AdjPtr e;
        for (e = residualGraph[u]; e && e->nextV != v; e = e->Next);
        if (e) e->edgeCap -= pathFlow;
        for (e = residualGraph[v]; e && e->nextV != u; e = e->Next);
        if (e) e->edgeCap += pathFlow;
    }
}

int fordFulkerson(AdjPtr *graph, int nodeCount, int src, int dest) {
    AdjPtr *residual = createResidualGraph(graph, nodeCount);
    int maxFlow = 0;
    int *parent = calloc(nodeCount, sizeof(int));
    int *visited = calloc(nodeCount, sizeof(int));
    while (true) {
        memset(visited, 0, nodeCount * sizeof(int));
        memset(parent, -1, nodeCount * sizeof(int));
        if (!dfs(residual, visited, src, dest, parent))
            break;
        int pathFlow = INT_MAX;
        for (int v = dest; v != src; v = parent[v]) {
            int u = parent[v];
            AdjPtr e;
            for (e = residual[u]; e && e->nextV != v; e = e->Next);
            if (e && e->edgeCap < pathFlow)
                pathFlow = e->edgeCap;
        }
        updateResidualGraph(residual, parent, src, dest, pathFlow);
        maxFlow += pathFlow;
    }
    for (int i = 0; i < nodeCount; i++) {
        AdjPtr e = residual[i];
        while (e) {
            AdjPtr t = e;
            e = e->Next;
            free(t);
        }
    }
    free(residual);
    free(parent);
    free(visited);
    return maxFlow;
}
