#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct{
    int u;
    int v;
    int weight;
} edge;
//we use edge to store the graph, which is suitable for the kruskal algorithm.
typedef edge * Graph;

void createGraph(Graph graph, int edgeCnt);
int cmp(const void *a, const void *b);
//the compare function used in the qsort

int main() {
    int vertexCnt, edgeCnt;
    scanf("%d %d", &vertexCnt, &edgeCnt);
    Graph graph = (Graph)calloc(edgeCnt, sizeof(edge));
    createGraph(graph, edgeCnt);
    qsort(graph, edgeCnt, sizeof(edge), cmp);
}

void createGraph(Graph graph, int edgeCnt) {
    for (int i = 0; i < edgeCnt; i ++) {
        scanf("%d %d %d", (graph+i)->u, (graph+i)->v, (graph+i)->weight);
    }
}

int cmp(const void *a, const void *b)
{
    edge * temp_a = (edge *)a;
    edge * temp_b = (edge *)b;
    return temp_a->weight - temp_b->weight;
}