#include "object_internals.h"
#include <instrumentr/sexp.h>

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
    object_class_table[INSTRUMENTR_APPLICATION] =
        instrumentr_sexp_acquire(create_class("instrumentr_application"));
    object_class_table[INSTRUMENTR_PACKAGE] =
        instrumentr_sexp_acquire(create_class("instrumentr_package"));
    object_class_table[INSTRUMENTR_FUNCTION] =
        instrumentr_sexp_acquire(create_class("instrumentr_function"));
    object_class_table[INSTRUMENTR_CALL] =
        instrumentr_sexp_acquire(create_class("instrumentr_call"));
    object_class_table[INSTRUMENTR_PARAMETER] =
        instrumentr_sexp_acquire(create_class("instrumentr_parameter"));
    object_class_table[INSTRUMENTR_ARGUMENT] =
        instrumentr_sexp_acquire(create_class("instrumentr_argument"));
}

/*******************************************************************************
 * finalize
 *******************************************************************************/

void instrumentr_object_class_finalize() {
    instrumentr_sexp_release(object_class_table[INSTRUMENTR_APPLICATION]);
    object_class_table[INSTRUMENTR_APPLICATION] = NULL;
    instrumentr_sexp_release(object_class_table[INSTRUMENTR_PACKAGE]);
    object_class_table[INSTRUMENTR_PACKAGE] = NULL;
    instrumentr_sexp_release(object_class_table[INSTRUMENTR_FUNCTION]);
    object_class_table[INSTRUMENTR_FUNCTION] = NULL;
    instrumentr_sexp_release(object_class_table[INSTRUMENTR_CALL]);
    object_class_table[INSTRUMENTR_CALL] = NULL;
    instrumentr_sexp_release(object_class_table[INSTRUMENTR_PARAMETER]);
    object_class_table[INSTRUMENTR_PARAMETER] = NULL;
    instrumentr_sexp_release(object_class_table[INSTRUMENTR_ARGUMENT]);
    object_class_table[INSTRUMENTR_ARGUMENT] = NULL;
}
