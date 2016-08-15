// vim: filetype=xs

static inline T(KEY) KV(FUZZY_FIND_FUNC)(pTHX_ KV(tree_cntr_t) * cntr, T(KEY) key, T(VALUE) * value_result){
    KV(tree_t) * t = cntr->root;
    T(KEY) best = K(unknown)(aTHX);
    while( t != (KV(tree_t)*) &nil ){
        if( K(cmp)(aTHX_ t->key, key, cntr->cmp) FUZZY_FIND_CMP_RESULT ){
            best = t->key;
#if I(VALUE) != I(void)
            *value_result = t->value;
#endif
            t = t->FUZZY_FIND_GOOD_DIR;
        }
        else
            t = t->FUZZY_FIND_BAD_DIR;
    }
    return best;
}

static inline UV KV(FUZZY_COUNT_FUNC)(pTHX_ KV(tree_cntr_t) * cntr, T(KEY) key){
    KV(tree_t) * t = cntr->root;
    int count = 0;
    while( t != (KV(tree_t)*) &nil ){
        if( K(cmp)(aTHX_ t->key, key, cntr->cmp) FUZZY_FIND_CMP_RESULT ){
            count += t->FUZZY_FIND_BAD_DIR->size + 1;
            t = t->FUZZY_FIND_GOOD_DIR;
        }
        else
            t = t->FUZZY_FIND_BAD_DIR;
    }
    return count;
}
