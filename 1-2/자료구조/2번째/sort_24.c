#include <stdio.h>
#include <string.h>

typedef struct pho{
    char name[20];
    char num[20];
    int ans;
    int flag;
};
int main()
{
    struct  pho s[100];
    int i,j,k;
    scanf("%d",&k);
    for(i = 1; i<= k; i++)
        {
			scanf("%s%s",s[i].name,s[i].num);
			s[i].flag = 0; 
			s[i].ans = 0;
		}       
    int end = 1;
    for(i = 1; end; i++) {
		end = 0;
		char temp1[25],temp2[25];
		for(j=k;j>=i+1;j--) {
            if(strcmp(s[j-1].name,s[j].name)>0)
            {
                strcpy(temp1,s[j].name);
                strcpy(s[j].name,s[j-1].name);
                strcpy(s[j-1].name,temp1);
                strcpy(temp2,s[j].num);
                strcpy(s[j].num,s[j-1].num);
                strcpy(s[j-1].num,temp2);
                end = 1;
            }
		}
	}
    for(i = 1; i <= k; i++)
    {
        if(strcmp(s[i].name,s[i+1].name)==0)
        {
            if(strcmp(s[i].num,s[i+1].num)==0)
                s[i+1].flag = 1;
            else s[i+1].ans = s[i].ans + 1;
        }
        if(s[i].ans >=1 &&s[i].flag == 0) 
		{
			printf("%s_%d %s\n",s[i].name,s[i].ans,s[i].num);
		}
        if(s[i].ans == 0 && s[i].flag == 0) 
		{
			printf("%s %s\n",s[i].name,s[i].num);
		}
        if(s[i].flag == 1) continue;
    }
    return 0;
}


