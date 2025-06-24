#include <stdio.h>
#include <string.h>

struct node
{
    int co;                     
    int en;
} a[100000], b[100000], p[100000], q[100000];

int cnta = 0, cntb = 0;
char c = ' ';
int cmpfuc(const void* a, const void* b)            
{
    struct node* c = (struct node*)a;
    struct node* d = (struct node*)b;
    return d->en - c->en;
}
int main()
{
	int i,j;
    while (c != '\n')                                
    {
        scanf("%d%d%c", &a[cnta].co, &a[cnta].en, &c);
        ++cnta;
    }
    c = ' ';
    while (c != '\n')                                   
    {
        scanf("%d%d%c", &b[cntb].co, &b[cntb].en, &c);
        ++cntb;
    }
    int k = 0;
    for (i = 0; i < cnta; i++)                      
        for (j = 0; j < cntb; j++)
        {
            p[k].co = a[i].co * b[j].co;
            p[k].en = a[i].en + b[j].en;
            k++;
        }
    qsort(p, k, sizeof(p[0]), cmpfuc);                   
    for (i = 0; i < k; i++)
    {
        if (p[i].en == p[i + 1].en && p[i].en != 0)        
        {
            p[i + 1].co += p[i].co;
            p[i].co = 0;
        }
    }
    for (i = 0; i < k; i++)
        if (p[i].co != 0)
            printf("%d %d ", p[i].co, p[i].en); 
	if(p[i].co==p[i].en)
	{
		p[i].co+=1;
		p[i].en-=1;
	}


    return 0;
}


