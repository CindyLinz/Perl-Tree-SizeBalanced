#ifndef TREE__H
#define TREE__H

#include "EXTERN.h"
#include "perl.h"

#define SEG_SIZE (512)

typedef union tree_t {
    struct {
        IV key;
        union tree_t *left, *right;
        IV size;
    };
    union tree_t * free_slot;
} tree_t;

typedef struct tree_seg_t {
    struct tree_seg_t * prev_seg;

    tree_t cell[SEG_SIZE];
} tree_seg_t;

extern tree_t nil;

typedef struct tree_cntr_t {
    tree_t * root; // (init 後, empty 前) 永不為空, 一開始指向 nil
    tree_t * free_slot;
    tree_seg_t * newest_seg;
} tree_cntr_t;

// 把所有的 cell 以 free_slot 串起來, 最後一個指向 NULL
// return 開頭的 cell
static inline tree_t * init_tree_seg(tree_seg_t * seg, tree_seg_t * prev){
    seg->prev_seg = prev;
    seg->cell[SEG_SIZE-1].free_slot = NULL;
    for(int i=SEG_SIZE-2; i>=0; --i)
        seg->cell[i].free_slot = &seg->cell[i+1];
    return &seg->cell[0];
}

static inline void init_tree_cntr(tree_cntr_t * cntr){
    cntr->root = &nil;
    cntr->newest_seg = NULL;
    cntr->free_slot = NULL;
}

static inline void empty_tree_cntr(tree_cntr_t * cntr){
    tree_seg_t * seg = cntr->newest_seg;
    while( seg ){
        tree_seg_t * prev = seg->prev_seg;
        Safefree(seg);
        seg = prev;
    }
    cntr->root = &nil;
    cntr->free_slot = NULL;
    cntr->newest_seg = NULL;
}

static inline tree_t * allocate_cell(tree_cntr_t * cntr, IV key){
    if( UNLIKELY(!cntr->free_slot) ){
        tree_seg_t * new_seg;
        Newx(new_seg, 1, tree_seg_t);
        cntr->free_slot = init_tree_seg(new_seg, cntr->newest_seg);
        cntr->newest_seg = new_seg;
    }
    tree_t * new_cell = cntr->free_slot;
    cntr->free_slot = new_cell->free_slot;

    new_cell->left = new_cell->right = &nil;
    new_cell->size = 1;
    new_cell->key = key;

    return new_cell;
}

static inline void free_cell(tree_cntr_t * cntr, tree_t * cell){
    cell->free_slot = cntr->free_slot;
    cntr->free_slot = cell;
}

// 假設 right[t] 存在, return 新的 subtree root
static inline tree_t* rotate_left(tree_t* t){
    tree_t * c = t->right;
    t->right = c->left;
    c->left = t;
    c->size = t->size;
    t->size = t->left->size + t->right->size + 1;
    return c;
}

// 假設 left[t] 存在, return 新的 subtree root
static inline tree_t* rotate_right(tree_t * t){
    tree_t * c = t->left;
    t->left = c->right;
    c->right = t;
    c->size = t->size;
    t->size = t->left->size + t->right->size + 1;
    return c;
}

void maintain_larger_left(tree_t * t);
void maintain_larger_right(tree_t * t);

static inline void tree_insert(tree_cntr_t * cntr, IV key){
    tree_t * new_tree = allocate_cell(cntr, key);

    if( UNLIKELY(cntr->root == &nil) ){
        cntr->root = new_tree;
        return;
    }

    tree_t * p = cntr->root;
    while(TRUE){
        ++p->size;
        if( key >= p->key ){
            if( p->right == &nil ){
                p->right = new_tree;
                break;
            }
            p = p->right;
        }
        else{
            if( p->left == &nil ){
                p->left = new_tree;
                break;
            }
            p = p->left;
        }
    }

    if( key >= cntr->root->key )
        maintain_larger_right(cntr->root);
    else
        maintain_larger_left(cntr->root);
}

bool tree_delete_subtree(tree_cntr_t * cntr, tree_t * tree, IV key);

// 假設一給定的 tree 不是空的
// 把子樹的最右節點拉上來成為子樹的 root, return 新的 root
static inline tree_t * tree_raise_max_cell(tree_t * tree){
    tree_t * root = tree;

    tree_t * parent = &nil;
    while( UNLIKELY(tree->right != &nil) ){
        --tree->size;
        parent = tree;
        tree = tree->right;
    }

    if( LIKELY(parent != &nil) ){
        parent->right = tree->left;
        tree->left = root;
        tree->size = root->size + 1;
    }

    return tree;
}

// 假設 parent 的 right 是 nil
// 把 right 接到 parent 的 right
// re-return parent;
static inline tree_t * tree_assign_right(tree_t * parent, tree_t * right){
    parent->right = right;
    parent->size += right->size;
    return parent;
}

// 假設 tree 不是空的
// 如果有左子樹, 把此 tree 的 root 用左子樹裡最大節點取代
// 否則用右字樹取代
// return 新的 root
static inline tree_t * tree_replace_cell(tree_t * tree){
    if( UNLIKELY(tree->left == &nil) )
        return tree->right;
    return tree_assign_right(tree_raise_max_cell(tree->left), tree->right);
}

