#include "tree.h"

tree_t nil = { .size = 0, .left = &nil, .right = &nil };

void maintain_larger_left(tree_t * t){
    if( t->left->left->size > t->right->size )
        rotate_right(t);
    else if( t->left->right->size > t->right->size ){
        rotate_left(t->left);
        rotate_right(t);
    }
    else
        return;

    maintain_larger_left(t->left);
    maintain_larger_right(t->right);
    maintain_larger_left(t);
    maintain_larger_right(t);
}

void maintain_larger_right(tree_t * t){
    if( t->right->right->size > t->left->size )
        rotate_left(t);
    else if( t->right->left->size > t->left->size ){
        rotate_right(t->right);
        rotate_left(t);
    }
    else
        return;

    maintain_larger_left(t->left);
    maintain_larger_right(t->right);
    maintain_larger_left(t);
    maintain_larger_right(t);
}

bool tree_delete_cell(tree_cntr_t * cntr, tree_t * tree, IV key){
    if( key >= tree->key ){
        if( tree->right == &nil )
            return FALSE;

        if( tree->right->key == key ){
            tree->right = tree_delete_root(cntr, tree->right);
            maintain_larger_right(tree->right);
            --tree->size;
            maintain_larger_left(tree);
            return TRUE;
        }

        if( tree_delete_subtree(cntr, tree->right, key) ){
            --tree->size;
            maintain_larger_left(tree);
            return TRUE;
        }

        return FALSE;
    }
    else{
        if( tree->left == &nil )
            return FALSE;

        if( tree->left->key == key ){
            tree->left = tree_delete_root(cntr, tree->left);
            maintain_larger_right(tree->left);
            --tree->size;
            maintain_larger_right(tree);
            return TRUE;
        }

        if( tree_delete_subtree(cntr, tree->left, key) ){
            --tree->size;
            maintain_larger_right(tree);
            return TRUE;
        }

        return FALSE;
    }
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
