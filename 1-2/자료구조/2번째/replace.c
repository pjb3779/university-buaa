#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// 문자열 비교 함수 (대소문자 무시, 길이 len만큼 비교)
int str_casecmp(const char *a, const char *b, int len) {
    for (int i = 0; i < len; i++) {
        if (tolower((unsigned char)a[i]) != tolower((unsigned char)b[i])) {
            return 0;
        }
    }
    return 1;
}

// 문자열 치환 함수
void replace_str(FILE *input, FILE *output, const char *target, const char *replace) {
    char line[1001];
    int target_len = strlen(target);
    int replace_len = strlen(replace);

    // 한 줄씩 읽기
    while (fgets(line, 1001, input)) {
        char *pos = line;
        while (*pos) {
            // 현재 위치에서 target_len만큼 읽어서 비교
            if (strnlen(pos, target_len) >= target_len && str_casecmp(pos, target, target_len)) {
                // 일치하면 치환 문자열 출력
                fwrite(replace, 1, replace_len, output);
                pos += target_len;  // target 길이만큼 포인터 이동
            } else {
                // 일치하지 않으면 원래 문자 출력
                fputc(*pos, output);
                pos++;
            }
        }
    }
}

int main() {
    char target[100], replace[100];
    FILE *input, *output;

    fgets(target, sizeof(target), stdin);
    fgets(replace, sizeof(replace), stdin);

	//fgets로 입력받을 때 개행 문자(\n)를 제거하는 데 자주 사용됨
    target[strcspn(target, "\n")] = '\0';
    replace[strcspn(replace, "\n")] = '\0';

    input = fopen("filein.txt", "r");
    output = fopen("fileout.txt", "w");


    // 문자열 치환 실행
    replace_str(input, output, target, replace);

    // 파일 닫기
    fclose(input);
    fclose(output);

    return 0;
}

