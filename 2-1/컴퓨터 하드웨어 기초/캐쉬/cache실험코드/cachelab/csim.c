#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cachelab.h"

int hits = 0;    
int misses = 0;   
int evictions = 0; 
int timestamp = 0;  

typedef struct {
    char valid;  
    unsigned int tag;  
    unsigned long long int last_used; 
} CacheLine;                    

typedef struct {
    CacheLine* lines;  
} CacheSet;  

typedef struct {
    CacheSet* sets; 
} Cache;              

Cache init_cache(int set_bits, int lines_per_set, int block_bits) {
    Cache cache;
    int num_sets = (1 << set_bits);
    cache.sets = (CacheSet*)malloc(sizeof(CacheSet) * num_sets);
    for (int i = 0; i < num_sets; i++) {
        cache.sets[i].lines = (CacheLine*)malloc(sizeof(CacheLine) * lines_per_set);
        for (int j = 0; j < lines_per_set; j++) {
            cache.sets[i].lines[j].valid = 0; 
            cache.sets[i].lines[j].tag = 0;  
            cache.sets[i].lines[j].last_used = 0; 
        }
    }
    return cache;
}

void free_cache(Cache cache, int set_bits) {
    for (int i = 0; i < (1 << set_bits); i++) {
        free(cache.sets[i].lines);
    }
    free(cache.sets);
}

void access_cache(Cache cache, int set_bits, int lines_per_set, int block_bits, unsigned long long int addr, int verbose) {
    unsigned long long int tag = addr >> (set_bits + block_bits);               
    unsigned long long int set_index = (addr >> block_bits) & ((1 << set_bits) - 1); 
    int i;

    // Try to find a line with the same tag
    for (i = 0; i < lines_per_set; i++) {
        CacheLine* line = &cache.sets[set_index].lines[i];
        if (line->valid == 0) {
            // Miss: Place the block in the empty line
            line->valid = 1;
            line->tag = tag;
            line->last_used = timestamp++;
            misses++;
            if (verbose) {
                printf("miss ");
            }
            return;
        } else if (line->tag == tag) {
            // Hit: Update the LRU timestamp
            hits++;
            line->last_used = timestamp++;
            if (verbose) {
                printf("hit ");
            }
            return;
        }
    }

    // Miss and Eviction: Replace the least recently used line
    misses++; 
    evictions++;
    int lru_index = 0;
    for (i = 1; i < lines_per_set; i++) {
        if (cache.sets[set_index].lines[i].last_used < cache.sets[set_index].lines[lru_index].last_used) {
            lru_index = i;
        }
    }

    CacheLine* lru_line = &cache.sets[set_index].lines[lru_index];
    lru_line->tag = tag;
    lru_line->last_used = timestamp++;
    if (verbose) {
        printf("miss eviction ");
    }
}

void print_summary() {
    printf("hits:%d misses:%d evictions:%d\n", hits, misses, evictions);
}

int main(int argc, char* argv[]) {
    int set_bits = 0;            
    int lines_per_set = 0;             
    int block_bits = 0;              
    int verbose = 0;         
    char* trace_file = NULL;  
    char c;
    
    while ((c = getopt(argc, argv, "hvs:E:b:t:")) != -1) {
        switch (c) {
            case 's':
                set_bits = atoi(optarg);
                break;
            case 'E':
                lines_per_set = atoi(optarg);
                break;
            case 'b':
                block_bits = atoi(optarg);
                break;
            case 'v':
                verbose = 1;
                break;
            case 't':
                trace_file = optarg;
                break;
            case 'h':
                printf(
                    "Usage: %s [-hv] -s <s> -E <E> -b <b> -t < tracefile >\n ",
                    argv[0]);
                printf("Options:\n");
                printf("  -h         Print this help message.\n");
                printf("  -v         Optional verbose flag.\n");
                printf("  -s <s>     Number of set index bits.\n");
                printf("  -E <E>     Number of lines per set.\n");
                printf("  -b <b>     Number of block offset bits.\n");
                printf(
                    "  -t <tracefile>   Name of the valgrind trace to "
                    "replay.\n");
                printf("\nExamples:\n");
                printf("  linux>  %s -s 4 -E 1 -b 4 -t traces/yi.trace\n",
                       argv[0]);
                printf("  linux>  %s -v -s 8 -E 2 -b 4 -t traces/yi.trace\n",
                       argv[0]);
                exit(0);
        }
    }
    
    if (set_bits == 0 || lines_per_set == 0 || block_bits == 0 || trace_file == NULL) {
        printf("Usage: %s [-hv] -s <s> -E <E> -b <b> -t <tracefile>\n", argv[0]);
        exit(1);
    }

    Cache cache = init_cache(set_bits, lines_per_set, block_bits);
    FILE* file = fopen(trace_file, "r");
    if (file == NULL) {
        printf("Error opening the file\n");
        exit(1);
    }

    char buf[100];
    while (fgets(buf, 100, file) != NULL) {
        timestamp++;
        char op;
        unsigned long long int addr;
        int size;
        sscanf(buf, " %c %llx,%d", &op, &addr, &size);

        if (verbose) {
            printf("%c %llx,%d ", op, addr, size);
        }

        if (op == 'L' || op == 'S') {
            access_cache(cache, set_bits, lines_per_set, block_bits, addr, verbose);
        } else if (op == 'M') {
            access_cache(cache, set_bits, lines_per_set, block_bits, addr, verbose);
            access_cache(cache, set_bits, lines_per_set, block_bits, addr, verbose);
        }

        if (verbose) {
            printf("\n");
        }
    }

    print_summary();
    fclose(file);
    free_cache(cache, set_bits);
    return 0;
}

