#define PERL_NO_GET_CONTEXT
#include "EXTERN.h"
#include "perl.h"
#include "XSUB.h"

#include "ppport.h"

#include "const-c.inc"

#include "tree.h"

static inline tree_cntr_t * assure_tree_cntr(SV * obj){
    if( !obj )
        croak("assure_tree_cntr: NULL ptr");

    if( !SvROK(obj) )
        croak("assure_tree_cntr: try to dereference a non-reference");

    SV * ret = SvRV(obj);
    if( !ret )
        croak("assure_tree_cntr: deref to NULL");

    if( !SvROK(ret) )
        croak("assure_tree_cntr: deref to non-reference");

    tree_cntr_t * cntr = (tree_cntr_t*) SvRV(ret);
    if( !cntr )
        croak("assure_tree_cntr: NULL cntr");

    if( cntr->secret != TREE_SECRET )
        croak("assure_tree_cntr: unmatched secret");

    return cntr;
}

MODULE = Tree::SizeBalanced		PACKAGE = Tree::SizeBalanced		

INCLUDE: const-xs.inc

void new(SV * class)
    PPCODE:
        tree_cntr_t * cntr;
        Newx(cntr, 1, tree_cntr_t);
        init_tree_cntr(cntr);

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

void DESTROY(SV * obj)
    PPCODE:
        tree_cntr_t * cntr = assure_tree_cntr(obj);
        empty_tree_cntr(cntr);
        Safefree(cntr);
        SvRV(SvRV(obj)) = NULL;

void size(SV *obj)
    PPCODE:
        dXSTARG;
        tree_cntr_t * cntr = assure_tree_cntr(obj);
        PUSHu((UV) tree_size(cntr));

void insert(SV * obj, IV key)
    PPCODE:
        tree_cntr_t * cntr = assure_tree_cntr(obj);
        tree_insert(cntr, key);

void delete(SV * obj, IV key)
    PPCODE:
        tree_cntr_t * cntr = assure_tree_cntr(obj);
        if( tree_delete(cntr, key) )
            PUSHs(&PL_sv_yes);
        else
            PUSHs(&PL_sv_no);

void find(SV * obj, IV key)
    PPCODE:
        tree_cntr_t * cntr = assure_tree_cntr(obj);
        if( tree_find(cntr, key) )
            PUSHs(&PL_sv_yes);
        else
            PUSHs(&PL_sv_no);

void find_lt(SV * obj, IV key)
    PPCODE:
        dXSTARG;
        tree_cntr_t * cntr = assure_tree_cntr(obj);
        IV found = tree_find_lt(cntr, key, key);
        if( found == key )
            PUSHs(&PL_sv_undef);
        else
            PUSHi(found);

void find_le(SV * obj, IV key)
    PPCODE:
        dXSTARG;
        tree_cntr_t * cntr = assure_tree_cntr(obj);
        IV found = tree_find_le(cntr, key+1, key);
        if( found == key+1 )
            PUSHs(&PL_sv_undef);
        else
            PUSHi(found);

void find_gt(SV * obj, IV key)
    PPCODE:
        dXSTARG;
        tree_cntr_t * cntr = assure_tree_cntr(obj);
        IV found = tree_find_gt(cntr, key, key);
        if( found == key )
            PUSHs(&PL_sv_undef);
        else
            PUSHi(found);

void find_ge(SV * obj, IV key)
    PPCODE:
        dXSTARG;
        tree_cntr_t * cntr = assure_tree_cntr(obj);
        IV found = tree_find_ge(cntr, key-1, key);
        if( found == key-1 )
            PUSHs(&PL_sv_undef);
        else
            PUSHi(found);

void find_min(SV * obj)
    PPCODE:
        dXSTARG;
        tree_cntr_t * cntr = assure_tree_cntr(obj);
        if( tree_size(cntr) == 0 )
            PUSHs(&PL_sv_undef);
        else
            PUSHi(tree_find_min(cntr));

void find_max(SV * obj)
    PPCODE:
        dXSTARG;
        tree_cntr_t * cntr = assure_tree_cntr(obj);
        if( tree_size(cntr) == 0 )
            PUSHs(&PL_sv_undef);
        else
            PUSHi(tree_find_max(cntr));

void count_lt(SV * obj, IV key)
    PPCODE:
        dXSTARG;
        tree_cntr_t * cntr = assure_tree_cntr(obj);
        PUSHu((UV) tree_count_lt(cntr, key));

void count_le(SV * obj, IV key)
    PPCODE:
        dXSTARG;
        tree_cntr_t * cntr = assure_tree_cntr(obj);
        PUSHu((UV) tree_count_le(cntr, key));

void count_gt(SV * obj, IV key)
    PPCODE:
        dXSTARG;
        tree_cntr_t * cntr = assure_tree_cntr(obj);
        PUSHu((UV) tree_count_gt(cntr, key));

void count_ge(SV * obj, IV key)
    PPCODE:
        dXSTARG;
        tree_cntr_t * cntr = assure_tree_cntr(obj);
        PUSHu((UV) tree_count_ge(cntr, key));

void skip_l(SV * obj, int offset)
    PPCODE:
        dXSTARG;
        tree_cntr_t * cntr = assure_tree_cntr(obj);
        if( offset < 0 || offset >= tree_size(cntr) )
            PUSHs(&PL_sv_undef);
        else
            PUSHi(tree_skip_l(cntr, offset));

void skip_g(SV * obj, int offset)
    PPCODE:
        dXSTARG;
        tree_cntr_t * cntr = assure_tree_cntr(obj);
        if( offset < 0 || offset >= tree_size(cntr) )
            PUSHs(&PL_sv_undef);
        else
            PUSHi(tree_skip_g(cntr, offset));

void dump(SV *obj)
    PPCODE:
        tree_cntr_t * cntr = assure_tree_cntr(obj);
        tree_dump(cntr);

void check(SV * obj)
    PPCODE:
        tree_cntr_t * cntr = assure_tree_cntr(obj);

        EXTEND(SP, 3);
        if( tree_check_order(cntr) )
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
