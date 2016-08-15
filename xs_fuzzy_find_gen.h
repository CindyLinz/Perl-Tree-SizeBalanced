// vim: filetype=xs

SV ** KV(XS_FUZZY_FIND_FUNC)(pTHX_ SV** SP, SV * obj, SV * key){
    KV(tree_cntr_t) * cntr = KV(assure_tree_cntr)(obj);

    save_scalar(a_GV);
    save_scalar(b_GV);
#if I(KEY) == I(any)
    SvREFCNT_inc_simple_void_NN(key);
#endif

    T(VALUE) value_result;
    T(KEY) found = KV(FUZZY_FIND_FUNC)(aTHX_ cntr, K(from_sv)(aTHX_ key), &value_result);
    if( found == K(unknown)(aTHX) )
        PUSHs(&PL_sv_undef);
    else{
        SP = K(ret)(aTHX_ SP, found);
#if I(VALUE) != I(void)
        if( GIMME_V == G_ARRAY )
            SP = V(mret)(aTHX_ SP, value_result);
#endif
    }

#if I(KEY) == I(any)
#   ifdef SvREFCNT_dec_NN
    SvREFCNT_dec_NN(key);
#   else
    SvREFCNT_dec(key);
#   endif
#endif
    return SP;
}