// 假設 tree 不是空的
// 刪除 tree 的 root
// return 新的 root
static inline tree_t * tree_delete_root(tree_cntr_t * cntr, tree_t * tree){
    tree_t * new_root = tree_replace_cell(tree);
    free_cell(cntr, tree);
    return new_root;
}

static inline bool tree_delete(tree_cntr_t * cntr, IV key){
    if( UNLIKELY(cntr->root == &nil) )
        return FALSE;

    if( UNLIKELY(cntr->root->key == key) ){
        cntr->root = tree_delete_root(cntr, cntr->root);
        return TRUE;
    }

    return tree_delete_subtree(cntr, cntr->root, key);
}

static inline bool tree_find(tree_cntr_t * cntr, IV key){
    tree_t * t = cntr->root;
    while( t != &nil ){
        if( t->key == key )
            return TRUE;
        if( key >= t->key )
            t = t->right;
        else
            t = t->left;
    }
    return FALSE;
}

static inline IV tree_find_lt(tree_cntr_t * cntr, IV not_found_key, IV key){
    tree_t * t = cntr->root;
    IV best = not_found_key;
    while( t != &nil ){
        if( t->key < key ){
            best = t->key;
            t = t->right;
        }
        else
            t = t->left;
    }
    return best;
}

static inline IV tree_find_le(tree_cntr_t * cntr, IV not_found_key, IV key){
    tree_t * t = cntr->root;
    IV best = not_found_key;
    while( t != &nil ){
        if( t->key <= key ){
            best = t->key;
            t = t->right;
        }
        else
            t = t->left;
    }
    return best;
}

static inline IV tree_find_gt(tree_cntr_t * cntr, IV not_found_key, IV key){
    tree_t * t = cntr->root;
    IV best = not_found_key;
    while( t != &nil ){
        if( t->key > key ){
            best = t->key;
            t = t->left;
        }
        else
            t = t->right;
    }
    return best;
}

static inline IV tree_find_ge(tree_cntr_t * cntr, IV not_found_key, IV key){
    tree_t * t = cntr->root;
    IV best = not_found_key;
    while( t != &nil ){
        if( t->key >= key ){
            best = t->key;
            t = t->left;
        }
        else
            t = t->right;
    }
    return best;
}

static inline int tree_lt_count(tree_cntr_t * cntr, IV key){
    tree_t * t = cntr->root;
    int count = 0;
    while( t != &nil ){
        if( key > t->key ){
            count += t->left->size + 1;
            t = t->right;
        }
        else
            t = t->left;
    }
    return count;
}

static inline int tree_le_count(tree_cntr_t * cntr, IV key){
    tree_t * t = cntr->root;
    int count = 0;
    while( t != &nil ){
        if( key >= t->key ){
            count += t->left->size + 1;
            t = t->right;
        }
        else
            t = t->left;
    }
    return count;
}

static inline int tree_gt_count(tree_cntr_t * cntr, IV key){
    tree_t * t = cntr->root;
    int count = 0;
    while( t != &nil ){
        if( key < t->key ){
            count += t->right->size + 1;
            t = t->left;
        }
        else
            t = t->right;
    }
    return count;
}

static inline int tree_ge_count(tree_cntr_t * cntr, IV key){
    tree_t * t = cntr->root;
    int count = 0;
    while( t != &nil ){
        if( key <= t->key ){
            count += t->right->size + 1;
            t = t->left;
        }
        else
            t = t->right;
    }
    return count;
}

// 假設 0 <= offset < root->size
static inline IV tree_select_left(tree_cntr_t * cntr, int offset){
    tree_t * t = cntr->root;
    while(TRUE){
        if( offset == t->left->size )
            return t->key;
        if( offset < t->left->size )
            t = t->left;
        else{
            offset -= t->left->size + 1;
            t = t->right;
        }
    }
}

// 假設 0 <= offset < root->size
static inline IV tree_select_right(tree_cntr_t * cntr, int offset){
    tree_t * t = cntr->root;
    while(TRUE){
        if( offset == t->right->size )
            return t->key;
        if( offset < t->right->size )
            t = t->right;
        else{
            offset -= t->right->size + 1;
            t = t->left;
        }
    }
}

bool tree_check_subtree_size(tree_t * tree);
static inline bool tree_check_size(tree_cntr_t * cntr){
    if( cntr->root == &nil )
        return TRUE;
    return tree_check_subtree_size(cntr->root);
}

bool tree_check_subtree_balance(tree_t * tree);
static inline bool tree_check_balance(tree_cntr_t * cntr){
    if( cntr->root == &nil )
        return TRUE;
    return tree_check_subtree_balance(cntr->root);
}

bool tree_check_subtree_order(tree_t * tree);
static inline bool tree_check_order(tree_cntr_t * cntr){
    if( cntr->root == &nil )
        return TRUE;
    return tree_check_subtree_order(cntr->root);
}

#endif
