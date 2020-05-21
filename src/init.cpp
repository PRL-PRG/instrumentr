#include "r_api.h"
#include "../inst/include/lightr.hpp"
#include <R_ext/Rdynload.h>

#include <stdio.h>

extern "C" {
static const R_CallMethodDef CallEntries[] = {
    /* lightr */
    {"lightr_get_application", (DL_FUNC) &r_lightr_get_application, 0},

    /* Object */
    {"object_get_id", (DL_FUNC) &r_object_get_id, 1},

    /* Application */
    {"application_get_packages", (DL_FUNC) &r_application_get_packages, 1},
    {"application_add_package", (DL_FUNC) &r_application_add_package, 2},

    /* Package */
    {"package_get_application", (DL_FUNC) &r_package_get_application, 1},
    {"package_get_name", (DL_FUNC) &r_package_get_name, 1},
    {"package_get_functions", (DL_FUNC) &r_package_get_functions, 1},
    {"package_add_function", (DL_FUNC) &r_package_add_function, 2},

    /* Function */
    {"function_get_package", (DL_FUNC) &r_function_get_package, 1},
    {"function_get_name", (DL_FUNC) &r_function_get_name, 1},
    {"function_get_parameter_count", (DL_FUNC) &r_function_get_parameter_count, 1},

    /* Call */
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
    {"call_stack_get_size", (DL_FUNC) &r_call_stack_get_size, 1},
    {"call_stack_get_frame", (DL_FUNC) &r_call_stack_get_frame, 2},
    {"call_stack_peek_frame", (DL_FUNC) &r_call_stack_peek_frame, 2},

    {NULL, NULL, 0}};

void R_init_lightr(DllInfo* dll) {
    R_registerRoutines(dll, NULL, CallEntries, NULL, NULL);
    R_useDynamicSymbols(dll, FALSE);
    lightr::initialize_lightr();
}
}
