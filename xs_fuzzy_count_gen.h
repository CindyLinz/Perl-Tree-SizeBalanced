// vim: filetype=xs

SV ** KV(XS_FUZZY_COUNT_FUNC)(pTHX_ SV** SP, SV * obj, SV * key){
    dXSTARG;
    KV(tree_cntr_t) * cntr = KV(assure_tree_cntr)(obj);
    PUSHu(KV(FUZZY_COUNT_FUNC)(cntr, K(from_sv)(key)));
    return SP;
}

