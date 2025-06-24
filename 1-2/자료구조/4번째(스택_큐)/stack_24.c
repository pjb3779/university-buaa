#include <stdio.h>

int main()
{
	int stack[100];
	int top=0;
	int num;
	int select=3;
	
	while(select!=-1)
	{
		scanf("%d",&select);
		switch(select)
		{
			case 1:
				if(top>=100)
				{
					printf("error");
				}
				else
				{
					scanf("%d",&num);
					stack[top]=num;
					top++;
				}
				break;
			case 0:
				if(top<=0)
				{
					printf("error ");
				}
				else
				{
					top--;
					printf("%d ",stack[top]);
					stack[top]=' ';
				}
				break;
		}
	}
	return 0; 
}


