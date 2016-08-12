// vim: filetype=xs
#ifndef TREE_CUSTOMIZE__H
#define TREE_CUSTOMIZE__H

#define void_id 0
#define int_id 1
#define num_id 2
#define str_id 3
#define any_id 4

#define void_t void*
#define int_t IV
#define num_t NV
#define str_t SV*
#define any_t SV*

static inline T(int) max_key_int(){
    return (((1 << (IVSIZE - 2)) - 1) << 1) + 1;
}
static inline T(int) min_key_int(){
    return -(((1 << (IVSIZE - 2)) - 1) << 1) - 2;
}
static inline T(num) max_key_num(){
    return INFINITY;
}
static inline T(num) min_key_num(){
    return -INFINITY;
}
static inline T(str) max_key_str(){
    return &PL_sv_undef;
}
static inline T(str) min_key_str(){
    return &PL_sv_undef;
}
static inline T(any) max_key_any(){
    return &PL_sv_undef;
}
static inline T(any) min_key_any(){
    return &PL_sv_undef;
}

static inline T(int) unknown_int(){
    return min_key_int();
}
static inline T(num) unknown_num(){
    return min_key_num();
}
static inline T(str) unknown_str(){
    return NULL;
}
static inline T(any) unknown_any(){
    return NULL;
}

static inline T(void) from_sv_void(SV * sv){
    return NULL;
}
static inline T(int) from_sv_int(SV * sv){
    return SvIV(sv);
}
static inline T(num) from_sv_num(SV * sv){
    return SvNV(sv);
}
static inline T(str) from_sv_str(SV * sv){
    return sv;
}
static inline T(any) from_sv_any(SV * sv){
    return sv;
}

static inline T(void) copy_sv_void(SV * sv){
    return NULL;
}
static inline T(int) copy_sv_int(SV * sv){
    return SvIV(sv);
}
static inline T(num) copy_sv_num(SV * sv){
    return SvNV(sv);
}
static inline T(str) copy_sv_str(SV * sv){
    return newSVsv(sv);
}
static inline T(any) copy_sv_any(SV * sv){
    return newSVsv(sv);
}

static inline SV** ret_int(SV ** SP, T(int) key){
    dTARGET;
    PUSHi(key);
    return SP;
}
static inline SV** ret_num(SV ** SP, T(num) key){
    dTARGET;
    PUSHn(key);
    return SP;
}
static inline SV** ret_str(SV ** SP, T(str) key){
    PUSHs(key);
    return SP;
}
static inline SV** ret_any(SV ** SP, T(any) key){
    PUSHs(key);
    return SP;
}

static inline SV** mret_int(SV ** SP, T(int) key){
    mPUSHi(key);
    return SP;
}
static inline SV** mret_num(SV ** SP, T(num) key){
    mPUSHn(key);
    return SP;
}
static inline SV** mret_str(SV ** SP, T(str) key){
    PUSHs(key);
    return SP;
}
static inline SV** mret_any(SV ** SP, T(any) key){
    PUSHs(key);
    return SP;
}

static inline IV cmp_int(T(int) a, T(int) b, cmp_t cmp){
    return a - b;
}
static inline NV cmp_num(T(num) a, T(num) b, cmp_t cmp){
    return a - b;
}
static inline IV cmp_str(T(str) a, T(str) b, cmp_t cmp){
    return cmp(a, b); // XXX 應該換成固定的字串比對
}
static inline IV cmp_any(T(any) a, T(any) b, cmp_t cmp){
    return cmp(a, b);
}

#endif
