#include "r_api.hpp"
#include <R_ext/Rdynload.h>

#include <stdio.h>

extern "C" {
static const R_CallMethodDef CallEntries[] = {
    /* Call */
    {"call_get_id", (DL_FUNC) &r_call_get_id, 1},
    {"call_get_package_name", (DL_FUNC) &r_call_get_package_name, 1},
    {"call_get_function_name", (DL_FUNC) &r_call_get_function_name, 1},
    {"call_get_parameter_count", (DL_FUNC) &r_call_get_parameter_count, 1},
    {"call_get_state", (DL_FUNC) &r_call_get_state, 1},
    {"call_get_argument", (DL_FUNC) &r_call_get_argument, 2},
    /* Argument */
    {"argument_get_position", (DL_FUNC) &r_argument_get_position, 1},
    {"argument_get_name", (DL_FUNC) &r_argument_get_name, 1},
    {"argument_is_evaluated", (DL_FUNC) &r_argument_is_evaluated, 1},
    {"argument_get_call", (DL_FUNC) &r_argument_get_call, 1},

    /* CallStack */
    {"call_stack_get_call_stack", (DL_FUNC) &r_call_stack_get_call_stack, 0},
    {"call_stack_get_size", (DL_FUNC) &r_call_stack_get_size, 1},
    {"call_stack_get_frame", (DL_FUNC) &r_call_stack_get_frame, 2},
    {"call_stack_peek_frame", (DL_FUNC) &r_call_stack_peek_frame, 2},

    {NULL, NULL, 0}};

void R_init_lightr(DllInfo* dll) {
    R_registerRoutines(dll, NULL, CallEntries, NULL, NULL);
    R_useDynamicSymbols(dll, FALSE);
}
}
