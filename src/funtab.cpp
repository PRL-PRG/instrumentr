#include "funtab.h"
#include "r_internals.h"
#include "function.h"
#include "object.h"

/* Information for Deparsing Expressions */
typedef enum {
    PP_INVALID = 0,
    PP_ASSIGN = 1,
    PP_ASSIGN2 = 2,
    PP_BINARY = 3,
    PP_BINARY2 = 4,
    PP_BREAK = 5,
    PP_CURLY = 6,
    PP_FOR = 7,
    PP_FUNCALL = 8,
    PP_FUNCTION = 9,
    PP_IF = 10,
    PP_NEXT = 11,
    PP_PAREN = 12,
    PP_RETURN = 13,
    PP_SUBASS = 14,
    PP_SUBSET = 15,
    PP_WHILE = 16,
    PP_UNARY = 17,
    PP_DOLLAR = 18,
    PP_FOREIGN = 19,
    PP_REPEAT = 20
} PPkind;

typedef enum {
    PREC_FN = 0,
    PREC_EQ = 1,
    PREC_LEFT = 2,
    PREC_RIGHT = 3,
    PREC_TILDE = 4,
    PREC_OR = 5,
    PREC_AND = 6,
    PREC_NOT = 7,
    PREC_COMPARE = 8,
    PREC_SUM = 9,
    PREC_PROD = 10,
    PREC_PERCENT = 11,
    PREC_COLON = 12,
    PREC_SIGN = 13,
    PREC_POWER = 14,
    PREC_SUBSET = 15,
    PREC_DOLLAR = 16,
    PREC_NS = 17
} PPprec;

typedef struct {
    PPkind kind;             /* deparse kind */
    PPprec precedence;       /* operator precedence */
    unsigned int rightassoc; /* right associative? */
} PPinfo;

/* The type definitions for the table of built-in functions. */
/* This table can be found in ../main/names.c */
typedef struct {
    char* name;  /* print name */
    CCODE cfun;  /* c-code address */
    int code;    /* offset within c-code */
    int eval;    /* evaluate args? */
    int arity;   /* function arity */
    PPinfo gram; /* pretty-print info */
} FUNTAB;

int instrumentr_funtab_get_index(SEXP op) {
    return op->u.primsxp.offset;
}

FUNTAB* get_funtab_entry(int index) {
    FUNTAB* R_FunTab = (FUNTAB*) dyntrace_get_funtab();
    return R_FunTab + index;
}

CCODE instrumentr_funtab_get_definition(FUNTAB* entry) {
    return entry->cfun;
}

const char* instrumentr_funtab_get_name(FUNTAB* entry) {
    return entry->name;
}

int instrumentr_funtab_get_parameter_count(FUNTAB* entry) {
    return entry->arity;
}

int instrumentr_funtab_is_internal(FUNTAB* entry) {
    return ((entry->eval % 100) / 10) == 1;
}

int instrumentr_funtab_is_primitive(FUNTAB* entry) {
    return !instrumentr_funtab_is_internal(entry);
}

int instrumentr_funtab_is_builtin(FUNTAB* entry) {
    return (entry->eval % 10) == 1;
}

int instrumentr_funtab_is_special(FUNTAB* entry) {
    return !instrumentr_funtab_is_builtin(entry);
}

int instrumentr_funtab_get_size() {
    FUNTAB* R_FunTab = (FUNTAB*) dyntrace_get_funtab();
    int size = 0;
    while (R_FunTab[size].name) {
        size++;
    }
    return size;
}

instrumentr_function_t
instrumentr_funtab_create_function(instrumentr_state_t state, int index) {
    FUNTAB* entry = get_funtab_entry(index);
    int builtin = instrumentr_funtab_is_builtin(entry);
    const char* name = instrumentr_funtab_get_name(entry);
    int parameter_count = instrumentr_funtab_get_parameter_count(entry);
    int internal = instrumentr_funtab_is_internal(entry);
    CCODE ccode = instrumentr_funtab_get_definition(entry);

    if (builtin) {
        return instrumentr_function_create_builtin(
            state, index, name, parameter_count, ccode, internal);
    } else {
        return instrumentr_function_create_special(
            state, index, name, parameter_count, ccode, internal);
    }
}
