#include "r_api.hpp"
#include <R_ext/Rdynload.h>

#include <stdio.h>

extern "C" {
static const R_CallMethodDef CallEntries[] = {
    /* Call */
    {"get_id", (DL_FUNC) &r_get_id, 1},
    {"get_package_name", (DL_FUNC) &r_get_package_name, 1},
    {"get_function_name", (DL_FUNC) &r_get_function_name, 1},
    {"get_parameter_count", (DL_FUNC) &r_get_parameter_count, 1},
    {"get_state", (DL_FUNC) &r_get_state, 1},
    {"get_argument", (DL_FUNC) &r_get_argument, 2},
    /* Argument */
    {"get_position", (DL_FUNC) &r_get_position, 1},
    {"get_name", (DL_FUNC) &r_get_name, 1},
    {"is_evaluated", (DL_FUNC) &r_is_evaluated, 1},
    {"get_call", (DL_FUNC) &r_get_call, 1},

    {NULL, NULL, 0}};

void R_init_lightr(DllInfo* dll) {
    R_registerRoutines(dll, NULL, CallEntries, NULL, NULL);
    R_useDynamicSymbols(dll, FALSE);
}
}
