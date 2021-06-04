#include "event.h"
#include "interop.h"

typedef struct {
    instrumentr_event_t type;
    const char* name;
    int parameter_count;
} event_info_t;

event_info_t table[INSTRUMENTR_EVENT_COUNT] = {
    {INSTRUMENTR_EVENT_TRACING_ENTRY, "tracing_entry", 4},
    {INSTRUMENTR_EVENT_TRACING_EXIT, "tracing_exit", 4},
    {INSTRUMENTR_EVENT_PACKAGE_LOAD, "package_load", 5},
    {INSTRUMENTR_EVENT_PACKAGE_UNLOAD, "package_unload", 5},
    {INSTRUMENTR_EVENT_PACKAGE_ATTACH, "package_attach", 5},
    {INSTRUMENTR_EVENT_PACKAGE_DETACH, "package_detach", 5},
    {INSTRUMENTR_EVENT_BUILTIN_CALL_ENTRY, "builtin_call_entry", 6},
    {INSTRUMENTR_EVENT_BUILTIN_CALL_EXIT, "builtin_call_exit", 6},
    {INSTRUMENTR_EVENT_SPECIAL_CALL_ENTRY, "special_call_entry", 6},
    {INSTRUMENTR_EVENT_SPECIAL_CALL_EXIT, "special_call_exit", 6},
    {INSTRUMENTR_EVENT_CLOSURE_CALL_ENTRY, "closure_call_entry", 6},
    {INSTRUMENTR_EVENT_CLOSURE_CALL_EXIT, "closure_call_exit", 6},
    {INSTRUMENTR_EVENT_USE_METHOD_ENTRY, "use_method_entry", 6},
    {INSTRUMENTR_EVENT_USE_METHOD_EXIT, "use_method_exit", 7},
    {INSTRUMENTR_EVENT_EVAL_ENTRY, "eval_entry", 5},
    {INSTRUMENTR_EVENT_EVAL_EXIT, "eval_exit", 5},
    {INSTRUMENTR_EVENT_EVAL_CALL_ENTRY, "eval_call_entry", 6},
    {INSTRUMENTR_EVENT_EVAL_CALL_EXIT, "eval_call_exit", 7},
    {INSTRUMENTR_EVENT_SUBSTITUTE_CALL_ENTRY, "substitute_call_entry", 6},
    {INSTRUMENTR_EVENT_SUBSTITUTE_CALL_EXIT, "substitute_call_exit", 7},
    {INSTRUMENTR_EVENT_GC_ALLOCATION, "gc_allocation", 5},
    {INSTRUMENTR_EVENT_GC_DEALLOCATION, "gc_deallocation", 5},
    {INSTRUMENTR_EVENT_VALUE_FINALIZE, "value_finalize", 5},
    {INSTRUMENTR_EVENT_VARIABLE_DEFINITION, "variable_definition", 7},
    {INSTRUMENTR_EVENT_VARIABLE_ASSIGNMENT, "variable_assignment", 7},
    {INSTRUMENTR_EVENT_VARIABLE_REMOVAL, "variable_removal", 6},
    {INSTRUMENTR_EVENT_VARIABLE_EXISTS, "variable_exists", 6},
    {INSTRUMENTR_EVENT_VARIABLE_LOOKUP, "variable_lookup", 7},
    {INSTRUMENTR_EVENT_ENVIRONMENT_LS, "environment_ls", 6},
    {INSTRUMENTR_EVENT_FUNCTION_CONTEXT_LOOKUP, "function_context_lookup", 7},
    {INSTRUMENTR_EVENT_CONTEXT_ENTRY, "context_entry", 5},
    {INSTRUMENTR_EVENT_CONTEXT_EXIT, "context_exit", 5},
    {INSTRUMENTR_EVENT_PROMISE_FORCE_ENTRY, "promise_force_entry", 5},
    {INSTRUMENTR_EVENT_PROMISE_FORCE_EXIT, "promise_force_exit", 5},
    {INSTRUMENTR_EVENT_PROMISE_VALUE_LOOKUP, "promise_value_lookup", 5},
    {INSTRUMENTR_EVENT_PROMISE_EXPRESSION_LOOKUP, "promise_expression_lookup", 5},
    {INSTRUMENTR_EVENT_PROMISE_SUBSTITUTE, "promise_substitute", 5},
    {INSTRUMENTR_EVENT_PROMISE_DELAYED_ASSIGN, "promise_delayed_assign", 7},
    {INSTRUMENTR_EVENT_PROMISE_LAZY_LOAD, "promise_lazy_load", 7},
    {INSTRUMENTR_EVENT_ERROR, "error", 5},
    {INSTRUMENTR_EVENT_ATTRIBUTE_SET, "attribute_set", 7},
    {INSTRUMENTR_EVENT_SUBASSIGN, "subassign", 8},
    {INSTRUMENTR_EVENT_SUBSET, "subset", 8}};

int instrumentr_event_get_parameter_count(instrumentr_event_t event) {
    int index = (int) (event);
    return table[index].parameter_count;
}

const char* instrumentr_event_to_string(instrumentr_event_t event) {
    int index = (int) (event);
    event_info_t info = table[index];
    if (event == info.type) {
        return info.name;
    }

    instrumentr_log_error("event information table unordered");
    /* NOTE: not executed */
    return NULL;
}

SEXP instrumentr_event_wrap(instrumentr_event_t event) {
    const char* str = instrumentr_event_to_string(event);
    return instrumentr_c_string_to_r_character(str);
}

instrumentr_event_t instrumentr_event_unwrap(SEXP r_event) {
    const char* str = instrumentr_r_character_to_c_string(r_event);

    for (int i = 0; i < INSTRUMENTR_EVENT_COUNT; ++i) {
        if (!strcmp(table[i].name, str)) {
            return table[i].type;
        }
    }

    instrumentr_log_error("cannot unwrap event %s", str);
    /* NOTE: never executed */
    return INSTRUMENTR_EVENT_COUNT;
}
