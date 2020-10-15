#ifndef INSTRUMENTR_CALLBACK_TYPE_H
#define INSTRUMENTR_CALLBACK_TYPE_H

typedef enum {
    INSTRUMENTR_CALLBACK_APPLICATION_LOAD = 0,
    INSTRUMENTR_CALLBACK_APPLICATION_UNLOAD,
    INSTRUMENTR_CALLBACK_APPLICATION_ATTACH,
    INSTRUMENTR_CALLBACK_APPLICATION_DETACH,
    INSTRUMENTR_CALLBACK_PACKAGE_LOAD,
    INSTRUMENTR_CALLBACK_PACKAGE_UNLOAD,
    INSTRUMENTR_CALLBACK_PACKAGE_ATTACH,
    INSTRUMENTR_CALLBACK_PACKAGE_DETACH,
    INSTRUMENTR_CALLBACK_FUNCTION_ATTACH,
    INSTRUMENTR_CALLBACK_FUNCTION_DETACH,
    INSTRUMENTR_CALLBACK_CALL_ENTRY,
    INSTRUMENTR_CALLBACK_CALL_EXIT,
    INSTRUMENTR_CALLBACK_BUILTIN_CALL_ENTRY,
    INSTRUMENTR_CALLBACK_BUILTIN_CALL_EXIT,
    INSTRUMENTR_CALLBACK_SPECIAL_CALL_ENTRY,
    INSTRUMENTR_CALLBACK_SPECIAL_CALL_EXIT,
    INSTRUMENTR_CALLBACK_CLOSURE_CALL_ENTRY,
    INSTRUMENTR_CALLBACK_CLOSURE_CALL_EXIT,
    INSTRUMENTR_CALLBACK_EVAL_ENTRY,
    INSTRUMENTR_CALLBACK_EVAL_EXIT,
    INSTRUMENTR_CALLBACK_GC_ALLOCATION,
    INSTRUMENTR_CALLBACK_VARIABLE_DEFINITION,
    INSTRUMENTR_CALLBACK_VARIABLE_ASSIGNMENT,
    INSTRUMENTR_CALLBACK_VARIABLE_REMOVAL,
    INSTRUMENTR_CALLBACK_VARIABLE_LOOKUP
} instrumentr_callback_type_t;

#define INSTRUMENTR_CALLBACK_TYPE_MAP_MACRO(MACRO)                       \
    MACRO(INSTRUMENTR_CALLBACK_APPLICATION_LOAD, application_load)       \
    MACRO(INSTRUMENTR_CALLBACK_APPLICATION_UNLOAD, application_unload)   \
    MACRO(INSTRUMENTR_CALLBACK_APPLICATION_ATTACH, application_attach)   \
    MACRO(INSTRUMENTR_CALLBACK_APPLICATION_DETACH, application_detach)   \
    MACRO(INSTRUMENTR_CALLBACK_PACKAGE_LOAD, package_load)               \
    MACRO(INSTRUMENTR_CALLBACK_PACKAGE_UNLOAD, package_unload)           \
    MACRO(INSTRUMENTR_CALLBACK_PACKAGE_ATTACH, package_attach)           \
    MACRO(INSTRUMENTR_CALLBACK_PACKAGE_DETACH, package_detach)           \
    MACRO(INSTRUMENTR_CALLBACK_FUNCTION_ATTACH, function_attach)         \
    MACRO(INSTRUMENTR_CALLBACK_FUNCTION_DETACH, function_detach)         \
    MACRO(INSTRUMENTR_CALLBACK_CALL_ENTRY, call_entry)                   \
    MACRO(INSTRUMENTR_CALLBACK_CALL_EXIT, call_exit)                     \
    MACRO(INSTRUMENTR_CALLBACK_BUILTIN_CALL_ENTRY, builtin_call_entry)   \
    MACRO(INSTRUMENTR_CALLBACK_BUILTIN_CALL_EXIT, builtin_call_exit)     \
    MACRO(INSTRUMENTR_CALLBACK_SPECIAL_CALL_ENTRY, special_call_entry)   \
    MACRO(INSTRUMENTR_CALLBACK_SPECIAL_CALL_EXIT, special_call_exit)     \
    MACRO(INSTRUMENTR_CALLBACK_CLOSURE_CALL_ENTRY, closure_call_entry)   \
    MACRO(INSTRUMENTR_CALLBACK_CLOSURE_CALL_EXIT, closure_call_exit)     \
    MACRO(INSTRUMENTR_CALLBACK_EVAL_ENTRY, eval_entry)                   \
    MACRO(INSTRUMENTR_CALLBACK_EVAL_EXIT, eval_exit)                     \
    MACRO(INSTRUMENTR_CALLBACK_GC_ALLOCATION, gc_allocation)             \
    MACRO(INSTRUMENTR_CALLBACK_VARIABLE_DEFINITION, variable_definition) \
    MACRO(INSTRUMENTR_CALLBACK_VARIABLE_ASSIGNMENT, variable_assignment) \
    MACRO(INSTRUMENTR_CALLBACK_VARIABLE_REMOVAL, variable_removal)       \
    MACRO(INSTRUMENTR_CALLBACK_VARIABLE_LOOKUP, variable_lookup)

int instrumentr_callback_type_get_parameter_count(
    instrumentr_callback_type_t callback_type);

const char*
instrumentr_callback_type_get_name(instrumentr_callback_type_t callback_type);

#endif /* INSTRUMENTR_CALLBACK_TYPE_H */
