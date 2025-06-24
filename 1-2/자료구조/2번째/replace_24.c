#include <stdio.h>
#include <string.h>
#include <ctype.h>

void replaceString(char *source, const char *old_str, const char *new_str) {
    char *temp = source;
    int old_len = strlen(old_str);
    int new_len = strlen(new_str);
    
    while ((temp = strstr(temp, old_str)) != NULL) {
        memmove(temp + new_len, temp + old_len, strlen(temp + old_len) + 1);
        memcpy(temp, new_str, new_len);
        temp += new_len;
    }
}

void toLower(char *str) {
    while (*str) {
        *str = tolower(*str);
        str++;
    }
}

int main() {
    char old_str[100];
    char new_str[100];

    fgets(old_str, 100, stdin);
    fgets(new_str, 100, stdin);

    old_str[strcspn(old_str, "\n")] = '\0';
    new_str[strcspn(new_str, "\n")] = '\0';

    //toLower(old_str);

    FILE *inputFile = fopen("filein.txt", "r");
    FILE *outputFile = fopen("fileout.txt", "w");

    char line[100];

    while (fgets(line, sizeof(line), inputFile)) {
        replaceString(line, old_str, new_str);
        fputs(line, outputFile);
    }

    fclose(inputFile);
    fclose(outputFile);

    return 0;
}




