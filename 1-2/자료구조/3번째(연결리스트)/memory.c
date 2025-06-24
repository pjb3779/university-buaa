#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

typedef struct node{
	int addr;
	int size;
	struct node* prev;
	struct node* next;
}node;

int n;

node* createnode(int addr, int size){
	node* newnode = (node*)malloc(sizeof(node));
	
	newnode->addr = addr;
	newnode->size = size;
	newnode->next = NULL;
	newnode->prev = NULL;
	return newnode;
}

node* createlist(int n){
	int addr, size;
	
	node* head = NULL;
	node* tail = NULL;
	
	for(int i = 0; i < n; i++){
		scanf("%d %d", &addr, &size);
		
		node* newnode = createnode(addr, size);
		if(head == NULL){
			head = newnode;
		} else {
			tail->next = newnode;
			newnode->prev = tail;
		}
		tail = newnode;//첫 연결시 헤드이자 테일 
	}
	if(tail){//원형 연결 
		tail->next = head;
		head->prev = tail;
	}
	return head;
}

void printlist(node* list){
	if(list == NULL) return;
	
	node* start = list;
	do {
		printf("%d %d\n", list->addr, list->size);
		list = list->next;
	} while(list != start);
} 

void freelist(node* list) {
    if (list == NULL) return;

    node* start = list;
    node* curr = list->next;
    while (curr != start) {
        node* temp = curr;
        curr = curr->next;
        free(temp);
    }
    free(start);
}

node* bestfit(node* list, int target){
	node* curr = list;
	node* bestnode = NULL;
	int best = INT_MAX;
	
	int flag = 0;// while문 실행용 
	while(curr != list || flag == 0){
		flag = 1;
		
		if(curr->size == target){//정확히 맞을경우 
			if(curr->next == curr){// 하나남았을경우 처리 
				free(curr);
				return NULL;
			} 
		
			curr->prev->next = curr->next;
			curr->next->prev = curr->prev;
			node* temp = curr;
			curr = curr->next;
			
			free(temp);
			
			return curr;
		} else if(curr->size < target){// 타겟이 더 클경우 패스 
			curr = curr->next;
		} else {
			if(curr->size < best){
				best = curr->size;
				bestnode = curr;
			}
			curr = curr->next;
		}
	}
	//bestfit알고리즘 구현 
	if(best != INT_MAX){
		bestnode->size -= target;
		return bestnode;
	} else {
		return list;
	}
}

node* run(node* list){
	int target;
	while(1){
		scanf("%d", &target);
		if(target == -1) break;
		
		list = bestfit(list, target);
	}
	return list;
}
//list객체 그대로 반환하기 
//리스트 객체를 그대로 반환해야 순서에 맞게 진행 
int main(){
	scanf("%d", &n);
	
	node* list = createlist(n);
	list = run(list);
	
	printlist(list);
	freelist(list);
	
	return 0;
}
