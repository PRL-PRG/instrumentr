#include "object_type.h"
#include "interop.h"
#include "utilities.h"
#include <vector>
#include <string>

typedef struct {
    instrumentr_object_type_t type;
    const char* name;
    SEXP klass;
} object_type_info_t;

object_type_info_t object_type_info_table[INSTRUMENTR_OBJECT_TYPE_COUNT] = {
    {INSTRUMENTR_OBJECT_TYPE_TRACER, "tracer", NULL},
    {INSTRUMENTR_OBJECT_TYPE_CALLBACK, "callback", NULL},
    {INSTRUMENTR_OBJECT_TYPE_STATE, "state", NULL},
    {INSTRUMENTR_OBJECT_TYPE_EXEC_STATS, "exec_stats", NULL},
    {INSTRUMENTR_OBJECT_TYPE_ALLOC_STATS, "alloc_stack", NULL}};

/*******************************************************************************
 * initialize
 *******************************************************************************/

void instrumentr_object_type_initialize() {
    for (int i = 0; i < INSTRUMENTR_OBJECT_TYPE_COUNT; ++i) {
        instrumentr_object_type_t object_type = (instrumentr_object_type_t) i;

        const char* object_name = instrumentr_object_type_get_name(object_type);

        std::vector<std::string> names(
            {std::string("instrumentr_") + object_name,
             std::string("instrumentr_object")});

        SEXP klass = create_class(names);

        object_type_info_table[object_type].klass = klass;

        instrumentr_sexp_acquire(klass);
    }
}

/*******************************************************************************
 * finalize
 *******************************************************************************/

void instrumentr_object_type_finalize() {
    for (int i = 0; i < INSTRUMENTR_OBJECT_TYPE_COUNT; ++i) {
        SEXP klass = object_type_info_table[i].klass;
        object_type_info_table[i].klass = NULL;
        instrumentr_sexp_release(klass);
    }
}

/*******************************************************************************
 * name
 *******************************************************************************/

const char*
instrumentr_object_type_get_name(instrumentr_object_type_t object_type) {
    if (object_type == INSTRUMENTR_OBJECT_TYPE_COUNT) {
        instrumentr_log_error("attempt to get name of pseudo object type "
                              "'INSTRUMENTR_OBJECT_TYPE_COUNT'");
    }
    int index = (int) object_type;
    return object_type_info_table[index].name;
}

/*******************************************************************************
 * class
 *******************************************************************************/

SEXP instrumentr_object_type_get_class(instrumentr_object_type_t object_type) {
    if (object_type == INSTRUMENTR_OBJECT_TYPE_COUNT) {
        instrumentr_log_error("attempt to get class of pseudo object type "
                              "'INSTRUMENTR_OBJECT_TYPE_COUNT'");
    }
    int index = (int) object_type;
    return object_type_info_table[index].klass;
}
