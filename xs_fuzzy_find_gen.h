// vim: filetype=xs

SV ** KV(XS_FUZZY_FIND_FUNC)(pTHX_ SV** SP, SV * obj, SV * key){
    KV(tree_cntr_t) * cntr = KV(assure_tree_cntr)(obj);

    save_scalar(a_GV);
    save_scalar(b_GV);
    SvREFCNT_inc_simple_void_NN(key);

    T(VALUE) value_result;
    T(KEY) found = KV(FUZZY_FIND_FUNC)(cntr, K(from_sv)(key), &value_result);
    if( found == K(unknown)() )
        PUSHs(&PL_sv_undef);
    else{
        SP = K(ret)(SP, found);
#if I(VALUE) != I(void)
        SP = V(mret)(SP, value_result);
#endif
    }

    SvREFCNT_dec_NN(key);
    return SP;
}
