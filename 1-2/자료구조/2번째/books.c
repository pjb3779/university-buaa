#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char title[51];
    char author[21];
    char publisher[31];
    char date[11];
} Book;

Book books[501];
int book_count = 0;

void load_books(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) return;
    while (fscanf(file, "%s %s %s %s", books[book_count].title, books[book_count].author, books[book_count].publisher, books[book_count].date) == 4) {
        book_count++;
    }
    fclose(file);
}

void save_books(const char *filename) {
    FILE *file = fopen(filename, "w");
    if (!file) return;
    for (int i = 0; i < book_count; i++) {
        fprintf(file, "%-50s%-20s%-30s%-10s\n", books[i].title, books[i].author, books[i].publisher, books[i].date);
    }
    fclose(file);
}

// 책 제목 기준으로 도서 정보를 오름차순 정렬하는 함수 (삽입 정렬 사용)
void sort_books() {
    for (int i = 1; i < book_count; i++) {
        Book temp = books[i];
        int j = i - 1;
        while (j >= 0 && strcmp(books[j].title, temp.title) > 0) {
            books[j + 1] = books[j];	//오른쪽으로 밀기 
            j--;
        }
        books[j + 1] = temp;
    }
}

// 도서 정보 추가 및 정렬
void add_book(Book new_book) {
    if (book_count >= 500) return;  // 최대 500권 제한
    books[book_count++] = new_book;
    sort_books();  // 추가 후 정렬
}

// 주어진 키워드가 포함된 책 제목을 검색하고 출력
void search_books(const char *keyword) {
    for (int i = 0; i < book_count; i++) {
        if (strstr(books[i].title, keyword)) {  // 부분 문자열 매칭
            printf("%-50s%-20s%-30s%-10s\n", 
                books[i].title, books[i].author, books[i].publisher, books[i].date);
        }
    }
}

// 주어진 키워드가 포함된 책 제목 삭제
void delete_books(const char *keyword) {
    int j = 0;
    for (int i = 0; i < book_count; i++) {
        // 키워드가 포함되지 않은 항목만 보존
        if (!strstr(books[i].title, keyword)) {
            books[j++] = books[i];
        }
    }
    book_count = j;  // 삭제된 수만큼 도서 수 갱신
}

int main() {
    load_books("books.txt");
    sort_books();
    
    int choice;
    while (1) {
        scanf("%d", &choice);
        if (choice == 0) {
            save_books("ordered.txt");
            break;
        } else if (choice == 1) {
            Book new_book;
            scanf("%s %s %s %s", new_book.title, new_book.author, new_book.publisher, new_book.date);
            add_book(new_book);
        } else if (choice == 2) {
            char keyword[51];
            scanf("%s", keyword);
            search_books(keyword);
        } else if (choice == 3) {
            char keyword[51];
            scanf("%s", keyword);
            delete_books(keyword);
        }
    }
    return 0;
}

//char *strstr(const char *haystack, const char *needle); 문자열내 특정 문자 검색 
