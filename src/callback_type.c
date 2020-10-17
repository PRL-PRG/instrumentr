#include "callback_type.h"

typedef struct {
    instrumentr_callback_type_t type;
    const char* name;
    int parameter_count;
} callback_type_info_t;

callback_type_info_t table[] = {
    /* INSTRUMENTR_CALLBACK_APPLICATION_LOAD */
    {INSTRUMENTR_CALLBACK_APPLICATION_LOAD, "application_load", 3},
    /* INSTRUMENTR_CALLBACK_APPLICATION_UNLOAD */
    {INSTRUMENTR_CALLBACK_APPLICATION_UNLOAD, "application_unload", 3},
    /* INSTRUMENTR_CALLBACK_APPLICATION_ATTACH */
    {INSTRUMENTR_CALLBACK_APPLICATION_ATTACH, "application_attach", 3},
    /* INSTRUMENTR_CALLBACK_APPLICATION_DETACH */
    {INSTRUMENTR_CALLBACK_APPLICATION_DETACH, "application_detach", 3},
    /* INSTRUMENTR_CALLBACK_PACKAGE_LOAD */
    {INSTRUMENTR_CALLBACK_PACKAGE_LOAD, "package_load", 4},
    /* INSTRUMENTR_CALLBACK_PACKAGE_UNLOAD */
    {INSTRUMENTR_CALLBACK_PACKAGE_UNLOAD, "package_unload", 4},
    /* INSTRUMENTR_CALLBACK_PACKAGE_ATTACH */
    {INSTRUMENTR_CALLBACK_PACKAGE_ATTACH, "package_attach", 4},
    /* INSTRUMENTR_CALLBACK_PACKAGE_DETACH */
    {INSTRUMENTR_CALLBACK_PACKAGE_DETACH, "package_detach", 4},
    /* INSTRUMENTR_CALLBACK_FUNCTION_ATTACH */
    {INSTRUMENTR_CALLBACK_FUNCTION_ATTACH, "function_attach", 5},
    /* INSTRUMENTR_CALLBACK_FUNCTION_DETACH */
    {INSTRUMENTR_CALLBACK_FUNCTION_DETACH, "function_detach", 5},
    /* INSTRUMENTR_CALLBACK_CALL_ENTRY */
    {INSTRUMENTR_CALLBACK_CALL_ENTRY, "call_entry", 6},
    /* INSTRUMENTR_CALLBACK_CALL_EXIT */
    {INSTRUMENTR_CALLBACK_CALL_EXIT, "call_exit", 6},
    /* INSTRUMENTR_CALLBACK_BUILTIN_CALL_ENTRY */
    {INSTRUMENTR_CALLBACK_BUILTIN_CALL_ENTRY, "builtin_call_entry", 7},
    /* INSTRUMENTR_CALLBACK_BUILTIN_CALL_EXIT */
    {INSTRUMENTR_CALLBACK_BUILTIN_CALL_EXIT, "builtin_call_exit", 8},
    /* INSTRUMENTR_CALLBACK_SPECIAL_CALL_ENTRY */
    {INSTRUMENTR_CALLBACK_SPECIAL_CALL_ENTRY, "special_call_entry", 7},
    /* INSTRUMENTR_CALLBACK_SPECIAL_CALL_EXIT */
    {INSTRUMENTR_CALLBACK_SPECIAL_CALL_EXIT, "special_call_exit", 8},
    /* INSTRUMENTR_CALLBACK_CLOSURE_CALL_ENTRY */
    {INSTRUMENTR_CALLBACK_CLOSURE_CALL_ENTRY, "closure_call_entry", 7},
    /* INSTRUMENTR_CALLBACK_CLOSURE_CALL_EXIT */
    {INSTRUMENTR_CALLBACK_CLOSURE_CALL_EXIT, "closure_call_exit", 8},
    /* INSTRUMENTR_CALLBACK_EVAL_ENTRY */
    {INSTRUMENTR_CALLBACK_EVAL_ENTRY, "eval_entry", 5},
    /* INSTRUMENTR_CALLBACK_EVAL_EXIT */
    {INSTRUMENTR_CALLBACK_EVAL_EXIT, "eval_exit", 6},
    /* INSTRUMENTR_CALLBACK_GC_ALLOCATION */
    {INSTRUMENTR_CALLBACK_GC_ALLOCATION, "gc_allocation", 4},
    /* INSTRUMENTR_CALLBACK_VARIABLE_DEFINITION */
    {INSTRUMENTR_CALLBACK_VARIABLE_DEFINITION, "variable_definition", 6},
    /* INSTRUMENTR_CALLBACK_VARIABLE_ASSIGNMENT */
    {INSTRUMENTR_CALLBACK_VARIABLE_ASSIGNMENT, "variable_assignment", 6},
    /* INSTRUMENTR_CALLBACK_VARIABLE_REMOVAL */
    {INSTRUMENTR_CALLBACK_VARIABLE_REMOVAL, "variable_removal", 5},
    /* INSTRUMENTR_CALLBACK_VARIABLE_LOOKUP */
    {INSTRUMENTR_CALLBACK_VARIABLE_LOOKUP, "variable_lookup", 6}};

int instrumentr_callback_type_get_parameter_count(
    instrumentr_callback_type_t callback_type) {
    int index = (int) (callback_type);
    return table[index].parameter_count;
}

const char*
instrumentr_callback_type_get_name(instrumentr_callback_type_t callback_type) {
    int index = (int) (callback_type);
    return table[index].name;
}
