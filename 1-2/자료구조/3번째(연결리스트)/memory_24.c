#include <stdio.h>
#include <stdlib.h>

typedef struct Block {
    int s;
    int l; 
    struct Block* n;
} Block;

Block* find_min_fit(Block* st, int sz) {
    Block *c = st, *bf = NULL;
    do {
        if (c->l >= sz && (!bf || c->l < bf->l)) {
            bf = c;
        }
        c = c->n;
    } while (c != st);
    return bf;
}

int main() {
    int cnt;
    scanf("%d", &cnt);
    Block *hd = NULL, *tl = NULL;
    
    for (int i = 0; i < cnt; i++) {
        int st, ln;
        scanf("%d %d", &st, &ln);
        Block* nb = (Block*)malloc(sizeof(Block));
        nb->s = st;
        nb->l = ln;
        nb->n = NULL;
        if (!hd) {
            hd = tl = nb;
        } else {
            tl->n = nb;
            tl = nb;
        }
    }
    tl->n = hd; 

    Block *cur = hd;
    int sz;
    while (scanf("%d", &sz) && sz != -1) {
        Block* fit = find_min_fit(cur, sz);
        if (fit && fit->l >= sz) {
            if (fit->l == sz) {
                if (fit == hd && fit == tl) {
                    hd = tl = NULL;
                } else {
                    Block* prv = hd;
                    while (prv->n != fit) {
                        prv = prv->n;
                    }
                    prv->n = fit->n;
                    if (fit == hd) {
                        hd = hd->n;
                    }
                    if (fit == tl) {
                        tl = prv;
                    }
                }
                cur = fit->n;
                free(fit);
            } else {
                fit->s += sz;
                fit->l -= sz;
                cur = fit;
            }
        }
    }

    if (hd) {
        Block *start = cur;
        do {
            printf("%d %d\n", cur->s, cur->l);
            cur = cur->n;
        } while (cur != start);
    }

    return 0;
}




