#ifndef BINOMIAL_HEAP_H
#define BINOMIAL_HEAP_H

#include <stdbool.h>

typedef struct BHNode {
    int key;
    int degree;
    struct BHNode *parent;
    struct BHNode *child;
    struct BHNode *sibling;
} BHNode;

typedef struct {
    BHNode *head;   
} BinomialHeap;

BinomialHeap* bh_make(void);
void          bh_free(BinomialHeap *H);
void          bh_free_node(BHNode *x);

void          bh_insert(BinomialHeap **H, int key);
BHNode*       bh_minimum(BinomialHeap *H);
int           bh_extract_min(BinomialHeap **H, bool *ok);
void          bh_decrease_key(BinomialHeap *H, BHNode *x, int new_key);
void          bh_delete(BinomialHeap **H, BHNode *x);

BinomialHeap* bh_union(BinomialHeap *H1, BinomialHeap *H2); 
BHNode*       bh_search(BHNode *root, int key);             
void          bh_print(BinomialHeap *H);                    

#endif

