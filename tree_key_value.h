// vim: filetype=xs

const long long KV(secret) = 5673533590968723872LL + I(KEY) * 64 + I(VALUE);


typedef union KV(tree_t) {
    struct {
        union KV(tree_t) *left, *right;
        IV size;
        T(KEY) key;
#if I(VALUE) != I(void)
        T(VALUE) value;
#endif
    };
    union KV(tree_t) * free_slot;
} KV(tree_t);

typedef struct KV(tree_seg_t) {
    struct KV(tree_seg_t) * prev_seg;

    KV(tree_t) cell[SEG_SIZE];
} KV(tree_seg_t);

typedef struct KV(tree_cntr_t) {
    long long secret;
    SV* cmp;
    KV(tree_t) * root; // (init 後, empty 前) 永不為空, 一開始指向 nil
    KV(tree_t) * free_slot;
    KV(tree_seg_t) * newest_seg;
} KV(tree_cntr_t);

static inline KV(tree_cntr_t) * KV(assure_tree_cntr)(SV * obj){
    if( !obj )
        croak("assure_tree_cntr: NULL ptr");

    if( !SvROK(obj) )
        croak("assure_tree_cntr: try to dereference a non-reference");

    SV * ret = SvRV(obj);
    if( !ret )
        croak("assure_tree_cntr: deref to NULL");

    if( !SvROK(ret) )
        croak("assure_tree_cntr: deref to non-reference");

    KV(tree_cntr_t) * cntr = (KV(tree_cntr_t)*) SvRV(ret);
    if( !cntr )
        croak("assure_tree_cntr: NULL cntr");

    if( cntr->secret != KV(secret) )
        croak("assure_tree_cntr: unmatched secret %lld against %lld", cntr->secret, KV(secret));

    return cntr;
}

// 把所有的 cell 以 free_slot 串起來, 最後一個指向 NULL
// return 開頭的 cell
static inline KV(tree_t) * KV(init_tree_seg)(KV(tree_seg_t) * seg, KV(tree_seg_t) * prev){
    seg->prev_seg = prev;
    seg->cell[SEG_SIZE-1].free_slot = NULL;
    for(int i=SEG_SIZE-2; i>=0; --i)
        seg->cell[i].free_slot = &seg->cell[i+1];
    return &seg->cell[0];
}

#ifndef MAINTAINER
#define MAINTAINER
KV(tree_t) nil = { .size = 0, .left = &nil, .right = &nil };

// 假設 t->right 存在, return 新的 subtree root
static inline KV(tree_t)* rotate_left(KV(tree_t)* t){
    KV(tree_t) * c = t->right;
    t->right = c->left;
    c->left = t;
    c->size = t->size;
    t->size = t->left->size + t->right->size + 1;
    return c;
}

// 假設 t->left 存在, return 新的 subtree root
static inline KV(tree_t)* rotate_right(KV(tree_t) * t){
    KV(tree_t) * c = t->left;
    t->left = c->right;
    c->right = t;
    c->size = t->size;
    t->size = t->left->size + t->right->size + 1;
    return c;
}

KV(tree_t) * maintain_larger_left(void * t);
KV(tree_t) * maintain_larger_right(void * t);

KV(tree_t) * maintain_larger_left(void * _t){
    KV(tree_t) * t = (KV(tree_t)*) _t;
    if( t->left->left->size > t->right->size )
        t = rotate_right(t);
    else if( t->left->right->size > t->right->size ){
        t->left = rotate_left(t->left);
        t = rotate_right(t);
    }
    else
        return t;

    t->left = maintain_larger_left(t->left);
    t->right = maintain_larger_right(t->right);
    t = maintain_larger_left(t);
    t = maintain_larger_right(t);
    return t;
}

KV(tree_t) * maintain_larger_right(void * _t){
    KV(tree_t) * t = (KV(tree_t)*) _t;
    if( t->right->right->size > t->left->size )
        t = rotate_left(t);
    else if( t->right->left->size > t->left->size ){
        t->right = rotate_right(t->right);
        t = rotate_left(t);
    }
    else
        return t;

    t->left = maintain_larger_left(t->left);
    t->right = maintain_larger_right(t->right);
    t = maintain_larger_left(t);
    t = maintain_larger_right(t);
    return t;
}

