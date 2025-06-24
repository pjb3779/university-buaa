#include <stdio.h>
#include <string.h>

struct books
{
    char name[51];
    char zuozhe[21];
    char chu[41];
    char day[11];
}book[505],t,enter;
int n = 0;
char s[1000004];


int cmpfuc(const void *a, const void *b) {
	struct books *c = (struct books*)a;
	struct books *d = (struct books*)b;
	return strcmp(c->name,d->name);
}

int main()
{
    FILE *fp = fopen("books.txt","r");
    FILE *fp2 = fopen("ordered.txt","w");
	struct books book[505];

    while( 4 == fscanf(fp,"%s %s %s %s\n",book[n].name,book[n].zuozhe,book[n].chu,book[n].day))  
		{
			n++;
		}	
    qsort(book,n,sizeof(book[0]),cmpfuc);									
    int i,j,opt;
    char name[51];
    while(scanf("%d",&opt)==1)
    {
        switch(opt)
		{
			case 1:
			scanf("%s%s%s%s",book[n].name,book[n].zuozhe,book[n].chu,book[n].day);
			n++;
			qsort(book,n,sizeof(book[0]),cmpfuc);

			break;
			case 2:
				scanf("%s",name);
				for(i = 0; i < n; i++)
					if( strstr(book[i].name,name)!=NULL)
						printf("%-50s%-20s%-30s%-10s\n",book[i].name,book[i].zuozhe,book[i].chu,book[i].day);
				break;
			case 3:
				scanf("%s",name);
				for(i = 0; i < n; i++)
					if(strstr(book[i].name,name)!=NULL)
					{
						for(j = i; j < n; j++)
							book[j] = book[j+1];
						n--;i--;
					}
			default: break;
		}
		if(opt== 0)
			break;
    }
	for(i = 0; i < n; i++)
		fprintf(fp2,"%-50s%-20s%-30s%-10s\n",book[i].name,book[i].zuozhe,book[i].chu,book[i].day);
    fclose(fp2);
    fclose(fp);
    return 0;
}


