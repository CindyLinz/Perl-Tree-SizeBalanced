// vim: filetype=xs

SV ** KV(new)(pTHX_ SV ** SP, SV * class){
    KV(tree_cntr_t) * cntr;
    Newx(cntr, 1, KV(tree_cntr_t));
    cntr->secret = KV(secret);
    KV(init_tree_cntr)(cntr);

    SV * ret = newSV(0);
    SvUPGRADE(ret, SVt_RV);
    SvROK_on(ret);
    SvRV(ret) = (SV*) cntr;

    SV * obj = newRV_noinc(ret);
    STRLEN classname_len;
    char * classname = SvPVbyte(class, classname_len);
    HV * stash = gv_stashpvn(classname, classname_len, 0);
    sv_bless(obj, stash);
    PUSHs(sv_2mortal(obj));
    return SP;
}

SV ** KV(DESTROY)(pTHX_ SV ** SP, SV * obj){
    KV(tree_cntr_t) * cntr = KV(assure_tree_cntr)(obj);
    KV(empty_tree_cntr)(cntr);
    Safefree(cntr);
    SvRV(SvRV(obj)) = NULL;
    return SP;
}

SV ** KV(size)(pTHX_ SV** SP, SV *obj){
    dXSTARG;
    KV(tree_cntr_t) * cntr = KV(assure_tree_cntr)(obj);
    PUSHu((UV) KV(tree_size)(cntr));
    return SP;
}

SV ** KV(insert)(pTHX_ SV** SP, SV * obj, SV * key, SV * value){
    KV(tree_cntr_t) * cntr = KV(assure_tree_cntr)(obj);
    KV(tree_insert)(cntr, K(copy_sv)(key), V(copy_sv)(value));
    return SP;
}

SV ** KV(delete)(pTHX_ SV** SP, SV * obj, SV * key){
    KV(tree_cntr_t) * cntr = KV(assure_tree_cntr)(obj);
    if( KV(tree_delete)(cntr, K(from_sv)(key)) )
        PUSHs(&PL_sv_yes);
    else
        PUSHs(&PL_sv_no);
    return SP;
}

SV ** KV(find)(pTHX_ SV** SP, SV * obj, SV * key){
    KV(tree_cntr_t) * cntr = KV(assure_tree_cntr)(obj);
    T(VALUE) value_result;
    if( KV(tree_find)(cntr, K(from_sv)(key), &value_result) ){
        PUSHs(&PL_sv_yes);
#if I(VALUE) != I(void)
        SP = V(ret)(SP, value_result);
#endif
    }
    else
        PUSHs(&PL_sv_no);
    return SP;
}

#define XS_FUZZY_FIND_FUNC find_lt
#define FUZZY_FIND_FUNC tree_find_lt
#include "xs_fuzzy_find_gen.h"
#undef FUZZY_FIND_FUNC
#undef XS_FUZZY_FIND_FUNC

#define XS_FUZZY_FIND_FUNC find_le
#define FUZZY_FIND_FUNC tree_find_le
#include "xs_fuzzy_find_gen.h"
#undef FUZZY_FIND_FUNC
#undef XS_FUZZY_FIND_FUNC

#define XS_FUZZY_FIND_FUNC find_gt
#define FUZZY_FIND_FUNC tree_find_gt
#include "xs_fuzzy_find_gen.h"
#undef FUZZY_FIND_FUNC
#undef XS_FUZZY_FIND_FUNC

#define XS_FUZZY_FIND_FUNC find_ge
#define FUZZY_FIND_FUNC tree_find_ge
#include "xs_fuzzy_find_gen.h"
#undef FUZZY_FIND_FUNC
#undef XS_FUZZY_FIND_FUNC

#define XS_FUZZY_COUNT_FUNC count_lt
#define FUZZY_COUNT_FUNC tree_count_lt
#include "xs_fuzzy_count_gen.h"
#undef FUZZY_COUNT_FUNC
#undef XS_FUZZY_COUNT_FUNC

