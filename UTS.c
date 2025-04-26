#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

typedef struct AdjList {
    int nextV;
    int edgeCap;
    struct AdjList *Next;
} AdjList;
typedef AdjList *AdjPtr;

int getIndex(char name[4], char (*refList)[4]);
void createGraph(AdjPtr *graph, int r_num, char (*refList)[4]);

int listTop = 0;

int main() {
    char src[4], dest[4];
    int routeCnt;
    scanf("%3s %3s", src, dest);
    scanf("%d", &routeCnt);
    char (*refList)[4] = (char (*)[4])calloc(2 * routeCnt, sizeof(char[4]));
    AdjPtr *graph = (AdjPtr *)calloc(2 * routeCnt, sizeof(AdjPtr));
    createGraph(graph, routeCnt, refList);
    




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
        temp_ptr->Next = graph[src];
        graph[src] = temp_ptr;
    }
}