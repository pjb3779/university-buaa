#include <stdio.h>
#define MAXE 200
#define MAXV 200

typedef struct
{
    int id;
    int vex1;   
    int vex2;  
    int weight; 
} Edge;

int cmp1(Edge* x, Edge* y)
{
    if (x->weight < y->weight)
        return -1;
    else
        return 1;
}

int cmp2(const void* x, const void* y) { return *(int*)x - *(int*)y; }

void graph_kruskal(Edge* E, int n, int e)
{
    int i, j, sn1, sn2, k, sum = 0;
    int m[200] = { 0 };
    int vset[200] = { 0 };
    for (i = 1; i <= n; i++)
        vset[i] = i;
    k = 1;
    j = 0;
    while (k <= e)
    {
        sn1 = vset[E[j].vex1];
        sn2 = vset[E[j].vex2];
        if (sn1 != sn2)
        { 
            m[k] = E[j].id;
            sum += E[j].weight;
            k++; 
            if (k >= n)
                break;
            for (i = 1; i <= n; i++) 
                if (vset[i] == sn2)
                    vset[i] = sn1;
        }
        j++;
    }
    printf("%d", sum);
    printf("\n");
    qsort(m, n, sizeof(m[0]), cmp2);
    for (i = 1; i < n; i++)
    {
        printf("%d ", m[i]);
    }
}

int main()
{
    Edge E[MAXE];
    int i, nume, numn;
    scanf("%d%d", &numn, &nume);
    for ( i = 0; i < nume; i++)
        scanf("%d%d%d%d", &E[i].id, &E[i].vex1, &E[i].vex2, &E[i].weight);
    qsort(E, nume, sizeof(E[0]), (const void*)cmp1);
    graph_kruskal(E, numn, nume);
}



