/*****************************************************************
 * BUAA Fall 2023 Fundamentals of Computer Hardware
 * Project4 Assembler and Linker
 *****************************************************************
 * my_assembler_utils.c
 * Assembler Submission
 *****************************************************************/

#include <string.h>
#include <stdlib.h>

#include "lib/translate_utils.h"
#include "lib_assembler/assembler_utils.h"
#include "my_assembler_utils.h"

/*
 * You should implement functions below.
 * Detailed information is in my_assembler_utils.h.
 */

int read_data_segment(FILE *input, SymbolTable *symtbl)
{
    char line[1000], label[100] = "";
    int bytes = 0, segment_bytes = 0, offset = 0;

    while (fgets(line, sizeof(line), input) != NULL && line[0] != '\n' && line[0] != '\r')
    {
        if (line[0] == '.')
        {
            continue;
        }

        skip_comment(line);

        // Extract label name
        for (int i = 0; line[i] != '\0'; i++)
        {
            if (line[i] != ':')
            {
                label[i] = line[i];
            }
            else
            {
                label[i] = '\0';
                if (!is_valid_label(label))
                {
                    continue;
                }
                // Add `%` prefix
                char formatted_label[105];
                snprintf(formatted_label, sizeof(formatted_label), "%%%s", label);

                // Parse byte size
                int idx = 0;
                char size_str[100] = "";
                for (int j = i + 1; line[j] != '\0'; j++)
                {
                    if (line[j] >= '0' && line[j] <= '9')
                    {
                        size_str[idx++] = line[j];
                    }
                }
                size_str[idx] = '\0';
                segment_bytes = atoi(size_str);

                bytes += segment_bytes;
                add_to_table(symtbl, formatted_label, offset);
                offset += segment_bytes;
                break;
            }
        }
    }

    return bytes;
}

unsigned write_li(FILE *output, const char *name, char **args, int num_args) {
	if (num_args != 2)
    {
        return 0;
    }

    int immediate = atoi(args[1]);

    // Handle 16-bit immediate values
    if (immediate >= -32768 && immediate <= 32767)
    {
        fprintf(output, "addiu %s $zero %d\n", args[0], immediate);
        return 1;
    }
    // Handle 32-bit immediate values
    else if (immediate >= -2147483648 && immediate <= 2147483647)
    {
        int upper = immediate >> 16;
        int lower = immediate & 0xFFFF;

        fprintf(output, "lui $at %d\n", upper);
        fprintf(output, "ori %s $at %d\n", args[0], lower);
        return 2;
    }

    return 0;
}

int write_rtype(uint8_t funct, FILE *output, char **args, size_t num_args)
{
    if (num_args != 3 || funct == 0x00 || funct == 0x08 || funct == 0x19 || funct == 0x1b)
    {
        return -1;
    }

    int rd = translate_reg(args[0]);
    int rs = translate_reg(args[1]);
    int rt = translate_reg(args[2]);

    if (rd == -1 || rs == -1 || rt == -1)
    {
        return -1;
    }

    uint32_t instruction = (rs << 21) | (rt << 16) | (rd << 11) | funct;
    write_inst_hex(output, instruction);

    return 0;
}