// 假設 tree 不是空的
bool tree_check_subtree_size(KV(tree_t) * tree){
    if( tree->size != tree->left->size + tree->right->size + 1 )
        return FALSE;
    if( tree->left != (KV(tree_t)*) &nil && !tree_check_subtree_size(tree->left) )
        return FALSE;
    if( tree->right != (KV(tree_t)*) &nil && !tree_check_subtree_size(tree->right) )
        return FALSE;
    return TRUE;
}
static inline bool tree_check_size(void * _cntr){
    KV(tree_cntr_t)* cntr = (KV(tree_cntr_t)*) _cntr;
    if( cntr->root == (KV(tree_t)*) &nil )
        return TRUE;
    return tree_check_subtree_size(cntr->root);
}

// 假設 tree 不是空的
bool tree_check_subtree_balance(KV(tree_t) * tree){
    if( tree->left->left->size > tree->right->size )
        return FALSE;
    if( tree->left->right->size > tree->right->size )
        return FALSE;
    if( tree->right->left->size > tree->left->size )
        return FALSE;
    if( tree->right->right->size > tree->left->size )
        return FALSE;
    if( tree->left != &nil && !tree_check_subtree_balance(tree->left) )
        return FALSE;
    if( tree->right != &nil && !tree_check_subtree_balance(tree->right) )
        return FALSE;
    return TRUE;
}
static inline bool tree_check_balance(void * _cntr){
    KV(tree_cntr_t)* cntr = (KV(tree_cntr_t)*) _cntr;
    if( cntr->root == (KV(tree_t)*) &nil )
        return TRUE;
    return tree_check_subtree_balance(cntr->root);
}

#endif // MAINTAINER

static inline KV(tree_t) * KV(allocate_cell)(KV(tree_cntr_t) * cntr, T(KEY) key, T(VALUE) value){
    if( UNLIKELY(!cntr->free_slot) ){
        KV(tree_seg_t) * new_seg;
        Newx(new_seg, 1, KV(tree_seg_t));
        cntr->free_slot = KV(init_tree_seg)(new_seg, cntr->newest_seg);
        cntr->newest_seg = new_seg;
    }
    KV(tree_t) * new_cell = cntr->free_slot;
    cntr->free_slot = new_cell->free_slot;

    new_cell->left = new_cell->right = (KV(tree_t)*) &nil;
    new_cell->size = 1;
    new_cell->key = key;
#if I(VALUE) != I(void)
    new_cell->value = value;
#endif

    return new_cell;
}

static inline void KV(free_cell)(KV(tree_cntr_t) * cntr, KV(tree_t) * cell){
    cell->free_slot = cntr->free_slot;
    cntr->free_slot = cell;
}

static inline void KV(empty_tree_cntr)(pTHX_ KV(tree_cntr_t) * cntr){
#if I(KEY) == I(str) || I(KEY) == I(any) || I(VALUE) == I(str) || I(VALUE) == I(any)
    KV(tree_t) * free_slot = cntr->free_slot;
    while( free_slot ){
        KV(tree_t) * next_free_slot = free_slot->free_slot;
#   if I(KEY) == I(str) || I(KEY) == I(any)
        free_slot->key = NULL;
#   endif
#   if I(VALUE) == I(str) || I(VALUE) == I(any)
        free_slot->value = NULL;
#   endif
        free_slot = next_free_slot;
    }
#endif
    KV(tree_seg_t) * seg = cntr->newest_seg;
    while( seg ){
        KV(tree_seg_t) * prev = seg->prev_seg;
#if I(KEY) == I(str) || I(KEY) == I(any) || I(VALUE) == I(str) || I(VALUE) == I(any)
        for(int i=SEG_SIZE-1; i>=0; --i){
#   if I(KEY) == I(str) || I(KEY) == I(any)
            SvREFCNT_dec(seg->cell[i].key);
#   endif
#   if I(VALUE) == I(str) || I(VALUE) == I(any)
            SvREFCNT_dec(seg->cell[i].value);
#   endif
        }
#endif
        Safefree(seg);
        seg = prev;
    }
    cntr->root = (KV(tree_t)*) &nil;
    cntr->free_slot = NULL;
    cntr->newest_seg = NULL;
}

