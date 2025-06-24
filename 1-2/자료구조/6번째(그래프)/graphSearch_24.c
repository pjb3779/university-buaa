#include <stdio.h>
#include <string.h>

int a, b, str[102][102], temp[102], tempp[102], flag;
void Dfs(int p);
void Bfs(int p);

int main()
{
    int i,k,x, y;
    scanf("%d%d", &a, &b);
    for (i = 1; i <= b; i++) 
    {
        scanf("%d%d", &x, &y);
        str[x][y] = 1;
        str[y][x] = 1;
    }
    Dfs(0);
    printf("\n");
    for (k = 0; k < 102; k++)
    {
        temp[k] = 0;
        flag = 0;
    }
    Bfs(0);
    printf("\n");
    for (k = 0; k < 102; k++)
    {
        temp[k] = 0;
        flag = 0;
    }
    scanf("%d", &x);
    temp[x] = 1;
    Dfs(0);
    printf("\n");
    for (k = 0; k < 102; k++)
    {
        temp[k] = 0;
        flag = 0;
    }
    temp[x] = 1;
    Bfs(0);
    printf("\n");
    for (k = 0; k < 102; k++)
    {
        temp[k] = 0;
        flag = 0;
    }
}

void Dfs(int p)
{
	int i;
    if (flag == 0)
    {
        printf("%d", p);
        flag = 1;
    }
    else
    {
        printf(" %d", p);
    }
    temp[p] = 1;
    for (i = 1; i <= a; i++)
    {
        if (str[p][i] == 1 && temp[i] == 0)
        {
            Dfs(i);
        }
    }
}

void Bfs(int p)
{
    int i,j,r,t;
    tempp[1] = p;
    for (j = 1, r = 1; j <= r;)
    {
        t = tempp[j++];
        if (temp[t] == 1)
        {
            continue;
        }
        if (flag == 0)
        {
            printf("%d", t);
            flag = 1;
        }
        else
        {
            printf(" %d", t);
        }

        temp[t] = 1;
        for (i = 1; i <= a; i++)
        {
            if (str[t][i] == 1 && temp[i] == 0)
            {
                tempp[++r] = i;
            }
        }
    }
}