#define XS_FUZZY_COUNT_FUNC count_le
#define FUZZY_COUNT_FUNC tree_count_le
#include "xs_fuzzy_count_gen.h"
#undef FUZZY_COUNT_FUNC
#undef XS_FUZZY_COUNT_FUNC

#define XS_FUZZY_COUNT_FUNC count_gt
#define FUZZY_COUNT_FUNC tree_count_gt
#include "xs_fuzzy_count_gen.h"
#undef FUZZY_COUNT_FUNC
#undef XS_FUZZY_COUNT_FUNC

#define XS_FUZZY_COUNT_FUNC count_ge
#define FUZZY_COUNT_FUNC tree_count_ge
#include "xs_fuzzy_count_gen.h"
#undef FUZZY_COUNT_FUNC
#undef XS_FUZZY_COUNT_FUNC

SV ** KV(find_min)(pTHX_ SV** SP, SV * obj){
    KV(tree_cntr_t) * cntr = KV(assure_tree_cntr)(obj);
    if( KV(tree_size)(cntr) == 0 )
        PUSHs(&PL_sv_undef);
    else{
        T(VALUE) value_result;
        SP = K(ret)(SP, KV(tree_find_min)(cntr, &value_result));
#if I(VALUE) != I(void)
        SP = V(mret)(SP, value_result);
#endif
    }
    return SP;
}

SV ** KV(find_max)(pTHX_ SV** SP, SV * obj){
    KV(tree_cntr_t) * cntr = KV(assure_tree_cntr)(obj);
    if( KV(tree_size)(cntr) == 0 )
        PUSHs(&PL_sv_undef);
    else{
        T(VALUE) value_result;
        SP = K(ret)(SP, KV(tree_find_max)(cntr, &value_result));
#if I(VALUE) != I(void)
        SP = V(mret)(SP, value_result);
#endif
    }
    return SP;
}

SV ** KV(skip_l)(pTHX_ SV** SP, SV * obj, int offset){
    KV(tree_cntr_t) * cntr = KV(assure_tree_cntr)(obj);
    if( offset < 0 || offset >= KV(tree_size)(cntr) )
        PUSHs(&PL_sv_undef);
    else{
        T(VALUE) value_result;
        SP = K(ret)(SP, KV(tree_skip_l)(cntr, offset, &value_result));
#if I(VALUE) != I(void)
        SP = V(mret)(SP, value_result);
#endif
    }
    return SP;
}

SV ** KV(skip_g)(pTHX_ SV** SP, SV * obj, int offset){
    KV(tree_cntr_t) * cntr = KV(assure_tree_cntr)(obj);
    if( offset < 0 || offset >= KV(tree_size)(cntr) )
        PUSHs(&PL_sv_undef);
    else{
        T(VALUE) value_result;
        SP = K(ret)(SP, KV(tree_skip_g)(cntr, offset, &value_result));
#if I(VALUE) != I(void)
        SP = V(mret)(SP, value_result);
#endif
    }
    return SP;
}

SV ** KV(dump)(pTHX_ SV** SP, SV *obj){
    KV(tree_cntr_t) * cntr = KV(assure_tree_cntr)(obj);
    KV(tree_dump)(cntr);
    return SP;
}

SV ** KV(check)(pTHX_ SV** SP, SV * obj){
    KV(tree_cntr_t) * cntr = KV(assure_tree_cntr)(obj);

    EXTEND(SP, 3);

    if( KV(tree_check_order)(cntr) )
        PUSHs(&PL_sv_yes);
    else
        PUSHs(&PL_sv_no);
    if( tree_check_size(cntr) )
        PUSHs(&PL_sv_yes);
    else
        PUSHs(&PL_sv_no);
    if( tree_check_balance(cntr) )
        PUSHs(&PL_sv_yes);
    else
        PUSHs(&PL_sv_no);
    return SP;
}
