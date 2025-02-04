/*****************************************************************
 * BUAA Fall 2023 Fundamentals of Computer Hardware
 * Project4 Assembler and Linker
 *****************************************************************
 * my_linker_utils.c
 * Linker Submission
 *****************************************************************/

#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "lib_linker/linker_utils.h"
#include "my_linker_utils.h"

/*
 * You should implement functions below.
 * Detailed information is in my_assembler_utils.h.
 */

int inst_needs_relocation(SymbolTable *reltbl, uint32_t offset) {
    // get_symbol_for_addr()를 사용하여 offset이 재배치 테이블(reltbl)에 존재하는지 확인합니다.
    if (get_symbol_for_addr(reltbl, offset) != NULL) {
        return 1; // 재배치 필요 
    }
    return 0; 
}

int add_to_symbol_table(FILE *input, SymbolTable *table, uint32_t base_text_offset, uint32_t base_data_offset) {
    char line[1024];
    while (fgets(line, sizeof(line), input)) {
        // 빈 줄을 만나면 처리를 끝 
        if (line[0] == '\n' || line[0] == '\r') {
            break;
        }

        //주소와 심볼로 분리합니다.
        char *addr_str = strtok(line, "\t");
        char *symbol = strtok(NULL, "\t\n\r");

        if (addr_str == NULL || symbol == NULL) {
            return -1; // 실패 
        }

        // 주소를 정수로 변환
        uint32_t relative_addr = (uint32_t)strtol(addr_str, NULL, 10);
        uint32_t absolute_addr;

        // 세그먼트 유형에 따라 절대 주소를 결정.
        if (symbol[0] == '%') {
            // .data 세그먼트 심볼에서 `%`제거.
            char cleaned_symbol[1024];
            snprintf(cleaned_symbol, sizeof(cleaned_symbol), "%s", symbol + 1);
            absolute_addr = relative_addr + base_data_offset;
            if (add_to_table(table, cleaned_symbol, absolute_addr) == -1) {
                return -1; // 실패
            }
        } else {
            // .text 세그먼트에서 온 심볼
            absolute_addr = relative_addr + base_text_offset;
            if (add_to_table(table, symbol, absolute_addr) == -1) {
                return -1; 
            }
        }
    }

    return 0; 
}