// 假設一給定的 tree 不是空的
// 把子樹的最右節點拉上來成為子樹的 root, return 新的 root
// 新子樹沒有右子樹, 而左子樹符合 SBTree 特性
static inline KV(tree_t) * KV(tree_raise_max_cell)(KV(tree_t) * tree){
    KV(tree_t) * root = tree;

    int step_count = 0;

    KV(tree_t) * parent = (KV(tree_t)*) &nil;
    while( UNLIKELY(tree->right != (KV(tree_t)*) &nil) ){
        --tree->size;
        parent = tree;
        tree = tree->right;
        ++step_count;
    }

    if( LIKELY(parent != (KV(tree_t)*) &nil) ){
        parent->right = tree->left;
        tree->left = root;
        tree->size = root->size + 1;

        KV(tree_t) * stack[step_count];
        for(int i=0; i<step_count; ++i){
            stack[i] = root;
            root = root->right;
        }
        for(int i=step_count-1; i>0; --i)
            stack[i-1]->right = (KV(tree_t)*) maintain_larger_left(stack[i]);
        tree->left = (KV(tree_t)*) maintain_larger_left(stack[0]);
    }

    return tree;
}

// 假設 parent 的 right 是 nil
// 把 right 接到 parent 的 right
// re-return parent;
static inline KV(tree_t) * KV(tree_assign_right)(KV(tree_t) * parent, KV(tree_t) * right){
    parent->right = right;
    parent->size += right->size;
    return parent;
}

// 假設 tree 不是空的
// 如果有左子樹, 把此 tree 的 root 用左子樹裡最大節點取代
// 否則用右字樹取代
// return 新的 root
static inline KV(tree_t) * KV(tree_replace_cell)(KV(tree_t) * tree){
    if( UNLIKELY(tree->left == (KV(tree_t)*) &nil) )
        return tree->right;
    return KV(tree_assign_right)(KV(tree_raise_max_cell)(tree->left), tree->right);
}

// 假設 tree 不是空的
// 刪除 tree 的 root
// return 新的 root
static inline KV(tree_t) * KV(tree_delete_root)(KV(tree_cntr_t) * cntr, KV(tree_t) * tree){
    KV(tree_t) * new_root = KV(tree_replace_cell)(tree);
    KV(free_cell)(cntr, tree);
    return new_root;
}

static inline int KV(tree_size)(KV(tree_cntr_t) * cntr){
    return cntr->root->size;
}

// 假設 tree 不是空的
static inline T(KEY) KV(tree_find_min)(KV(tree_cntr_t) * cntr, T(VALUE) * value_result){
    KV(tree_t) * t = cntr->root;
    while( t->left != (KV(tree_t)*) &nil )
        t = t->left;
#if I(VALUE) != I(void)
    *value_result = t->value;;
#endif
    return t->key;
}

// 假設 tree 不是空的
static inline T(KEY) KV(tree_find_max)(KV(tree_cntr_t) * cntr, T(VALUE) * value_result){
    KV(tree_t) * t = cntr->root;
    while( t->right != (KV(tree_t)*) &nil )
        t = t->right;
#if I(VALUE) != I(void)
    *value_result = t->value;;
#endif
    return t->key;
}

// 假設 0 <= offset < root->size
static inline T(KEY) KV(tree_skip_l)(KV(tree_cntr_t) * cntr, int offset, T(VALUE) * value_result){
    KV(tree_t) * t = cntr->root;
    while(TRUE){
        if( offset == t->left->size ){
#if I(VALUE) != I(void)
            *value_result = t->value;;
#endif
            return t->key;
        }
        if( offset < t->left->size )
            t = t->left;
        else{
            offset -= t->left->size + 1;
            t = t->right;
        }
    }
}

// 假設 0 <= offset < root->size
static inline T(KEY) KV(tree_skip_g)(KV(tree_cntr_t) * cntr, int offset, T(VALUE) * value_result){
    KV(tree_t) * t = cntr->root;
    while(TRUE){
        if( offset == t->right->size ){
#if I(VALUE) != I(void)
            *value_result = t->value;;
#endif
            return t->key;
        }
        if( offset < t->right->size )
            t = t->right;
        else{
            offset -= t->right->size + 1;
            t = t->left;
        }
    }
}

static inline void KV(init_tree_cntr)(KV(tree_cntr_t) * cntr, SV * cmp){
    cntr->secret = KV(secret);
    cntr->root = (KV(tree_t)*) &nil;
    cntr->newest_seg = NULL;
    cntr->free_slot = NULL;
#if I(KEY) == I(any)
    cntr->cmp = SvREFCNT_inc_simple_NN(cmp);
#endif
}

