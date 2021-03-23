#include "value_type.h"
#include "utilities.h"
#include "interop.h"
#include <vector>
#include <string>

typedef struct {
    instrumentr_value_type_t type;
    const char* name;
    SEXP klass;
} value_type_info_t;

value_type_info_t value_type_info_table[INSTRUMENTR_VALUE_TYPE_COUNT] = {
    {INSTRUMENTR_VALUE_TYPE_NULL, "null", NULL},
    {INSTRUMENTR_VALUE_TYPE_SYMBOL, "symbol", NULL},
    {INSTRUMENTR_VALUE_TYPE_PAIRLIST, "pairlist", NULL},
    {INSTRUMENTR_VALUE_TYPE_CLOSURE, "closure", NULL},
    {INSTRUMENTR_VALUE_TYPE_ENVIRONMENT, "environment", NULL},
    {INSTRUMENTR_VALUE_TYPE_PROMISE, "promise", NULL},
    {INSTRUMENTR_VALUE_TYPE_LANGUAGE, "language", NULL},
    {INSTRUMENTR_VALUE_TYPE_SPECIAL, "special", NULL},
    {INSTRUMENTR_VALUE_TYPE_BUILTIN, "builtin", NULL},
    {INSTRUMENTR_VALUE_TYPE_CHAR, "char", NULL},
    {INSTRUMENTR_VALUE_TYPE_LOGICAL, "logical", NULL},
    {INSTRUMENTR_VALUE_TYPE_INTEGER, "integer", NULL},
    {INSTRUMENTR_VALUE_TYPE_REAL, "real", NULL},
    {INSTRUMENTR_VALUE_TYPE_COMPLEX, "complex", NULL},
    {INSTRUMENTR_VALUE_TYPE_CHARACTER, "character", NULL},
    {INSTRUMENTR_VALUE_TYPE_DOT, "dot", NULL},
    {INSTRUMENTR_VALUE_TYPE_LIST, "list", NULL},
    {INSTRUMENTR_VALUE_TYPE_EXPRESSION, "expression", NULL},
    {INSTRUMENTR_VALUE_TYPE_BYTECODE, "bytecode", NULL},
    {INSTRUMENTR_VALUE_TYPE_EXTERNALPTR, "externalptr", NULL},
    {INSTRUMENTR_VALUE_TYPE_WEAKREF, "weakref", NULL},
    {INSTRUMENTR_VALUE_TYPE_RAW, "raw", NULL},
    {INSTRUMENTR_VALUE_TYPE_S4, "s4", NULL},
    {INSTRUMENTR_VALUE_TYPE_UNBOUND, "unbound", NULL},
    {INSTRUMENTR_VALUE_TYPE_MISSING, "missing", NULL},
    {INSTRUMENTR_VALUE_TYPE_MISCELLANEOUS, "miscellaneous", NULL}};

/*******************************************************************************
 * initialize
 *******************************************************************************/

void instrumentr_value_type_initialize() {
    for (int i = 0; i < INSTRUMENTR_VALUE_TYPE_COUNT; ++i) {
        instrumentr_value_type_t value_type = (instrumentr_value_type_t) i;

        const char* value_name = instrumentr_value_type_get_name(value_type);

        std::vector<std::string> names(
            {std::string("instrumentr_") + value_name,
             std::string("instrumentr_value")});

        SEXP klass = create_class(names);

        value_type_info_table[value_type].klass = klass;

        instrumentr_sexp_acquire(klass);
    }
}

/*******************************************************************************
 * finalize
 *******************************************************************************/

void instrumentr_value_type_finalize() {
    for (int i = 0; i < INSTRUMENTR_VALUE_TYPE_COUNT; ++i) {
        SEXP klass = value_type_info_table[i].klass;
        value_type_info_table[i].klass = NULL;
        instrumentr_sexp_release(klass);
    }
}

/*******************************************************************************
 * name
 *******************************************************************************/

const char*
instrumentr_value_type_get_name(instrumentr_value_type_t value_type) {
    if (value_type == INSTRUMENTR_VALUE_TYPE_COUNT) {
        instrumentr_log_error("attempt to get name of pseudo value type "
                              "'INSTRUMENTR_VALUE_TYPE_COUNT'");
    }
    int index = (int) value_type;
    return value_type_info_table[index].name;
}

/*******************************************************************************
 * class
 *******************************************************************************/

SEXP instrumentr_value_type_get_class(instrumentr_value_type_t value_type) {
    if (value_type == INSTRUMENTR_VALUE_TYPE_COUNT) {
        instrumentr_log_error("attempt to get class of pseudo value type "
                              "'INSTRUMENTR_VALUE_TYPE_COUNT'");
    }
    int index = (int) value_type;
    return value_type_info_table[index].klass;
}
