#include "binomial_heap.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>

static void load_file_into_heap(BinomialHeap **H, const char *path) {
    FILE *f = fopen(path, "r");
    if (!f) { perror("fopen"); exit(1); }
    int x;
    while (fscanf(f, "%d", &x) == 1) {
        bh_insert(H, x);
    }
    fclose(f);
}

static void help(void) {
    puts("commands:");
    puts("  load <file>         # insert all integers from file");
    puts("  insert <x>");
    puts("  min                 # show current minimum");
    puts("  extractmin          # remove and print minimum");
    puts("  decrease <old> <new>");
    puts("  delete <key>");
    puts("  union <file>        # union with heap built from file");
    puts("  print               # print heap");
    puts("  random <n> <max>    # insert n random ints in [0,max]");
    puts("  quit");
}

int main(int argc, char **argv) {
    BinomialHeap *H = bh_make();

    if (argc == 3 && strcmp(argv[1],"--build") == 0) {
        load_file_into_heap(&H, argv[2]);
        bh_print(H);
    }

    puts("Binomial Heap (CLRS) – interactive shell. Type 'help' for commands.");
    char cmd[64];
    help();

    srand((unsigned)time(NULL));
    for (;;) {
        printf("> ");
        if (scanf("%63s", cmd) != 1) break;

        if (!strcmp(cmd,"help")) help();
        else if (!strcmp(cmd,"load")) {
            char path[256]; if (scanf("%255s", path) != 1) continue;
            load_file_into_heap(&H, path);
        } else if (!strcmp(cmd,"insert")) {
            int x; if (scanf("%d",&x)!=1) continue;
            bh_insert(&H, x);
        } else if (!strcmp(cmd,"min")) {
            BHNode *m = bh_minimum(H);
            if (m) printf("min = %d\n", m->key); else puts("(empty)");
        } else if (!strcmp(cmd,"extractmin")) {
            bool ok=false; int v = bh_extract_min(&H, &ok);
            if (ok) printf("extracted %d\n", v); else puts("(empty)");
        } else if (!strcmp(cmd,"decrease")) {
            int oldk,newk; if (scanf("%d %d",&oldk,&newk)!=2) continue;
            BHNode *x = bh_search(H->head, oldk);
            if (!x) puts("key not found");
            else    bh_decrease_key(H, x, newk);
        } else if (!strcmp(cmd,"delete")) {
            int k; if (scanf("%d",&k)!=1) continue;
            BHNode *x = bh_search(H->head, k);
            if (!x) puts("key not found");
            else    bh_delete(&H, x);
        } else if (!strcmp(cmd,"union")) {
            char path[256]; if (scanf("%255s", path) != 1) continue;
            BinomialHeap *B = bh_make();
            load_file_into_heap(&B, path);
            H = bh_union(H, B);
        } else if (!strcmp(cmd,"random")) {
            int n,maxv; if (scanf("%d %d",&n,&maxv)!=2) continue;
            for (int i=0;i<n;i++) bh_insert(&H, rand()%(maxv+1));
        } else if (!strcmp(cmd,"print")) {
            bh_print(H);
        } else if (!strcmp(cmd,"quit")) {
            break;
        } else {
            puts("unknown command; type 'help'");
        }
    }

    bh_free(H);
    return 0;
}