KV(tree_t) * KV(tree_insert_subtree)(pTHX_ KV(tree_cntr_t) * cntr, KV(tree_t) * p, T(KEY) key, KV(tree_t) * new_tree, T(VALUE) value){
    ++p->size;
    if( K(cmp)(aTHX_ p->key, key, cntr->cmp) <= 0 ){
        if( p->right == (KV(tree_t)*) &nil )
            p->right = new_tree;
        else{
            p->right = KV(tree_insert_subtree)(aTHX_ cntr, p->right, key, new_tree, value);
            p = (KV(tree_t)*) maintain_larger_right(p);
        }
    }
    else{
        if( p->left == (KV(tree_t)*) &nil )
            p->left = new_tree;
        else{
            p->left = KV(tree_insert_subtree)(aTHX_ cntr, p->left, key, new_tree, value);
            p = (KV(tree_t)*) maintain_larger_left(p);
        }
    }
    return p;
}
static inline void KV(tree_insert)(pTHX_ KV(tree_cntr_t) * cntr, T(KEY) key, T(VALUE) value){
    KV(tree_t) * new_tree = KV(allocate_cell)(cntr, key, value);

    if( UNLIKELY(cntr->root == (KV(tree_t)*) &nil) ){
        cntr->root = new_tree;
        return;
    }

    cntr->root = KV(tree_insert_subtree)(aTHX_ cntr, cntr->root, key, new_tree, value);
}

KV(tree_t) * KV(tree_delete_subtree)(pTHX_ KV(tree_cntr_t) * cntr, KV(tree_t) * tree, T(KEY) key){
    KV(tree_t) * c;
    if( K(cmp)(aTHX_ tree->key, key, cntr->cmp) <= 0 ){
        if( tree->right == (KV(tree_t)*) &nil )
            return NULL;

        if( K(cmp)(aTHX_ tree->right->key, key, cntr->cmp) == 0 ){
            tree->right = (KV(tree_t)*) maintain_larger_right(KV(tree_delete_root)(cntr, tree->right));
            --tree->size;
            tree = (KV(tree_t)*) maintain_larger_left(tree);
            return tree;
        }

        c = KV(tree_delete_subtree)(aTHX_ cntr, tree->right, key);
        if( c ){
            tree->right = c;
            --tree->size;
            tree = (KV(tree_t)*) maintain_larger_left(tree);
            return tree;
        }

        return NULL;
    }
    else{
        if( tree->left == (KV(tree_t)*) &nil )
            return NULL;

        if( K(cmp)(aTHX_ tree->left->key, key, cntr->cmp) == 0 ){
            tree->left = (KV(tree_t)*) maintain_larger_right(KV(tree_delete_root)(cntr, tree->left));
            --tree->size;
            tree = (KV(tree_t)*) maintain_larger_right(tree);
            return tree;
        }

        c = KV(tree_delete_subtree)(aTHX_ cntr, tree->left, key);
        if( c ){
            tree->left = c;
            --tree->size;
            tree = (KV(tree_t)*) maintain_larger_right(tree);
            return tree;
        }

        return NULL;
    }
}

static inline bool KV(tree_delete)(pTHX_ KV(tree_cntr_t) * cntr, T(KEY) key){
    if( UNLIKELY(cntr->root == (KV(tree_t)*) &nil) )
        return FALSE;

    if( K(cmp)(aTHX_ cntr->root->key, key, cntr->cmp)==0 ){
        cntr->root = (KV(tree_t)*) maintain_larger_right(KV(tree_delete_root)(cntr, cntr->root));
        return TRUE;
    }

    KV(tree_t) * new_root = KV(tree_delete_subtree)(aTHX_ cntr, cntr->root, key);
    if( new_root ){
        cntr->root = new_root;
        return TRUE;
    }
    return FALSE;
}

static inline bool KV(tree_find)(pTHX_ KV(tree_cntr_t) * cntr, T(KEY) key, T(VALUE) * value_result){
    KV(tree_t) * t = cntr->root;
    while( t != (KV(tree_t)*) &nil ){
        if( K(cmp)(aTHX_ t->key, key, cntr->cmp) == 0 ){
#if I(VALUE) != I(void)
            *value_result = t->value;
#endif
            return TRUE;
        }
        if( K(cmp)(aTHX_ t->key, key, cntr->cmp) <= 0 )
            t = t->right;
        else
            t = t->left;
    }
    return FALSE;
}

#define FUZZY_FIND_FUNC tree_find_lt
#define FUZZY_COUNT_FUNC tree_count_lt
#define FUZZY_FIND_CMP_RESULT < 0
#define FUZZY_FIND_GOOD_DIR right
#define FUZZY_FIND_BAD_DIR left
#include "fuzzy_find_gen.h"
#undef FUZZY_FIND_BAD_DIR
#undef FUZZY_FIND_GOOD_DIR
#undef FUZZY_FIND_CMP_RESULT
#undef FUZZY_COUNT_FUNC
#undef FUZZY_FIND_FUNC

