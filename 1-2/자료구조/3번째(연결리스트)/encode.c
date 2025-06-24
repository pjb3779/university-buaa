#include <stdio.h>
#include <stdlib.h>
#include <string.h> 

typedef struct node{
	char c;
	struct node* prev;
	struct node* next;
}Node;

void remove_due(char *src, char *key){// 중복제거 
	int seen[128] = {0};
    int j = 0;
    for (int i = 0; src[i] && j < 32; i++) {
        char c = src[i];
        if (c >= 32 && c <= 126 && !seen[c]) {
            seen[c] = 1;
            key[j++] = c;
        }
    }
    key[j] = '\0';
}

// 환형 연결 리스트 만들기: 중복 제거된 key + 나머지 가시 문자로 구성
Node* makering(char *key){
	Node *head = NULL;
	Node *tail = NULL;
	
	int ascii[128] = {0,};
	
	// 1. key 문자들을 리스트에 먼저 추가
	for(int i = 0; key[i] ; i++){
		Node *newnode = (Node*)malloc(sizeof(Node));
		newnode->c = key[i];
		newnode->next = NULL;
		newnode->prev = NULL;
		
		ascii[(int)key[i]] = 1;// 중복 체크 등록
		
		if(!head){
			head = newnode;
			tail = newnode;
		} else {
			tail->next = newnode;
			newnode->prev = tail;
			tail = newnode;
		}	
	}
	
	// 2. 나머지 가시 문자 추가 (key에 없던 문자만)
	for (int i = 32; i <= 126; i++) {//나머지 처리 
        if (!ascii[i]) {
            Node *newnode = (Node*)malloc(sizeof(Node));
            newnode->c = (char)i;
            newnode->next = NULL;
			newnode->prev = NULL;

            tail->next = newnode;
            newnode->prev = tail;
            tail = newnode;
        }
    }
     // 3. 환형 연결 리스트 연결
    tail->next = head;
    head->prev = tail;
    
    return head;
}

void printlist(Node* list){
	if(list == NULL) return;
	
	Node* start = list;
	do {
		printf("%c",list->c);
		list = list->next;
	} while(list != start);
} 

void freelist(Node* list) {
    if (list == NULL) return;

    Node* start = list;
    Node* curr = list->next;
    while (curr != start) {
        Node* temp = curr;
        curr = curr->next;
        free(temp);
    }
    free(start);
}

// 환을 기반으로 암호화 맵 생성
void create_encode(char* encodekey, Node* ring, char first){
	Node* curr = ring;
	
	// 리스트에 노드가 하나 남을 때까지 반복
	while (curr->next != curr) {
		Node* temp = curr;
		Node* next = curr->next;

		char plain = curr->c;
		int index = (int)curr->c;// ASCII 값만큼 이동
		
		temp->prev->next = temp->next;
		temp->next->prev = temp->prev;

		curr = next;

		// index(ASCII값)만큼 시계방향 이동
		for (int i = 1; i < index; i++) {
			curr = curr->next;
		}
		encodekey[(int)plain] = curr->c;

		free(temp); 
	}
	encodekey[(int)curr->c] = first; 
	free(curr); 
}


int main(){
	char input[33];
	char key[33]; 
	char encodekey[128] = {0,};
	
	fgets(input, sizeof(input), stdin);// 문자열 저장 버퍼/ 최대 문자수/ 읽어올대상 
	input[strcspn(input, "\n")] = '\0'; // 개행 제거 strcspn = 첫 등장 위치 반환 
	
	remove_due(input, key);
	Node* ring = makering(key);
	
//	printlist(ring);
	create_encode(encodekey, ring, key[0]);
	
//	printf("\n");printf("\n");
//	for (int i = 32; i <= 126; i++) {
//    	if (encodekey[i] != 0) {
//     	   printf("%c", i);
//    	}
//	}
//	printf("\n");
//	for (int i = 32; i <= 126; i++) {
//    	if (encodekey[i] != 0) {
//        	printf("%c", encodekey[i]);
//   		}
//	}
//	printf("\n");

	FILE *fin = fopen("in.txt", "r");
	FILE *fout = fopen("in_crpyt.txt", "w");
	
	if(!fin || !fout){
		perror("파일 열기 tlqkf 실패");
		return 1; 
	}
	
	int c;
	
	while((c = fgetc(fin)) != EOF){
		if(c >= 32 && c <= 126){
			fputc(encodekey[c], fout);//fputc 에서 c는 인트형이어야함
			//fputs는 char문자열 
		} else {
			fputc(c, fout);
		}
	}
	
	fclose(fin);
	fclose(fout);
	
	return 0;
}
//freelist(ring);
//객체를 free하면 NULL이 아니라 해제된 포인터임 그래서 같은객체를 두번 free할경우 배열초과 에러 발생 
