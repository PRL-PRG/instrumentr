#include "event.h"
#include "interop.h"

typedef struct {
    instrumentr_event_t type;
    const char* name;
    int parameter_count;
} event_info_t;

event_info_t table[] = {
    /* INSTRUMENTR_EVENT_TRACING_INITIALIZATION */
    {INSTRUMENTR_EVENT_TRACING_INITIALIZATION, "tracing_initialization", 3},
    /* INSTRUMENTR_EVENT_TRACING_FINALIZATION */
    {INSTRUMENTR_EVENT_TRACING_FINALIZATION, "tracing_finalization", 3},
    /* INSTRUMENTR_EVENT_PACKAGE_LOAD */
    {INSTRUMENTR_EVENT_PACKAGE_LOAD, "package_load", 4},
    /* INSTRUMENTR_EVENT_PACKAGE_UNLOAD */
    {INSTRUMENTR_EVENT_PACKAGE_UNLOAD, "package_unload", 4},
    /* INSTRUMENTR_EVENT_PACKAGE_ATTACH */
    {INSTRUMENTR_EVENT_PACKAGE_ATTACH, "package_attach", 4},
    /* INSTRUMENTR_EVENT_PACKAGE_DETACH */
    {INSTRUMENTR_EVENT_PACKAGE_DETACH, "package_detach", 4},
    /* INSTRUMENTR_EVENT_BUILTIN_CALL_ENTRY */
    {INSTRUMENTR_EVENT_BUILTIN_CALL_ENTRY, "builtin_call_entry", 6},
    /* INSTRUMENTR_EVENT_BUILTIN_CALL_EXIT */
    {INSTRUMENTR_EVENT_BUILTIN_CALL_EXIT, "builtin_call_exit", 6},
    /* INSTRUMENTR_EVENT_SPECIAL_CALL_ENTRY */
    {INSTRUMENTR_EVENT_SPECIAL_CALL_ENTRY, "special_call_entry", 6},
    /* INSTRUMENTR_EVENT_SPECIAL_CALL_EXIT */
    {INSTRUMENTR_EVENT_SPECIAL_CALL_EXIT, "special_call_exit", 6},
    /* INSTRUMENTR_EVENT_CLOSURE_CALL_ENTRY */
    {INSTRUMENTR_EVENT_CLOSURE_CALL_ENTRY, "closure_call_entry", 6},
    /* INSTRUMENTR_EVENT_CLOSURE_CALL_EXIT */
    {INSTRUMENTR_EVENT_CLOSURE_CALL_EXIT, "closure_call_exit", 6},
    /* INSTRUMENTR_EVENT_EVAL_ENTRY */
    {INSTRUMENTR_EVENT_EVAL_ENTRY, "eval_entry", 5},
    /* INSTRUMENTR_EVENT_EVAL_EXIT */
    {INSTRUMENTR_EVENT_EVAL_EXIT, "eval_exit", 6},
    /* INSTRUMENTR_EVENT_GC_ALLOCATION */
    {INSTRUMENTR_EVENT_GC_ALLOCATION, "gc_allocation", 4},
    /* INSTRUMENTR_EVENT_VARIABLE_DEFINITION */
    {INSTRUMENTR_EVENT_VARIABLE_DEFINITION, "variable_definition", 6},
    /* INSTRUMENTR_EVENT_VARIABLE_ASSIGNMENT */
    {INSTRUMENTR_EVENT_VARIABLE_ASSIGNMENT, "variable_assignment", 6},
    /* INSTRUMENTR_EVENT_VARIABLE_REMOVAL */
    {INSTRUMENTR_EVENT_VARIABLE_REMOVAL, "variable_removal", 5},
    /* INSTRUMENTR_EVENT_VARIABLE_LOOKUP */
    {INSTRUMENTR_EVENT_VARIABLE_LOOKUP, "variable_lookup", 6},
    /* INSTRUMENTR_EVENT_CONTEXT_ENTRY */
    {INSTRUMENTR_EVENT_CONTEXT_ENTRY, "context_entry", 4},
    /* INSTRUMENTR_EVENT_CONTEXT_EXIT */
    {INSTRUMENTR_EVENT_CONTEXT_EXIT, "context_exit", 4}
};

int instrumentr_event_get_parameter_count(
    instrumentr_event_t event) {
    int index = (int) (event);
    return table[index].parameter_count;
}

const char*
instrumentr_event_to_string(instrumentr_event_t event) {
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

    for(int i = 0; i < INSTRUMENTR_EVENT_COUNT; ++i) {
        if(!strcmp(table[i].name, str)) {
            return table[i].type;
        }
    }

    instrumentr_log_error("cannot unwrap event %s", str);
    /* NOTE: never executed */
    return -1;
}