#define FUZZY_FIND_FUNC tree_find_le
#define FUZZY_COUNT_FUNC tree_count_le
#define FUZZY_FIND_CMP_RESULT <= 0
#define FUZZY_FIND_GOOD_DIR right
#define FUZZY_FIND_BAD_DIR left
#include "fuzzy_find_gen.h"
#undef FUZZY_FIND_BAD_DIR
#undef FUZZY_FIND_GOOD_DIR
#undef FUZZY_FIND_CMP_RESULT
#undef FUZZY_COUNT_FUNC
#undef FUZZY_FIND_FUNC

#define FUZZY_FIND_FUNC tree_find_gt
#define FUZZY_COUNT_FUNC tree_count_gt
#define FUZZY_FIND_CMP_RESULT > 0
#define FUZZY_FIND_GOOD_DIR left
#define FUZZY_FIND_BAD_DIR right
#include "fuzzy_find_gen.h"
#undef FUZZY_FIND_BAD_DIR
#undef FUZZY_FIND_GOOD_DIR
#undef FUZZY_FIND_CMP_RESULT
#undef FUZZY_COUNT_FUNC
#undef FUZZY_FIND_FUNC

#define FUZZY_FIND_FUNC tree_find_ge
#define FUZZY_COUNT_FUNC tree_count_ge
#define FUZZY_FIND_CMP_RESULT >= 0
#define FUZZY_FIND_GOOD_DIR left
#define FUZZY_FIND_BAD_DIR right
#include "fuzzy_find_gen.h"
#undef FUZZY_FIND_BAD_DIR
#undef FUZZY_FIND_GOOD_DIR
#undef FUZZY_FIND_CMP_RESULT
#undef FUZZY_COUNT_FUNC
#undef FUZZY_FIND_FUNC

void KV(tree_dump_subtree)(pTHX_ int indent, KV(tree_t) * tree){
    if( tree->right != (KV(tree_t)*) &nil )
        KV(tree_dump_subtree)(aTHX_ indent+1, tree->right);
    for(int i=0; i<indent; ++i)
        PerlIO_printf(PerlIO_stdout(), "  ");

#if I(KEY) == I(int)
    T(KEY) key = tree->key;
#   define KEY_FMT "%d"
#   define KEY_FMT_TYPE (int)
#elif I(KEY) == I(num)
    T(KEY) key = tree->key;
#   define KEY_FMT "%lf"
#   define KEY_FMT_TYPE (double)
#else
    char * key = SvPV_nolen(tree->key);
#   define KEY_FMT "%s"
#   define KEY_FMT_TYPE (char*)
#endif
    PerlIO_printf(PerlIO_stdout(), "(" KEY_FMT ", %d)\n", KEY_FMT_TYPE key, (int) tree->size);
#undef KEY_FMT_TYPE
#undef KEY_FMT

    if( tree->left != (KV(tree_t)*) &nil )
        KV(tree_dump_subtree)(aTHX_ indent+1, tree->left);
}
static inline void KV(tree_dump)(pTHX_ KV(tree_cntr_t) * cntr){
    if( cntr->root == (KV(tree_t)*) &nil ){
        puts("(empty tree)");
        return;
    }
    KV(tree_dump_subtree)(aTHX_ 0, cntr->root);
}

// 假設 tree 不是空的
bool KV(tree_check_subtree_order)(pTHX_ KV(tree_cntr_t) * cntr, KV(tree_t) * tree){
    if( tree->left != (KV(tree_t)*) &nil && (K(cmp)(aTHX_ tree->left->key, tree->key, cntr->cmp) > 0 || !KV(tree_check_subtree_order)(aTHX_ cntr, tree->left)) )
        return FALSE;
    if( tree->right != (KV(tree_t)*) &nil && (K(cmp)(aTHX_ tree->key, tree->right->key, cntr->cmp) > 0 || !KV(tree_check_subtree_order)(aTHX_ cntr, tree->right)) )
        return FALSE;
    return TRUE;
}
static inline bool KV(tree_check_order)(pTHX_ KV(tree_cntr_t) * cntr){
    if( cntr->root == (KV(tree_t)*) &nil )
        return TRUE;
    return KV(tree_check_subtree_order)(aTHX_ cntr, cntr->root);
}
