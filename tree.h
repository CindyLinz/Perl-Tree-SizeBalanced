#ifndef TREE__H
#define TREE__H

#include "perl.h"

typedef struct tree_t {
    unsigned int capacity, size;
    int data[0];
    /* (int) (key, left, right) * capacity
     * (int) size * (capacity + 1)  (index in [-1 .. capacity-1], index=-1(nil) 處存 0)
     * (int) value * capacity
     */
} tree_t;

static inline int tree_memory_capacity_size(tree_t * tree, int capacity){
    return sizeof(int) * (2 + capacity * 5 + 1);
}
static inline int tree_memory_size(tree_t * tree){
    return tree_memory_capacity_size(tree, tree->capacity);
}

static inline int * key_capacity_p(tree_t * tree, int capacity, int i){
    return tree->data + i*3;
}
static inline int * left_capacity_p(tree_t * tree, int capacity, int i){
    return tree->data + i*3+1;
}
static inline int * right_capacity_p(tree_t * tree, int capacity, int i){
    return tree->data + i*3+2;
}
static inline int * size_capacity_p(tree_t * tree, int capacity, int i){
    return tree->data + 3*capacity + i + 1;
}
static inline int * value_capacity_p(tree_t * tree, int capacity, int i){
    return tree->data + 4*capacity + i + 1;
}

static inline int * key_p(tree_t * tree, int i){
    key_capacity_p(tree, tree->capacity, i);
}
static inline int * left_p(tree_t * tree, int i){
    left_capacity_p(tree, tree->capacity, i);
}
static inline int * right_p(tree_t * tree, int i){
    right_capacity_p(tree, tree->capacity, i);
}
static inline int * size_p(tree_t * tree, int i){
    size_capacity_p(tree, tree->capacity, i);
}
static inline int * value_p(tree_t * tree, int i){
    value_capacity_p(tree, tree->capacity, i);
}

static inline int key(tree_t * tree, int i){
    return *key_p(tree, i);
}
static inline int left(tree_t * tree, int i){
    return *left_p(tree, i);
}
static inline int right(tree_t * tree, int i){
    return *right_p(tree, i);
}
static inline int size(tree_t * tree, int i){
    return *size_p(tree, i);
}
static inline int value(tree_t * tree, int i){
    return *value_p(tree, i);
}

static inline void tree_compact(tree_t * tree){
    if( tree->size < tree->capacity ){
        Move(left_p(tree, 0), left_capacity_p(tree, tree->size, 0), tree->size, int);
        Move(right_p(tree, 0), right_capacity_p(tree, tree->size, 0), tree->size, int);
        Move(size_p(tree, -1), size_capacity_p(tree, tree->size, -1), tree->size, int);
        Move(value_p(tree, 0), value_capacity_p(tree, tree->size, 0), tree->size, int);
        tree->capacity = tree->size;
        Renewc(tree, tree_memory_size(tree), char, tree_t);
    }
}

static inline void tree_expand(tree_t * tree, int new_capacity){
    if( new_capacity > tree->capacity ){
        Renewc(tree, tree_memory_capacity_size(tree, new_capacity), char, tree_t);
        Move(value_p(tree, 0), value_capacity_p(tree, new_capacity, 0), tree->size, int);
        Move(size_p(tree, -1), size_capacity_p(tree, new_capacity, -1), tree->size, int);
        Move(right_p(tree, 0), right_capacity_p(tree, new_capacity, 0), tree->size, int);
        Move(left_p(tree, 0), left_capacity_p(tree, new_capacity, 0), tree->size, int);
        tree->capacity = new_capacity;
    }
}

// 假設 right[t] 存在, return 新的 subtree root
static inline int rotate_left(tree_t * tree, int t){
    int c = right(tree, t);
    *right_p(tree, t) = left(tree, c);
    *left_p(tree, c) = t;
    *size_p(tree, c) = size(tree, t);
    *size_p(tree, t) = size(tree, left(tree, t)) + size(tree, right(tree, t)) + 1;
    return c;
}

// 假設 left[t] 存在, return 新的 subtree root
static inline int rotate_right(tree_t * tree, int t){
    int c = left(tree, t);
    *left_p(tree, t) = right(tree, c);
    *right_p(tree, c) = t;
    *size_p(tree, c) = size(tree, t);
    *size_p(tree, t) = size(tree, left(tree, t)) + size(tree, right(tree, t)) + 1;
    return c;
}

#endif
