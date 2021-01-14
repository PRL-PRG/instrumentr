#include "funtab.h"
#include "sexp.h"
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

int FUNTAB_SIZE = 0;
extern FUNTAB* R_FunTab;

instrumentr_function_t* functions;

int instrumentr_funtab_get_index(SEXP op) {
    return op->u.primsxp.offset;
}

CCODE instrumentr_funtab_get_definition(int index) {
    return R_FunTab[index].cfun;
}

const char* instrumentr_funtab_get_name(int index) {
    return R_FunTab[index].name;
}

int instrumentr_funtab_get_parameter_count(int index) {
    return R_FunTab[index].arity;
}

int instrumentr_funtab_is_internal(int index) {
    return ((R_FunTab[index].eval % 100) / 10) == 1;
}

int instrumentr_funtab_is_primitive(int index) {
    return !instrumentr_funtab_is_internal(index);
}

int instrumentr_funtab_is_builtin(int index) {
    return (R_FunTab[index].eval % 10) == 1;
}

int instrumentr_funtab_is_special(int index) {
    return !instrumentr_funtab_is_builtin(index);
}

instrumentr_function_t instrumentr_funtab_get_function(SEXP op) {
    int funtab_index = instrumentr_funtab_get_index(op);
    return functions[funtab_index];
}

instrumentr_function_t instrumentr_funtab_create_function(int funtab_index) {
    int builtin = instrumentr_funtab_is_builtin(funtab_index);
    const char* name = instrumentr_funtab_get_name(funtab_index);
    int parameter_count = instrumentr_funtab_get_parameter_count(funtab_index);
    int internal = instrumentr_funtab_is_internal(funtab_index);
    CCODE ccode = instrumentr_funtab_get_definition(funtab_index);

    if (builtin) {
        return instrumentr_function_create_builtin(
            funtab_index, name, parameter_count, ccode, internal);
    } else {
        return instrumentr_function_create_special(
            funtab_index, name, parameter_count, ccode, internal);
    }
}

void funtab_initialize() {
    for (FUNTAB_SIZE = 0; R_FunTab[FUNTAB_SIZE].name; FUNTAB_SIZE++)
        ;

    functions = (instrumentr_function_t*) malloc(
        FUNTAB_SIZE * sizeof(instrumentr_function_t));

    for (int i = 0; i < FUNTAB_SIZE; ++i) {
        functions[i] = instrumentr_funtab_create_function(i);
    }
}

void funtab_finalize() {
    for (int i = 0; i < FUNTAB_SIZE; ++i) {
        instrumentr_object_release(functions[i]);
    }

    free(functions);
    functions = NULL;
}

void instrumentr_funtab_instrument(const char* function_name, CCODE trace) {
    for (int i = 0; i < FUNTAB_SIZE; ++i) {
        if(!strcmp(R_FunTab[i].name, function_name)) {
            R_FunTab[i].cfun = trace;
        }
    }
}
