#ifndef INSTRUMENTR_INSTRUMENTR_H
#define INSTRUMENTR_INSTRUMENTR_H

#include <instrumentr/api.h>

INSTRUMENTR_DECLARE_API()

#ifdef __cplusplus
extern "C" {
#endif

namespace instrumentr {

typedef instrumentr_object_t Object;
typedef instrumentr_tracer_t Tracer;
typedef instrumentr_callback_t Callback;
typedef instrumentr_exec_stats_t ExecStats;
typedef instrumentr_alloc_stats_t AllocStats;
typedef instrumentr_state_t State;
typedef instrumentr_model_t Model;
typedef instrumentr_application_t Application;
typedef instrumentr_call_t Call;
typedef instrumentr_call_stack_t CallStack;
typedef instrumentr_frame_t Frame;
typedef instrumentr_context_t Context;
typedef instrumentr_value_t Value;
typedef instrumentr_null_t Null;
typedef instrumentr_symbol_t Symbol;
typedef instrumentr_pairlist_t Pairlist;
typedef instrumentr_closure_t Closure;
typedef instrumentr_environment_t Environment;
typedef instrumentr_promise_t Promise;
typedef instrumentr_language_t Language;
typedef instrumentr_special_t Special;
typedef instrumentr_builtin_t Builtin;
typedef instrumentr_char_t Char;
typedef instrumentr_logical_t Logical;
typedef instrumentr_integer_t Integer;
typedef instrumentr_real_t Real;
typedef instrumentr_complex_t Complex;
typedef instrumentr_character_t Character;
typedef instrumentr_dot_t Dot;
typedef instrumentr_list_t List;
typedef instrumentr_expression_t Expression;
typedef instrumentr_bytecode_t Bytecode;
typedef instrumentr_externalptr_t Externalptr;
typedef instrumentr_weakref_t Weakref;
typedef instrumentr_raw_t Raw;
typedef instrumentr_s4_t S4;
typedef instrumentr_unbound_t Unbound;
typedef instrumentr_missing_t Missing;
typedef instrumentr_miscellaneous_t Miscellaneous;
typedef instrumentr_value_type_t ValueType;
typedef instrumentr_model_type_t ModelType;

INSTRUMENTR_DEFINE_INLINE_CPP_API()

} // namespace instrumentr

#ifdef __cplusplus
}
#endif

#endif /* INSTRUMENTR_INSTRUMENTR_H */
