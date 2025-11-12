#include "binomial_heap.h"
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>


static BHNode* node_new(int key) {
    BHNode *x = (BHNode*)calloc(1, sizeof(BHNode));
    if (!x) { perror("calloc"); exit(1); }
    x->key = key;
    x->degree = 0;
    return x;
}

BinomialHeap* bh_make(void) {
    BinomialHeap *H = (BinomialHeap*)calloc(1, sizeof(BinomialHeap));
    if (!H) { perror("calloc"); exit(1); }
    H->head = NULL;
    return H;
}

void bh_free_node(BHNode *x) {
    if (!x) return;
    BHNode *c = x->child;
    while (c) {
        BHNode *next = c->sibling;
        bh_free_node(c);
        c = next;
    }
    free(x);
}

void bh_free(BinomialHeap *H) {
    if (!H) return;
    BHNode *r = H->head;
    while (r) {
        BHNode *next = r->sibling;
        bh_free_node(r);
        r = next;
    }
    free(H);
}

static void binomial_link(BHNode *y, BHNode *x) {
    y->parent  = x;
    y->sibling = x->child;
    x->child   = y;
    x->degree++;
}

static BHNode* merge_root_lists(BHNode *a, BHNode *b) {
    if (!a) return b;
    if (!b) return a;
    BHNode *head = NULL, **tail = &head;
    while (a && b) {
        if (a->degree <= b->degree) {
            *tail = a; tail = &a->sibling; a = a->sibling;
        } else {
            *tail = b; tail = &b->sibling; b = b->sibling;
        }
    }
    *tail = a ? a : b;
    return head;
}

BinomialHeap* bh_union(BinomialHeap *H1, BinomialHeap *H2) {
    BinomialHeap *H = bh_make();
    H->head = merge_root_lists(H1 ? H1->head : NULL, H2 ? H2->head : NULL);

    if (H1) free(H1);
    if (H2) free(H2);

    if (!H->head) return H;

    BHNode *prev = NULL;
    BHNode *x    = H->head;
    BHNode *next = x->sibling;

    while (next) {
        if (x->degree != next->degree ||
            (next->sibling && next->sibling->degree == x->degree)) {
            prev = x;
            x = next;
        } else {            
            if (x->key <= next->key) {
                x->sibling = next->sibling;
                binomial_link(next, x);
            } else {
                if (prev) prev->sibling = next;
                else       H->head      = next;
                binomial_link(x, next);
                x = next;
            }
        }
        next = x->sibling;
    }
    return H;
}

void bh_insert(BinomialHeap **H, int key) {
    BinomialHeap *H2 = bh_make();
    H2->head = node_new(key);
    *H = bh_union(*H, H2);
}

BHNode* bh_minimum(BinomialHeap *H) {
    if (!H || !H->head) return NULL;
    BHNode *y = NULL, *x = H->head;
    int min = INT_MAX;
    while (x) {
        if (x->key < min) { min = x->key; y = x; }
        x = x->sibling;
    }
    return y;
}

int bh_extract_min(BinomialHeap **Hp, bool *ok) {
    BinomialHeap *H = *Hp;
    if (!H || !H->head) { if (ok) *ok=false; return 0; }

    BHNode *prev_min = NULL, *min_node = H->head, *prev = NULL, *cur = H->head;
    int min = cur->key;
    while (cur) {
        if (cur->key < min) { min = cur->key; prev_min = prev; min_node = cur; }
        prev = cur; cur = cur->sibling;
    }

    if (prev_min) prev_min->sibling = min_node->sibling;
    else H->head = min_node->sibling;

    BHNode *child = min_node->child;
    BHNode *rev = NULL;
    while (child) {
        BHNode *next = child->sibling;
        child->parent = NULL;
        child->sibling = rev;
        rev = child;
        child = next;
    }
    BinomialHeap *H2 = bh_make();
    H2->head = rev;

    free(min_node);

    *Hp = bh_union(H, H2);
    if (ok) *ok = true;
    return min;
}

void bh_decrease_key(BinomialHeap *H, BHNode *x, int new_key) {
    if (!x || !H) return;
    if (new_key > x->key) return; 
    x->key = new_key;
    BHNode *y = x;
    BHNode *z = y->parent;
    while (z && y->key < z->key) {
        int tmp = y->key; y->key = z->key; z->key = tmp;
        y = z; z = y->parent;
    }
}

void bh_delete(BinomialHeap **Hp, BHNode *x) {
    if (!x) return;
    bh_decrease_key(*Hp, x, INT_MIN);
    bool ok=false;
    (void)bh_extract_min(Hp, &ok);
}

BHNode* bh_search(BHNode *root, int key) {
    for (BHNode *r = root; r; r = r->sibling) {
        if (r->key == key) return r;
        BHNode *hit = bh_search(r->child, key);
        if (hit) return hit;
    }
    return NULL;
}

static void print_tree(BHNode *x, int indent) {
    if (!x) return;
    for (int i=0;i<indent;i++) putchar(' ');
    printf("(%d,d=%d)\n", x->key, x->degree);
    print_tree(x->child, indent+2);
    print_tree(x->sibling, indent);
}

void bh_print(BinomialHeap *H) {
    printf("== Binomial Heap ==\n");
    for (BHNode *r = H->head; r; r = r->sibling) {
        printf("[root d=%d] ", r->degree);
        print_tree(r, 0);
    }
    printf("========\n");
}

