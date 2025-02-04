#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cachelab.h"
int hits = 0;    
int misses = 0;   
int evictions = 0; 
int time = 0;  
typedef struct _cache_line {
    char valid;  
    unsigned int tag;  
    unsigned long long int lru; 
} cache_line;                    

typedef struct _cache_set {
    cache_line* lines;  

} cache_set;  

typedef struct _cache {
    cache_set* sets; 
} cache;              

cache init_cache(int s, int E, int b) {
    cache c;
    c.sets = (cache_set*)malloc(sizeof(cache_set) * (1 << s));
    for (int i = 0; i < (1 << s); i++) {
        c.sets[i].lines = (cache_line*)malloc(sizeof(cache_line) * E);
        for (int j = 0; j < E; j++) {
            c.sets[i].lines[j].valid = 0; 
            c.sets[i].lines[j].tag = 0;  
            c.sets[i].lines[j].lru = 0; 
        }
    }
    return c;
}

void free_cache(cache c, int s) {
    for (int i = 0; i < (1 << s); i++) {
        free(c.sets[i].lines);
    }
    free(c.sets);
}

void access_cache(cache c,
                  int s,
                  int E,
                  int b,
                  unsigned long long int addr,
                  int verbose) {
    unsigned long long int tag = addr >> (s + b);               
    unsigned long long int set = (addr >> b) & ((1 << s) - 1); 
    int i;
    for (i = 0; i < E; i++) {
        if (c.sets[set].lines[i].valid == 0) {
            // 无效行,直接写入
            c.sets[set].lines[i].valid = 1;
            c.sets[set].lines[i].tag = tag;
            c.sets[set].lines[i].lru = time++;
            misses++;
            if (verbose) {
                printf("miss ");
            }
            return;
        } else if (c.sets[set].lines[i].tag == tag) {
            // 命中
            hits++;
            c.sets[set].lines[i].lru = time++;
            if (verbose) {
                printf("hit ");
            }
            return;
        }
    }
    misses++; 
    evictions++;
    int min = 0;
    for (i = 0; i < E; i++) {
        if (c.sets[set].lines[i].lru < c.sets[set].lines[min].lru) {
            min = i;
        }
    }
    c.sets[set].lines[min].tag = tag;
    c.sets[set].lines[min].lru = time++;
    if (verbose) {
        printf("miss eviction ");
    }

    return;
}

void print_summary() {
    printf("hits:%d misses:%d evictions:%d\n", hits, misses, evictions);
    // printf("hit rate:%.2f%%\n", (double)hits / (hits + misses) * 100);
}

int main(int argc, char* argv[]) {
    int s = 0;            
    int E = 0;             
    int b = 0;              
    int verbose = 0;         
    char* trace_file = NULL;  
    char c;
    while ((c = getopt(argc, argv, "hvs:E:b:t:")) != -1) {
        switch (c) {
            case 's':
                s = atoi(optarg);
                break;
            case 'E':
                E = atoi(optarg);
                break;
            case 'b':
                b = atoi(optarg);
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
    if (s == 0 || E == 0 || b == 0 || trace_file == NULL) {
        printf("Usage: %s [-hv] -s <s> -E <E> -b <b> -t <tracefile>\n",
               argv[0]);
        exit(1);
    }

    cache cache = init_cache(s, E, b);
    FILE* file = fopen(trace_file, "r");
    if (file == NULL) {
        printf("Error opening the file\n");
        exit(1);
    }
    char buf[100];
    while (fgets(buf, 100, file) != NULL) {
        time++;
        char op;
        unsigned long long int addr;
        int size;
        sscanf(buf, " %c %llx,%d", &op, &addr, &size);
        if (verbose) {
            printf("%c %llx,%d ", op, addr, size);
        }
        if (op == 'L' || op == 'S') {
            access_cache(cache, s, E, b, addr, verbose);
        } else if (op == 'M') {
            access_cache(cache, s, E, b, addr, verbose);
            access_cache(cache, s, E, b, addr, verbose);
        }
        if (verbose) {
            printf("\n");
        }
    }
    print_summarys();
    return 0;
}
