#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <limits.h> 

typedef struct AdjList {
    int nextV;          
    int edgeCap;        
    int flow;           
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

    char (*refList)[4] = (char (*)[4])calloc(2 * routeCnt, sizeof(char[4]));
    AdjPtr *graph = (AdjPtr *)calloc(2 * routeCnt, sizeof(AdjPtr));
    createGraph(graph, routeCnt, refList);

    int srcIndex = getIndex(src, refList);
    int destIndex = getIndex(dest, refList);

    int maxFlow = fordFulkerson(graph, 2 * routeCnt, srcIndex, destIndex);
    printf("%d", maxFlow);

    free(refList);
    for (int i = 0; i < 2 * routeCnt; i++) {
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
    listTop++;
    return listTop - 1;
}

void createGraph(AdjPtr *graph, int r_num, char (*refList)[4]) {
    int src, dest, cap;
    char srcStr[4], destStr[4];
    AdjPtr temp_ptr;

    for (int i = 0; i < r_num; i++) {
        scanf("%3s %3s %d", srcStr, destStr, &cap);
        src = getIndex(srcStr, refList);
        dest = getIndex(destStr, refList);

        temp_ptr = (AdjPtr)calloc(1, sizeof(AdjList));
        temp_ptr->edgeCap = cap;
        temp_ptr->nextV = dest;
        temp_ptr->flow = 0; 
        temp_ptr->Next = graph[src];
        graph[src] = temp_ptr;
    }
}

AdjPtr *createResidualGraph(AdjPtr *graph, int nodeCount) {
    AdjPtr *residualGraph = (AdjPtr *)calloc(nodeCount, sizeof(AdjPtr));
    for (int i = 0; i < nodeCount; i++) {
        AdjPtr temp = graph[i];
        while (temp) {
            AdjPtr forwardEdge = (AdjPtr)calloc(1, sizeof(AdjList));
            forwardEdge->nextV = temp->nextV;
            forwardEdge->edgeCap = temp->edgeCap;
            forwardEdge->flow = 0;
            forwardEdge->Next = residualGraph[i];
            residualGraph[i] = forwardEdge;

            AdjPtr reverseEdge = (AdjPtr)calloc(1, sizeof(AdjList));
            reverseEdge->nextV = i;
            reverseEdge->edgeCap = 0;
            reverseEdge->flow = 0;
            reverseEdge->Next = residualGraph[temp->nextV];
            residualGraph[temp->nextV] = reverseEdge;

            temp = temp->Next;
        }
    }

    return residualGraph;
}

bool dfs(AdjPtr *residualGraph, int *visited, int src, int dest, int *parent) {
    visited[src] = 1;
    if (src == dest) return true;

    AdjPtr temp = residualGraph[src];
    while (temp) {
        int next = temp->nextV;
        if (!visited[next] && (temp->edgeCap - temp->flow > 0)) {
            parent[next] = src;
            if (dfs(residualGraph, visited, next, dest, parent)) {
                return true;
            }
        }
        temp = temp->Next;
    }
    return false;
}

void updateResidualGraph(AdjPtr *residualGraph, int *parent, int src, int dest, int pathFlow) {
    for (int v = dest; v != src; v = parent[v]) {
        int u = parent[v];

        AdjPtr temp = residualGraph[u];
        while (temp && temp->nextV != v) {
            temp = temp->Next;
        }
        if (temp) {
            temp->flow += pathFlow;
            temp->edgeCap -= pathFlow;
        }

        temp = residualGraph[v];
        while (temp && temp->nextV != u) {
            temp = temp->Next;
        }
        if (temp) {
            temp->edgeCap += pathFlow;
        }
    }
}

int fordFulkerson(AdjPtr *graph, int nodeCount, int src, int dest) {
    AdjPtr *residualGraph = createResidualGraph(graph, nodeCount);
    int maxFlow = 0;
    int *parent = (int *)calloc(nodeCount, sizeof(int));
    int *visited = (int *)calloc(nodeCount, sizeof(int));

    while (1) {
        memset(visited, 0, nodeCount * sizeof(int));
        memset(parent, -1, nodeCount * sizeof(int));

        if (!dfs(residualGraph, visited, src, dest, parent)) {
            break;
        }

        int pathFlow = INT_MAX;
        for (int v = dest; v != src; v = parent[v]) {
            int u = parent[v];
            AdjPtr temp = residualGraph[u];
            while (temp && temp->nextV != v) {
                temp = temp->Next;
            }
            if (temp) {
                pathFlow = (temp->edgeCap - temp->flow < pathFlow) ? (temp->edgeCap - temp->flow) : pathFlow;
            }
        }

        updateResidualGraph(residualGraph, parent, src, dest, pathFlow);

        maxFlow += pathFlow;
    }

    for (int i = 0; i < nodeCount; i++) {
        AdjPtr temp = residualGraph[i];
        while (temp) {
            AdjPtr toFree = temp;
            temp = temp->Next;
            free(toFree);
        }
    }
    free(residualGraph);
    free(parent);
    free(visited);

    return maxFlow;
}