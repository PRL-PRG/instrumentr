#include "object.h"
#include "interop.h"

SEXP object_class_table[INSTRUMENTR_EXEC_STATS + 1];

SEXP create_class(const char* type_name) {
    SEXP klass = PROTECT(allocVector(STRSXP, 2));

    SET_STRING_ELT(klass, 0, mkChar(type_name));
    SET_STRING_ELT(klass, 1, mkChar("instrumentr_object"));

    UNPROTECT(1);

    return klass;
}

/*******************************************************************************
 * initialize
 *******************************************************************************/

void instrumentr_object_class_initialize() {
    object_class_table[INSTRUMENTR_TRACER] =
        create_class("instrumentr_tracer");
    instrumentr_sexp_acquire(object_class_table[INSTRUMENTR_TRACER]);

    object_class_table[INSTRUMENTR_CALLBACK] =
        create_class("instrumentr_callback");
    instrumentr_sexp_acquire(object_class_table[INSTRUMENTR_CALLBACK]);

    object_class_table[INSTRUMENTR_STATE] =
        create_class("instrumentr_state");
    instrumentr_sexp_acquire(object_class_table[INSTRUMENTR_STATE]);

    object_class_table[INSTRUMENTR_APPLICATION] =
        create_class("instrumentr_application");
    instrumentr_sexp_acquire(object_class_table[INSTRUMENTR_APPLICATION]);

    object_class_table[INSTRUMENTR_PACKAGE] =
        create_class("instrumentr_package");
    instrumentr_sexp_acquire(object_class_table[INSTRUMENTR_PACKAGE]);

    object_class_table[INSTRUMENTR_FUNCTION] =
        create_class("instrumentr_function");
    instrumentr_sexp_acquire(object_class_table[INSTRUMENTR_FUNCTION]);

    object_class_table[INSTRUMENTR_CALL] = create_class("instrumentr_call");
    instrumentr_sexp_acquire(object_class_table[INSTRUMENTR_CALL]);

    object_class_table[INSTRUMENTR_CALL_STACK] =
        create_class("instrumentr_call_stack");
    instrumentr_sexp_acquire(object_class_table[INSTRUMENTR_CALL_STACK]);

    object_class_table[INSTRUMENTR_PARAMETER] =
        create_class("instrumentr_parameter");
    instrumentr_sexp_acquire(object_class_table[INSTRUMENTR_PARAMETER]);

    object_class_table[INSTRUMENTR_ARGUMENT] =
        create_class("instrumentr_argument");
    instrumentr_sexp_acquire(object_class_table[INSTRUMENTR_ARGUMENT]);

    object_class_table[INSTRUMENTR_PROMISE] = create_class("instrumentr_promise");
    instrumentr_sexp_acquire(object_class_table[INSTRUMENTR_PROMISE]);

    object_class_table[INSTRUMENTR_VALUE] =
        create_class("instrumentr_value");
    instrumentr_sexp_acquire(object_class_table[INSTRUMENTR_VALUE]);

    object_class_table[INSTRUMENTR_FRAME] = create_class("instrumentr_frame");
    instrumentr_sexp_acquire(object_class_table[INSTRUMENTR_FRAME]);

    object_class_table[INSTRUMENTR_CONTEXT] = create_class("instrumentr_context");
    instrumentr_sexp_acquire(object_class_table[INSTRUMENTR_CONTEXT]);

    object_class_table[INSTRUMENTR_EXEC_STATS] = create_class("instrumentr_exec_stats");
    instrumentr_sexp_acquire(object_class_table[INSTRUMENTR_EXEC_STATS]);
}

/*******************************************************************************
 * finalize
 *******************************************************************************/

void instrumentr_object_class_finalize() {
    instrumentr_sexp_release(object_class_table[INSTRUMENTR_TRACER]);
    object_class_table[INSTRUMENTR_TRACER] = NULL;
    instrumentr_sexp_release(object_class_table[INSTRUMENTR_CALLBACK]);
    object_class_table[INSTRUMENTR_CALLBACK] = NULL;
    instrumentr_sexp_release(object_class_table[INSTRUMENTR_STATE]);
    object_class_table[INSTRUMENTR_STATE] = NULL;
    instrumentr_sexp_release(object_class_table[INSTRUMENTR_APPLICATION]);
    object_class_table[INSTRUMENTR_APPLICATION] = NULL;
    instrumentr_sexp_release(object_class_table[INSTRUMENTR_PACKAGE]);
    object_class_table[INSTRUMENTR_PACKAGE] = NULL;
    instrumentr_sexp_release(object_class_table[INSTRUMENTR_FUNCTION]);
    object_class_table[INSTRUMENTR_FUNCTION] = NULL;
    instrumentr_sexp_release(object_class_table[INSTRUMENTR_CALL]);
    object_class_table[INSTRUMENTR_CALL] = NULL;
    instrumentr_sexp_release(object_class_table[INSTRUMENTR_CALL_STACK]);
    object_class_table[INSTRUMENTR_CALL_STACK] = NULL;
    instrumentr_sexp_release(object_class_table[INSTRUMENTR_PARAMETER]);
    object_class_table[INSTRUMENTR_PARAMETER] = NULL;
    instrumentr_sexp_release(object_class_table[INSTRUMENTR_ARGUMENT]);
    object_class_table[INSTRUMENTR_ARGUMENT] = NULL;
    instrumentr_sexp_release(object_class_table[INSTRUMENTR_PROMISE]);
    object_class_table[INSTRUMENTR_PROMISE] = NULL;
    instrumentr_sexp_release(object_class_table[INSTRUMENTR_VALUE]);
    object_class_table[INSTRUMENTR_VALUE] = NULL;
    instrumentr_sexp_release(object_class_table[INSTRUMENTR_FRAME]);
    object_class_table[INSTRUMENTR_FRAME] = NULL;
    instrumentr_sexp_release(object_class_table[INSTRUMENTR_CONTEXT]);
    object_class_table[INSTRUMENTR_CONTEXT] = NULL;
    instrumentr_sexp_release(object_class_table[INSTRUMENTR_EXEC_STATS]);
    object_class_table[INSTRUMENTR_EXEC_STATS] = NULL;
}

/*******************************************************************************
 * class
 *******************************************************************************/

SEXP instrumentr_object_get_class(instrumentr_object_type_t object_type) {
    return object_class_table[object_type];
}
