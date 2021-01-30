#ifndef INSTRUMENTR_R_INTERNALS_H
#define INSTRUMENTR_R_INTERNALS_H

#define TYPE_BITS 5

#define NAMED_BITS 16

/* Flags */

struct sxpinfo_struct {
    SEXPTYPE type: TYPE_BITS;
    /* ==> (FUNSXP == 99) %% 2^5 == 3 == CLOSXP
     * -> warning: `type' is narrower than values
     *              of its type
     * when SEXPTYPE was an enum */
    unsigned int scalar : 1;
    unsigned int obj : 1;
    unsigned int alt : 1;
    unsigned int gp : 16;
    unsigned int mark : 1;
    unsigned int debug : 1;
    unsigned int trace : 1; /* functions and memory tracing */
    unsigned int spare : 1; /* used on closures and when REFCNT is defined */
    unsigned int gcgen : 1; /* old generation number */
    unsigned int gccls : 3; /* node class */
    unsigned int named: NAMED_BITS;
    unsigned int extra : 32 - NAMED_BITS; /* used for immediate bindings */
};                                        /*		    Tot: 64 */

struct vecsxp_struct {
    R_xlen_t length;
    R_xlen_t truelength;
};

struct primsxp_struct {
    int offset;
};

struct symsxp_struct {
    struct SEXPREC* pname;
    struct SEXPREC* value;
    struct SEXPREC* internal;
};

struct listsxp_struct {
    struct SEXPREC* carval;
    struct SEXPREC* cdrval;
    struct SEXPREC* tagval;
};

struct envsxp_struct {
    struct SEXPREC* frame;
    struct SEXPREC* enclos;
    struct SEXPREC* hashtab;
};

struct closxp_struct {
    struct SEXPREC* formals;
    struct SEXPREC* body;
    struct SEXPREC* env;
};

struct promsxp_struct {
    struct SEXPREC* value;
    struct SEXPREC* expr;
    struct SEXPREC* env;
};

/* Every node must start with a set of sxpinfo flags and an attribute
   field. Under the generational collector these are followed by the
   fields used to maintain the collector's linked list structures. */

#define SEXPREC_HEADER             \
    struct sxpinfo_struct sxpinfo; \
    struct SEXPREC* attrib;        \
    struct SEXPREC *gengc_next_node, *gengc_prev_node

/* The standard node structure consists of a header followed by the
   node data. */
typedef struct SEXPREC {
    SEXPREC_HEADER;
    union {
        struct primsxp_struct primsxp;
        struct symsxp_struct symsxp;
        struct listsxp_struct listsxp;
        struct envsxp_struct envsxp;
        struct closxp_struct closxp;
        struct promsxp_struct promsxp;
    } u;
} SEXPREC;

#endif /* INSTRUMENTR_R_INTERNALS_H */
