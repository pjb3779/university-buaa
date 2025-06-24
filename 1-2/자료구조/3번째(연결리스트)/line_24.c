#include <stdio.h>
#include <string.h>

typedef struct xian				
{
    int beginx;
    int beginy;
    int endx;
    int endy;
    int num;				
}xian[100];



int cmpfuc(const void *a, const void *b)			
{													
    struct xian *c = (xian *)a;						
    struct xian *d = (xian *)b;
    if(c->beginx!=d->beginx) return d->beginx - c->beginx;
    else return d->beginy - c->beginy;
}
int main()
{
    struct xian xian[100];
    int g;
    scanf("%d",&g);									
    int i,j;
    for(i = 0; i < g; i++)
        scanf("%d%d%d%d",&xian[i].beginx,&xian[i].beginy,&xian[i].endx,&xian[i].endy),xian[i].num = 1;

    qsort(xian,g,sizeof(xian[0]),cmpfuc);					


    for(i = 0; i < g-1; i++)
    {
        for(j = i; j < g-1; j++)					
        {											
            if(xian[i].beginx==xian[j+1].endx && xian[i].beginy == xian[j+1].endy)
            {
                xian[j+1].num += xian[i].num;
            }
        }
    }
    int max = -1; int flag = 0;
    for(i = 0; i < g; i++)
    {
        if(xian[i].num > max)
            max = xian[i].num,flag = i;				
    }
    printf("%d %d %d",max,xian[flag].beginx,xian[flag].beginy);		
    return 0;
}


