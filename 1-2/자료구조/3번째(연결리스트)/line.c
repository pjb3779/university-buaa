#include <stdio.h>

typedef struct {
	int x1, y1, x2, y2;
} Xian;

int n;
Xian xian[100];
int visited[100];
int max = 0;
int ansx = 0, ansy = 0;

void digui(int index, int count, int x, int y){
	if(count > max){
		max = count;
		ansx = x;
		ansy = y;
	}
	
	for (int i = 0; i < n; i++){	//visited·Î i Áõ°¡ 
        if (!visited[i] && xian[index].x2 == xian[i].x1 && xian[index].y2 == xian[i].y1) {
            visited[i] = 1;
            digui(i, count + 1, x, y);
            visited[i] = 0;
        }
    }
}

int main(){
	scanf("%d", &n);
    for (int i = 0; i < n; i++){
        scanf("%d %d %d %d", &xian[i].x1, &xian[i].y1, &xian[i].x2, &xian[i].y2);
    }

    for (int i = 0; i < n; i++){
        visited[i] = 1;
        digui(i, 1, xian[i].x1, xian[i].y1);
        visited[i] = 0;
    }

    printf("%d %d %d\n", max, ansx, ansy);
	return 0;
}
