// vim: filetype=xs

SV ** KV(XS_FUZZY_COUNT_FUNC)(pTHX_ SV** SP, SV * obj, SV * key){
    dXSTARG;
    KV(tree_cntr_t) * cntr = KV(assure_tree_cntr)(obj);

    save_scalar(a_GV);
    save_scalar(b_GV);
#if I(KEY) == I(any)
    SvREFCNT_inc_simple_void_NN(key);
#endif

    PUSHu(KV(FUZZY_COUNT_FUNC)(cntr, K(from_sv)(key)));

#if I(KEY) == I(any)
    SvREFCNT_dec_NN(key);
#endif
    return SP;
}

