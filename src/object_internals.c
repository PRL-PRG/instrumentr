#include "object.h"
#include "interop.h"

SEXP object_class_table[INSTRUMENTR_ARGUMENT + 1];

SEXP create_class(const char* type_name) {
    SEXP class = PROTECT(allocVector(STRSXP, 2));

    SET_STRING_ELT(class, 0, mkChar(type_name));
    SET_STRING_ELT(class, 1, mkChar("instrumentr_object"));

    UNPROTECT(1);

    return class;
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
}

/*******************************************************************************
 * finalize
 *******************************************************************************/

void instrumentr_object_class_finalize() {
    instrumentr_sexp_release(object_class_table[INSTRUMENTR_TRACER]);
    object_class_table[INSTRUMENTR_TRACER] = NULL;
    instrumentr_sexp_release(object_class_table[INSTRUMENTR_CALLBACK]);
    object_class_table[INSTRUMENTR_CALLBACK] = NULL;
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
}

/*******************************************************************************
 * class
 *******************************************************************************/

SEXP instrumentr_object_get_class(instrumentr_object_type_t object_type) {
    return object_class_table[object_type];
}
