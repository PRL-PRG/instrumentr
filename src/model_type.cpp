#include "model_type.h"
#include "utilities.h"
#include "interop.h"
#include <vector>
#include <string>

typedef struct {
    instrumentr_model_type_t type;
    const char* name;
    SEXP klass;
} model_type_info_t;

model_type_info_t model_type_info_table[INSTRUMENTR_MODEL_TYPE_COUNT] = {
    {INSTRUMENTR_MODEL_TYPE_APPLICATION, "application", NULL},
    {INSTRUMENTR_MODEL_TYPE_INTEGER, "integer", NULL},
    {INSTRUMENTR_MODEL_TYPE_REAL, "real", NULL},
    {INSTRUMENTR_MODEL_TYPE_LOGICAL, "logical", NULL},
    {INSTRUMENTR_MODEL_TYPE_ENVIRONMENT, "environment", NULL},
    {INSTRUMENTR_MODEL_TYPE_FUNCTION, "function", NULL},
    {INSTRUMENTR_MODEL_TYPE_CALL, "call", NULL},
    {INSTRUMENTR_MODEL_TYPE_CALL_STACK, "call_stack", NULL},
    {INSTRUMENTR_MODEL_TYPE_PARAMETER, "parameter", NULL},
    {INSTRUMENTR_MODEL_TYPE_ARGUMENT, "argument", NULL},
    {INSTRUMENTR_MODEL_TYPE_PROMISE, "promise", NULL},
    {INSTRUMENTR_MODEL_TYPE_VALUE, "value", NULL},
    {INSTRUMENTR_MODEL_TYPE_FRAME, "frame", NULL},
    {INSTRUMENTR_MODEL_TYPE_CONTEXT, "context", NULL},
    {INSTRUMENTR_MODEL_TYPE_MISCELLANEOUS, "miscellaneous", NULL}};

/*******************************************************************************
 * initialize
 *******************************************************************************/

void instrumentr_model_type_initialize() {
    for (int i = 0; i < INSTRUMENTR_MODEL_TYPE_COUNT; ++i) {
        instrumentr_model_type_t model_type = (instrumentr_model_type_t) i;

        const char* model_name = instrumentr_model_type_get_name(model_type);

        std::vector<std::string> names(
            {std::string("instrumentr_") + model_name,
             std::string("instrumentr_model")});

        SEXP klass = create_class(names);

        model_type_info_table[model_type].klass = klass;

        instrumentr_sexp_acquire(klass);
    }
}

/*******************************************************************************
 * finalize
 *******************************************************************************/

void instrumentr_model_type_finalize() {
    for (int i = 0; i < INSTRUMENTR_MODEL_TYPE_COUNT; ++i) {
        SEXP klass = model_type_info_table[i].klass;
        model_type_info_table[i].klass = NULL;
        instrumentr_sexp_release(klass);
    }
}

/*******************************************************************************
 * name
 *******************************************************************************/

const char*
instrumentr_model_type_get_name(instrumentr_model_type_t model_type) {
    if (model_type == INSTRUMENTR_MODEL_TYPE_COUNT) {
        instrumentr_log_error("attempt to get name of pseudo model type "
                              "'INSTRUMENTR_MODEL_TYPE_COUNT'");
    }
    int index = (int) model_type;
    return model_type_info_table[index].name;
}

/*******************************************************************************
 * class
 *******************************************************************************/

SEXP instrumentr_model_type_get_class(instrumentr_model_type_t model_type) {
    if (model_type == INSTRUMENTR_MODEL_TYPE_COUNT) {
        instrumentr_log_error("attempt to get class of pseudo model type "
                              "'INSTRUMENTR_MODEL_TYPE_COUNT'");
    }
    int index = (int) model_type;
    return model_type_info_table[index].klass;
}
