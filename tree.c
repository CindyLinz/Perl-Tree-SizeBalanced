#include "tree.h"

tree_t nil = { .size = 0, .left = &nil, .right = &nil };

tree_t * maintain_larger_left(tree_t * t){
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

tree_t * maintain_larger_right(tree_t * t){
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

tree_t * tree_insert_subtree(tree_t * p, IV key, tree_t * new_tree){
    ++p->size;
    if( key >= p->key ){
        if( p->right == &nil )
            p->right = new_tree;
        else{
            p->right = tree_insert_subtree(p->right, key, new_tree);
            p = maintain_larger_right(p);
        }
    }
    else{
        if( p->left == &nil )
            p->left = new_tree;
        else{
            p->left = tree_insert_subtree(p->left, key, new_tree);
            p = maintain_larger_left(p);
        }
    }
    return p;
}

tree_t * tree_delete_subtree(tree_cntr_t * cntr, tree_t * tree, IV key){
    tree_t * c;
    if( key >= tree->key ){
        if( tree->right == &nil )
            return NULL;

        if( tree->right->key == key ){
            tree->right = maintain_larger_right(tree_delete_root(cntr, tree->right));
            --tree->size;
            tree = maintain_larger_left(tree);
            return tree;
        }

        c = tree_delete_subtree(cntr, tree->right, key);
        if( c ){
            tree->right = c;
            --tree->size;
            tree = maintain_larger_left(tree);
            return tree;
        }

        return NULL;
    }
    else{
        if( tree->left == &nil )
            return NULL;

        if( tree->left->key == key ){
            tree->left = maintain_larger_right(tree_delete_root(cntr, tree->left));
            --tree->size;
            tree = maintain_larger_right(tree);
            return tree;
        }

        c = tree_delete_subtree(cntr, tree->left, key);
        if( c ){
            tree->left = c;
            --tree->size;
            tree = maintain_larger_right(tree);
            return tree;
        }

        return NULL;
    }
}

void tree_dump_subtree(int indent, tree_t * tree){
    if( tree->right != &nil )
        tree_dump_subtree(indent+1, tree->right);
    for(int i=0; i<indent; ++i)
        PerlIO_printf(PerlIO_stdout(), "  ");
    PerlIO_printf(PerlIO_stdout(), "(%d, %d)\n", tree->key, tree->size);
    if( tree->left != &nil )
        tree_dump_subtree(indent+1, tree->left);
}

// 假設 tree 不是空的
bool tree_check_subtree_order(tree_t * tree){
    if( tree->left != &nil && (tree->left->key > tree->key || !tree_check_subtree_order(tree->left)) )
        return FALSE;
    if( tree->right != &nil && (tree->key > tree->right->key || !tree_check_subtree_order(tree->right)) )
        return FALSE;
    return TRUE;
}

// 假設 tree 不是空的
bool tree_check_subtree_size(tree_t * tree){
    if( tree->size != tree->left->size + tree->right->size + 1 )
        return FALSE;
    if( tree->left != &nil && !tree_check_subtree_size(tree->left) )
        return FALSE;
    if( tree->right != &nil && !tree_check_subtree_size(tree->right) )
        return FALSE;
    return TRUE;
}

// 假設 tree 不是空的
bool tree_check_subtree_balance(tree_t * tree){
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
