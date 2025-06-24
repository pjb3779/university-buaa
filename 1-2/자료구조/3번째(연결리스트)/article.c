#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h> 

typedef struct word{
	char word[100];
	int cnt;
}Word;

Word word[1000];

int cmp(const void *a, const void *b){
	return strcmp(((Word*)a)->word, ((Word*)b)->word);
}

int main(){
	FILE *file = fopen("article.txt", "r");
	
	if (file == NULL) {
        printf("Cannot open file\n");
        return 1;
    }
    
	int wordNum = 0;
	char buffer[100];
	
	char ch;
	int idx = 0;
	
	while ((ch = fgetc(file)) != EOF) {
	    if (isalpha(ch)) {
	        buffer[idx++] = tolower(ch);
	        //if (idx >= 99) idx = 99;  // 단어가 너무 길어질 경우 보호
	    } else if (idx > 0) {  // 단어가 끝났다면
	        buffer[idx] = '\0';
	        idx = 0;
	
	        int found = 0;
	        for (int i = 0; i < wordNum; i++) {
	            if (strcmp(word[i].word, buffer) == 0) {
	                word[i].cnt++;
	                found = 1;
	                break;
	            }
	        }
	
	        if (!found && wordNum < 1000) {
	            strcpy(word[wordNum].word, buffer);
	            word[wordNum].cnt = 1;
	            wordNum++;
	        }
	    }
	}
	
	fclose(file);
	
	qsort(word, wordNum, sizeof(Word), cmp);
	
	for(int i = 0; i < wordNum; i++){
		printf("%s %d\n", word[i].word, word[i].cnt);
	}
	
	return 0;
}
