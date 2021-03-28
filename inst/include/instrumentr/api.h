#ifndef INSTRUMENTR_API_H
#define INSTRUMENTR_API_H

#include <instrumentr/Rincludes.h>
#include <instrumentr/types.h>
#include <vector>
#include <string>

#define INSTRUMENTR_MODEL_DECLARE_DERIVED_C_API(                              \
    MACRO, MODEL_TYPE, MODEL_NAME, MODEL_VAR)                                 \
    MACRO(instrumentr_##MODEL_NAME##_wrap,                                    \
          SEXP,                                                               \
          instrumentr_##MODEL_NAME##_t MODEL_NAME##_var)                      \
    MACRO(instrumentr_##MODEL_NAME##_unwrap,                                  \
          instrumentr_##MODEL_NAME##_t,                                       \
          SEXP r_##MODEL_NAME)                                                \
    MACRO(instrumentr_##MODEL_NAME##_acquire,                                 \
          int,                                                                \
          instrumentr_##MODEL_NAME##_t MODEL_NAME##_var)                      \
    MACRO(instrumentr_##MODEL_NAME##_release,                                 \
          int,                                                                \
          instrumentr_##MODEL_NAME##_t MODEL_NAME##_var)                      \
    MACRO(instrumentr_##MODEL_NAME##_kill,                                    \
          int,                                                                \
          instrumentr_##MODEL_NAME##_t MODEL_NAME##_var)                      \
    MACRO(instrumentr_##MODEL_NAME##_get_reference_count,                     \
          int,                                                                \
          instrumentr_##MODEL_NAME##_t MODEL_NAME##_var)                      \
    MACRO(instrumentr_##MODEL_NAME##_get_id,                                  \
          instrumentr_id_t,                                                   \
          instrumentr_##MODEL_NAME##_t MODEL_NAME##_var)                      \
    MACRO(instrumentr_model_is_##MODEL_NAME, bool, instrumentr_model_t model) \
    MACRO(instrumentr_model_as_##MODEL_NAME,                                  \
          instrumentr_##MODEL_NAME##_t,                                       \
          instrumentr_model_t model)                                          \
    MACRO(instrumentr_##MODEL_NAME##_as_model,                                \
          instrumentr_model_t,                                                \
          instrumentr_##MODEL_NAME##_t MODEL_NAME##_var)                      \
    MACRO(instrumentr_##MODEL_NAME##_get_birth_time,                          \
          int,                                                                \
          instrumentr_##MODEL_NAME##_t MODEL_NAME##_var)                      \
    MACRO(instrumentr_##MODEL_NAME##_get_death_time,                          \
          int,                                                                \
          instrumentr_##MODEL_NAME##_t MODEL_NAME##_var)                      \
    MACRO(instrumentr_##MODEL_NAME##_get_life_time,                           \
          int,                                                                \
          instrumentr_##MODEL_NAME##_t MODEL_NAME##_var)                      \
    MACRO(instrumentr_##MODEL_NAME##_is_alive,                                \
          int,                                                                \
          instrumentr_##MODEL_NAME##_t MODEL_NAME##_var)                      \
    MACRO(instrumentr_##MODEL_NAME##_is_dead,                                 \
          int,                                                                \
          instrumentr_##MODEL_NAME##_t MODEL_NAME##_var)                      \
    MACRO(instrumentr_##MODEL_NAME##_is_local,                                \
          int,                                                                \
          instrumentr_##MODEL_NAME##_t MODEL_NAME##_var)                      \
    MACRO(instrumentr_##MODEL_NAME##_is_foreign,                              \
          int,                                                                \
          instrumentr_##MODEL_NAME##_t MODEL_NAME##_var)                      \
    MACRO(instrumentr_##MODEL_NAME##_get_state,                               \
          instrumentr_state_t,                                                \
          instrumentr_##MODEL_NAME##_t MODEL_NAME##_var)

#define INSTRUMENTR_MODEL_DECLARE_DERIVED_R_API( \
    MACRO, MODEL_TYPE, MODEL_NAME, MODEL_VAR)

#define INSTRUMENTR_MODEL_DECLARE_DERIVED_API(                                 \
    MODEL_TYPE, MODEL_NAME, MODEL_VAR)                                         \
    INSTRUMENTR_MODEL_DECLARE_DERIVED_R_API(MODEL_TYPE, MODEL_NAME, MODEL_VAR) \
    INSTRUMENTR_MODEL_DECLARE_DERIVED_C_API(MODEL_TYPE, MODEL_NAME, MODEL_VAR)

#define INSTRUMENTR_VALUE_DECLARE_C_API(                                      \
    MACRO, VALUE_TYPE, VALUE_NAME, VALUE_VAR)                                 \
    MACRO(instrumentr_##VALUE_NAME##_wrap,                                    \
          SEXP,                                                               \
          instrumentr_##VALUE_NAME##_t VALUE_NAME##_var)                      \
    MACRO(instrumentr_##VALUE_NAME##_unwrap,                                  \
          instrumentr_##VALUE_NAME##_t,                                       \
          SEXP r_##VALUE_NAME)                                                \
    MACRO(instrumentr_##VALUE_NAME##_acquire,                                 \
          int,                                                                \
          instrumentr_##VALUE_NAME##_t VALUE_NAME##_var)                      \
    MACRO(instrumentr_##VALUE_NAME##_release,                                 \
          int,                                                                \
          instrumentr_##VALUE_NAME##_t VALUE_NAME##_var)                      \
    MACRO(instrumentr_##VALUE_NAME##_kill,                                    \
          int,                                                                \
          instrumentr_##VALUE_NAME##_t VALUE_NAME##_var)                      \
    MACRO(instrumentr_##VALUE_NAME##_get_reference_count,                     \
          int,                                                                \
          instrumentr_##VALUE_NAME##_t VALUE_NAME##_var)                      \
    MACRO(instrumentr_##VALUE_NAME##_get_id,                                  \
          instrumentr_id_t,                                                   \
          instrumentr_##VALUE_NAME##_t VALUE_NAME##_var)                      \
    MACRO(instrumentr_value_is_##VALUE_NAME, bool, instrumentr_value_t value) \
    MACRO(instrumentr_value_as_##VALUE_NAME,                                  \
          instrumentr_##VALUE_NAME##_t,                                       \
          instrumentr_value_t value)                                          \
    MACRO(instrumentr_##VALUE_NAME##_as_value,                                \
          instrumentr_value_t,                                                \
          instrumentr_##VALUE_NAME##_t VALUE_NAME##_var)                      \
    MACRO(instrumentr_##VALUE_NAME##_get_birth_time,                          \
          int,                                                                \
          instrumentr_##VALUE_NAME##_t VALUE_NAME##_var)                      \
    MACRO(instrumentr_##VALUE_NAME##_get_death_time,                          \
          int,                                                                \
          instrumentr_##VALUE_NAME##_t VALUE_NAME##_var)                      \
    MACRO(instrumentr_##VALUE_NAME##_get_life_time,                           \
          int,                                                                \
          instrumentr_##VALUE_NAME##_t VALUE_NAME##_var)                      \
    MACRO(instrumentr_##VALUE_NAME##_is_alive,                                \
          int,                                                                \
          instrumentr_##VALUE_NAME##_t VALUE_NAME##_var)                      \
    MACRO(instrumentr_##VALUE_NAME##_is_dead,                                 \
          int,                                                                \
          instrumentr_##VALUE_NAME##_t VALUE_NAME##_var)                      \
    MACRO(instrumentr_##VALUE_NAME##_is_local,                                \
          int,                                                                \
          instrumentr_##VALUE_NAME##_t VALUE_NAME##_var)                      \
    MACRO(instrumentr_##VALUE_NAME##_is_foreign,                              \
          int,                                                                \
          instrumentr_##VALUE_NAME##_t VALUE_NAME##_var)                      \
    MACRO(instrumentr_##VALUE_NAME##_get_state,                               \
          instrumentr_state_t,                                                \
          instrumentr_##VALUE_NAME##_t VALUE_NAME##_var)                      \
    MACRO(instrumentr_##VALUE_NAME##_get_sexp,                                \
          SEXP,                                                               \
          instrumentr_##VALUE_NAME##_t VALUE_NAME##_var)

#define INSTRUMENTR_VALUE_DECLARE_R_API(                                       \
    MACRO, VALUE_TYPE, VALUE_NAME, VALUE_VAR)                                  \
    MACRO(r_instrumentr_##VALUE_NAME##_get_reference_count,                    \
          SEXP,                                                                \
          SEXP r_##VALUE_NAME##_var)                                           \
    MACRO(                                                                     \
        r_instrumentr_##VALUE_NAME##_get_id, SEXP, SEXP r_##VALUE_NAME##_var)  \
    MACRO(r_instrumentr_##VALUE_NAME##_get_birth_time,                         \
          SEXP,                                                                \
          SEXP r_##VALUE_NAME##_var)                                           \
    MACRO(r_instrumentr_##VALUE_NAME##_get_death_time,                         \
          SEXP,                                                                \
          SEXP r_##VALUE_NAME##_var)                                           \
    MACRO(r_instrumentr_##VALUE_NAME##_get_life_time,                          \
          SEXP,                                                                \
          SEXP r_##VALUE_NAME##_var)                                           \
    MACRO(r_instrumentr_##VALUE_NAME##_is_alive,                               \
          SEXP,                                                                \
          SEXP r_##VALUE_NAME##_var)                                           \
    MACRO(                                                                     \
        r_instrumentr_##VALUE_NAME##_is_dead, SEXP, SEXP r_##VALUE_NAME##_var) \
    MACRO(r_instrumentr_##VALUE_NAME##_is_local,                               \
          SEXP,                                                                \
          SEXP r_##VALUE_NAME##_var)                                           \
    MACRO(r_instrumentr_##VALUE_NAME##_is_foreign,                             \
          SEXP,                                                                \
          SEXP r_##VALUE_NAME##_var)                                           \
    MACRO(r_instrumentr_##VALUE_NAME##_get_sexp,                               \
          SEXP,                                                                \
          SEXP r_##VALUE_NAME##_var)

#define INSTRUMENTR_VALUE_DECLARE_API(VALUE_TYPE, VALUE_NAME, VALUE_VAR) \
    INSTRUMENTR_VALUE_DECLARE_R_API(VALUE_TYPE, VALUE_NAME, VALUE_VAR)   \
    INSTRUMENTR_VALUE_DECLARE_C_API(VALUE_TYPE, VALUE_NAME, VALUE_VAR)

/********************************************************************************
 interop
 *******************************************************************************/

#define INSTRUMENTR_INTEROP_C_API_MAP(MACRO)                                  \
    MACRO(instrumentr_log_error, void, const char* fmt, ...)                  \
    MACRO(instrumentr_log_warning, void, const char* fmt, ...)                \
    MACRO(instrumentr_log_message, void, const char* fmt, ...)                \
    MACRO(instrumentr_sexp_acquire, void, SEXP r_object)                      \
    MACRO(instrumentr_sexp_release, void, SEXP r_object)                      \
    MACRO(instrumentr_sexp_set_class, SEXP, SEXP r_object, SEXP r_class)      \
    MACRO(instrumentr_c_int_to_r_logical, SEXP, int value)                    \
    MACRO(instrumentr_r_logical_to_c_int, int, SEXP r_value)                  \
    MACRO(instrumentr_c_int_to_r_integer, SEXP, int value)                    \
    MACRO(instrumentr_r_integer_to_c_int, int, SEXP r_value)                  \
    MACRO(instrumentr_c_double_to_r_double, SEXP, double value)               \
    MACRO(instrumentr_r_double_to_c_double, double, SEXP r_value)             \
    MACRO(instrumentr_c_complex_to_r_complex, SEXP, Rcomplex value)           \
    MACRO(instrumentr_r_complex_to_c_complex, Rcomplex, SEXP r_value)         \
    MACRO(instrumentr_c_raw_to_r_raw, SEXP, Rbyte value)                      \
    MACRO(instrumentr_r_raw_to_c_raw, Rbyte, SEXP r_value)                    \
    MACRO(instrumentr_c_string_to_r_character, SEXP, const char* string)      \
    MACRO(instrumentr_r_character_to_c_string, const char*, SEXP r_character) \
    MACRO(instrumentr_c_pointer_to_r_externalptr,                             \
          SEXP,                                                               \
          void* pointer,                                                      \
          SEXP r_tag,                                                         \
          SEXP r_prot,                                                        \
          R_CFinalizer_t finalizer)                                           \
    MACRO(instrumentr_r_externalptr_to_c_pointer, void*, SEXP r_pointer)      \
    MACRO(instrumentr_r_externalptr_clear, void, SEXP r_externalptr)          \
    MACRO(instrumentr_create_list, SEXP, int column_count, ...)               \
    MACRO(instrumentr_create_data_frame, SEXP, int column_count, ...)         \
    MACRO(instrumentr_sexp_to_string,                                         \
          std::vector<std::string>,                                           \
          SEXP r_expr,                                                        \
          bool collapse)                                                      \
    MACRO(instrumentr_string_encode, std::string, const std::string& src)     \
    MACRO(instrumentr_string_decode, std::string, const std::string& src)     \
    MACRO(instrumentr_compute_hash, std::string, const std::string& src)

#define INSTRUMENTR_INTEROP_R_API_MAP(MACRO)             \
    MACRO(r_instrumentr_get_object_details,              \
          SEXP,                                          \
          SEXP r_value,                                  \
          SEXP r_variable,                               \
          SEXP r_environment,                            \
          SEXP r_peek)                                   \
    MACRO(r_instrumentr_string_encode, SEXP, SEXP r_src) \
    MACRO(r_instrumentr_string_decode, SEXP, SEXP r_src) \
    MACRO(r_instrumentr_compute_hash, SEXP, SEXP r_src, SEXP r_collapse)

#define INSTRUMENTR_INTEROP_API_MAP(MACRO) \
    INSTRUMENTR_INTEROP_C_API_MAP(MACRO)   \
    INSTRUMENTR_INTEROP_R_API_MAP(MACRO)

/********************************************************************************
 OBJECT API
 *******************************************************************************/

#define INSTRUMENTR_OBJECT_C_API_MAP(MACRO)              \
    MACRO(instrumentr_object_acquire, int, void* object) \
    MACRO(instrumentr_object_release, int, void* object) \
    MACRO(instrumentr_object_get_reference_count, int, void* object)

#define INSTRUMENTR_OBJECT_R_API_MAP(MACRO) \
    MACRO(r_instrumentr_object_get_reference_count, SEXP, SEXP r_object)

#define INSTRUMENTR_OBJECT_API_MAP(MACRO) \
    INSTRUMENTR_OBJECT_C_API_MAP(MACRO)   \
    INSTRUMENTR_OBJECT_R_API_MAP(MACRO)

/********************************************************************************
 TRACER API
 *******************************************************************************/

#define INSTRUMENTR_TRACER_C_API_MAP(MACRO)                                    \
    MACRO(instrumentr_tracer_create, instrumentr_tracer_t)                     \
    MACRO(instrumentr_tracer_wrap, SEXP, instrumentr_tracer_t tracer)          \
    MACRO(instrumentr_tracer_unwrap, instrumentr_tracer_t, SEXP r_tracer)      \
    MACRO(instrumentr_tracer_get_state,                                        \
          instrumentr_state_t,                                                 \
          instrumentr_tracer_t tracer)                                         \
    MACRO(instrumentr_tracer_get_application,                                  \
          instrumentr_application_t,                                           \
          instrumentr_tracer_t tracer)                                         \
    MACRO(                                                                     \
        instrumentr_tracer_get_environment, SEXP, instrumentr_tracer_t tracer) \
    MACRO(instrumentr_tracer_is_active, int, instrumentr_tracer_t tracer)      \
    MACRO(instrumentr_tracer_get_active_callback,                              \
          instrumentr_callback_t,                                              \
          instrumentr_tracer_t tracer)                                         \
    MACRO(instrumentr_tracer_is_enabled, int, instrumentr_tracer_t tracer)     \
    MACRO(instrumentr_tracer_enable, void, instrumentr_tracer_t tracer)        \
    MACRO(instrumentr_tracer_disable, void, instrumentr_tracer_t tracer)       \
    MACRO(instrumentr_tracer_reinstate, void, instrumentr_tracer_t tracer)     \
    MACRO(instrumentr_tracer_has_callback,                                     \
          int,                                                                 \
          instrumentr_tracer_t tracer,                                         \
          instrumentr_event_t event)                                           \
    MACRO(instrumentr_tracer_get_callback,                                     \
          instrumentr_callback_t,                                              \
          instrumentr_tracer_t tracer,                                         \
          instrumentr_event_t event)                                           \
    MACRO(instrumentr_tracer_set_callback,                                     \
          void,                                                                \
          instrumentr_tracer_t tracer,                                         \
          instrumentr_callback_t callback)                                     \
    MACRO(instrumentr_tracer_remove_callback,                                  \
          void,                                                                \
          instrumentr_tracer_t tracer,                                         \
          instrumentr_event_t event)

#define INSTRUMENTR_TRACER_R_API_MAP(MACRO)                                   \
    MACRO(r_instrumentr_tracer_create, SEXP)                                  \
    MACRO(r_instrumentr_tracer_get_state, SEXP, SEXP r_tracer)                \
    MACRO(r_instrumentr_tracer_get_application, SEXP, SEXP r_tracer)          \
    MACRO(r_instrumentr_tracer_get_environment, SEXP, SEXP r_tracer)          \
    MACRO(r_instrumentr_tracer_is_active, SEXP, SEXP r_tracer)                \
    MACRO(r_instrumentr_tracer_get_active_callback, SEXP, SEXP r_tracer)      \
    MACRO(r_instrumentr_tracer_is_enabled, SEXP, SEXP r_tracer)               \
    MACRO(r_instrumentr_tracer_enable, SEXP, SEXP r_tracer)                   \
    MACRO(r_instrumentr_tracer_disable, SEXP, SEXP r_tracer)                  \
    MACRO(r_instrumentr_tracer_reinstate, SEXP, SEXP r_tracer)                \
    MACRO(                                                                    \
        r_instrumentr_tracer_has_callback, SEXP, SEXP r_tracer, SEXP r_event) \
    MACRO(                                                                    \
        r_instrumentr_tracer_get_callback, SEXP, SEXP r_tracer, SEXP r_event) \
    MACRO(r_instrumentr_tracer_set_callback,                                  \
          SEXP,                                                               \
          SEXP r_tracer,                                                      \
          SEXP r_callback)                                                    \
    MACRO(r_instrumentr_tracer_remove_callback,                               \
          SEXP,                                                               \
          SEXP r_tracer,                                                      \
          SEXP r_event)

#define INSTRUMENTR_TRACER_API_MAP(MACRO) \
    INSTRUMENTR_TRACER_C_API_MAP(MACRO)   \
    INSTRUMENTR_TRACER_R_API_MAP(MACRO)

/********************************************************************************
 CALLBACK API
 *******************************************************************************/

#define INSTRUMENTR_CALLBACK_C_API_MAP(MACRO)                               \
    MACRO(instrumentr_callback_wrap, SEXP, instrumentr_callback_t callback) \
    MACRO(instrumentr_callback_create_from_r_function,                      \
          instrumentr_callback_t,                                           \
          SEXP r_function,                                                  \
          instrumentr_event_t event)                                        \
    MACRO(instrumentr_callback_create_from_c_function,                      \
          instrumentr_callback_t,                                           \
          void* c_function,                                                 \
          instrumentr_event_t event)                                        \
    MACRO(instrumentr_callback_get_event,                                   \
          instrumentr_event_t,                                              \
          instrumentr_callback_t callback)                                  \
    MACRO(instrumentr_callback_handles_event,                               \
          bool,                                                             \
          instrumentr_callback_t callback,                                  \
          instrumentr_event_t event)                                        \
    MACRO(instrumentr_callback_get_parameter_count,                         \
          int,                                                              \
          instrumentr_callback_t callback)                                  \
    MACRO(instrumentr_callback_has_r_function,                              \
          int,                                                              \
          instrumentr_callback_t callback)                                  \
    MACRO(instrumentr_callback_get_r_function,                              \
          SEXP,                                                             \
          instrumentr_callback_t callback)                                  \
    MACRO(instrumentr_callback_has_c_function,                              \
          int,                                                              \
          instrumentr_callback_t callback)                                  \
    MACRO(instrumentr_callback_get_c_function,                              \
          void*,                                                            \
          instrumentr_callback_t callback)                                  \
    MACRO(instrumentr_callback_is_active, int, instrumentr_callback_t callback)

#define INSTRUMENTR_CALLBACK_R_API_MAP(MACRO)                                \
    MACRO(r_instrumentr_callback_create_from_r_function,                     \
          SEXP,                                                              \
          SEXP r_event,                                                      \
          SEXP r_function)                                                   \
    MACRO(r_instrumentr_callback_create_from_c_function,                     \
          SEXP,                                                              \
          SEXP r_c_function,                                                 \
          SEXP r_event)                                                      \
    MACRO(r_instrumentr_callback_get_event, SEXP, SEXP r_callback)           \
    MACRO(r_instrumentr_callback_handles_event,                              \
          SEXP,                                                              \
          SEXP r_callback,                                                   \
          SEXP r_event)                                                      \
    MACRO(r_instrumentr_callback_get_parameter_count, SEXP, SEXP r_callback) \
    MACRO(r_instrumentr_callback_has_r_function, SEXP, SEXP r_callback)      \
    MACRO(r_instrumentr_callback_get_r_function, SEXP, SEXP r_callback)      \
    MACRO(r_instrumentr_callback_has_c_function, SEXP, SEXP r_callback)      \
    MACRO(r_instrumentr_callback_get_c_function, SEXP, SEXP r_callback)      \
    MACRO(r_instrumentr_callback_is_active, SEXP, SEXP r_callback)

#define INSTRUMENTR_CALLBACK_API_MAP(MACRO) \
    INSTRUMENTR_CALLBACK_C_API_MAP(MACRO)   \
    INSTRUMENTR_CALLBACK_R_API_MAP(MACRO)

/******************************************************************************
 EXEC_STATS API
*******************************************************************************/

#define INSTRUMENTR_EXEC_STATS_C_API_MAP(MACRO)       \
    MACRO(instrumentr_exec_stats_get_minimum_time,    \
          double,                                     \
          instrumentr_exec_stats_t exec_stats,        \
          instrumentr_event_t event)                  \
    MACRO(instrumentr_exec_stats_get_maximum_time,    \
          double,                                     \
          instrumentr_exec_stats_t exec_stats,        \
          instrumentr_event_t event)                  \
    MACRO(instrumentr_exec_stats_get_average_time,    \
          double,                                     \
          instrumentr_exec_stats_t exec_stats,        \
          instrumentr_event_t event)                  \
    MACRO(instrumentr_exec_stats_get_total_time,      \
          double,                                     \
          instrumentr_exec_stats_t exec_stats,        \
          instrumentr_event_t event)                  \
    MACRO(instrumentr_exec_stats_get_execution_count, \
          int,                                        \
          instrumentr_exec_stats_t exec_stats,        \
          instrumentr_event_t event)                  \
    MACRO(instrumentr_exec_stats_as_data_frame,       \
          SEXP,                                       \
          instrumentr_exec_stats_t exec_stats)

#define INSTRUMENTR_EXEC_STATS_R_API_MAP(MACRO)         \
    MACRO(r_instrumentr_exec_stats_get_minimum_time,    \
          SEXP,                                         \
          SEXP r_exec_stats,                            \
          SEXP r_event)                                 \
    MACRO(r_instrumentr_exec_stats_get_maximum_time,    \
          SEXP,                                         \
          SEXP r_exec_stats,                            \
          SEXP r_event)                                 \
    MACRO(r_instrumentr_exec_stats_get_average_time,    \
          SEXP,                                         \
          SEXP r_exec_stats,                            \
          SEXP r_event)                                 \
    MACRO(r_instrumentr_exec_stats_get_total_time,      \
          SEXP,                                         \
          SEXP r_exec_stats,                            \
          SEXP r_event)                                 \
    MACRO(r_instrumentr_exec_stats_get_execution_count, \
          SEXP,                                         \
          SEXP r_exec_stats,                            \
          SEXP r_event)

#define INSTRUMENTR_EXEC_STATS_API_MAP(MACRO) \
    INSTRUMENTR_EXEC_STATS_C_API_MAP(MACRO)   \
    INSTRUMENTR_EXEC_STATS_R_API_MAP(MACRO)

/******************************************************************************
 ALLOC_STATS API
*******************************************************************************/

#define INSTRUMENTR_ALLOC_STATS_C_API_MAP(MACRO)         \
    MACRO(instrumentr_alloc_stats_get_value_size,        \
          int,                                           \
          instrumentr_alloc_stats_t alloc_stats,         \
          instrumentr_value_type_t value_type)           \
    MACRO(instrumentr_alloc_stats_get_allocated_count,   \
          int,                                           \
          instrumentr_alloc_stats_t alloc_stats,         \
          instrumentr_value_type_t value_type)           \
    MACRO(instrumentr_alloc_stats_get_deallocated_count, \
          int,                                           \
          instrumentr_alloc_stats_t alloc_stats,         \
          instrumentr_value_type_t value_type)           \
    MACRO(instrumentr_alloc_stats_get_finalized_count,   \
          int,                                           \
          instrumentr_alloc_stats_t alloc_stats,         \
          instrumentr_value_type_t value_type)           \
    MACRO(instrumentr_alloc_stats_get_zombie_count,      \
          int,                                           \
          instrumentr_alloc_stats_t alloc_stats,         \
          instrumentr_value_type_t value_type)           \
    MACRO(instrumentr_alloc_stats_get_max_alive_count,   \
          int,                                           \
          instrumentr_alloc_stats_t alloc_stats,         \
          instrumentr_value_type_t value_type)           \
    MACRO(instrumentr_alloc_stats_as_data_frame,         \
          SEXP,                                          \
          instrumentr_alloc_stats_t alloc_stats)

#define INSTRUMENTR_ALLOC_STATS_R_API_MAP(MACRO)

#define INSTRUMENTR_ALLOC_STATS_API_MAP(MACRO) \
    INSTRUMENTR_ALLOC_STATS_C_API_MAP(MACRO)   \
    INSTRUMENTR_ALLOC_STATS_R_API_MAP(MACRO)

/******************************************************************************
 STATE API
*******************************************************************************/

#define INSTRUMENTR_STATE_C_API_MAP(MACRO)                             \
    MACRO(instrumentr_state_wrap, SEXP, instrumentr_state_t state)     \
    MACRO(instrumentr_state_unwrap, instrumentr_state_t, SEXP r_state) \
    MACRO(instrumentr_state_get_time, int, instrumentr_state_t state)  \
    MACRO(instrumentr_state_get_alloc_stats,                           \
          instrumentr_alloc_stats_t,                                   \
          instrumentr_state_t state)                                   \
    MACRO(instrumentr_state_get_exec_stats,                            \
          instrumentr_exec_stats_t,                                    \
          instrumentr_state_t state)                                   \
    MACRO(instrumentr_state_is_empty, int, instrumentr_state_t state)  \
    MACRO(instrumentr_state_get_size, int, instrumentr_state_t state)  \
    MACRO(instrumentr_state_clear, void, instrumentr_state_t state)    \
    MACRO(instrumentr_state_has_key,                                   \
          int,                                                         \
          instrumentr_state_t state,                                   \
          const char* key)                                             \
    MACRO(instrumentr_state_as_list, SEXP, instrumentr_state_t state)  \
    MACRO(instrumentr_state_lookup,                                    \
          SEXP,                                                        \
          instrumentr_state_t state,                                   \
          const char* key,                                             \
          SEXP r_alternative)                                          \
    MACRO(instrumentr_state_insert,                                    \
          void,                                                        \
          instrumentr_state_t state,                                   \
          const char* key,                                             \
          SEXP r_value,                                                \
          int overwrite)                                               \
    MACRO(instrumentr_state_erase,                                     \
          void,                                                        \
          instrumentr_state_t state,                                   \
          const char* key,                                             \
          int permissive)                                              \
    MACRO(instrumentr_state_get_call_stack,                            \
          instrumentr_call_stack_t,                                    \
          instrumentr_state_t state)                                   \
    MACRO(instrumentr_state_get_packages,                              \
          std::vector<instrumentr_environment_t>,                      \
          instrumentr_state_t state)                                   \
    MACRO(instrumentr_state_get_namespaces,                            \
          std::vector<instrumentr_environment_t>,                      \
          instrumentr_state_t state)

#define INSTRUMENTR_STATE_R_API_MAP(MACRO)                             \
    MACRO(r_instrumentr_state_get_time, SEXP, SEXP r_state)            \
    MACRO(r_instrumentr_state_get_alloc_stats, SEXP, SEXP r_state)     \
    MACRO(r_instrumentr_state_get_exec_stats, SEXP, SEXP r_state)      \
    MACRO(r_instrumentr_state_is_empty, SEXP, SEXP r_state)            \
    MACRO(r_instrumentr_state_get_size, SEXP, SEXP r_state)            \
    MACRO(r_instrumentr_state_clear, SEXP, SEXP r_state)               \
    MACRO(r_instrumentr_state_has_key, SEXP, SEXP r_state, SEXP r_key) \
    MACRO(r_instrumentr_state_as_list, SEXP, SEXP r_state)             \
    MACRO(r_instrumentr_state_lookup,                                  \
          SEXP,                                                        \
          SEXP r_state,                                                \
          SEXP r_key,                                                  \
          SEXP r_alternative)                                          \
    MACRO(r_instrumentr_state_insert,                                  \
          SEXP,                                                        \
          SEXP r_state,                                                \
          SEXP r_key,                                                  \
          SEXP r_value,                                                \
          SEXP r_overwrite)                                            \
    MACRO(r_instrumentr_state_erase,                                   \
          SEXP,                                                        \
          SEXP r_state,                                                \
          SEXP r_key,                                                  \
          SEXP r_permissive)                                           \
    MACRO(r_instrumentr_state_get_call_stack, SEXP, SEXP r_state)      \
    MACRO(r_instrumentr_state_get_packages, SEXP, SEXP r_state)        \
    MACRO(r_instrumentr_state_get_namespaces, SEXP, SEXP r_state)

#define INSTRUMENTR_STATE_API_MAP(MACRO) \
    INSTRUMENTR_STATE_C_API_MAP(MACRO)   \
    INSTRUMENTR_STATE_R_API_MAP(MACRO)

/******************************************************************************
 MODEL API
*******************************************************************************/

#define INSTRUMENTR_MODEL_C_API_MAP(MACRO)                                     \
    MACRO(                                                                     \
        instrumentr_model_get_id, instrumentr_id_t, instrumentr_model_t model) \
    MACRO(                                                                     \
        instrumentr_model_get_reference_count, int, instrumentr_model_t model) \
    MACRO(instrumentr_model_get_birth_time, int, instrumentr_model_t model)    \
    MACRO(instrumentr_model_get_death_time, int, instrumentr_model_t model)    \
    MACRO(instrumentr_model_get_life_time, int, instrumentr_model_t model)     \
    MACRO(instrumentr_model_is_alive, int, instrumentr_model_t model)          \
    MACRO(instrumentr_model_is_dead, int, instrumentr_model_t model)           \
    MACRO(instrumentr_model_is_local, int, instrumentr_model_t model)          \
    MACRO(instrumentr_model_is_foreign, int, instrumentr_model_t model)

#define INSTRUMENTR_MODEL_R_API_MAP(MACRO)                             \
    MACRO(r_instrumentr_model_get_id, SEXP, SEXP r_model)              \
    MACRO(r_instrumentr_model_get_reference_count, SEXP, SEXP r_model) \
    MACRO(r_instrumentr_model_get_birth_time, SEXP, SEXP r_model)      \
    MACRO(r_instrumentr_model_get_death_time, SEXP, SEXP r_model)      \
    MACRO(r_instrumentr_model_get_life_time, SEXP, SEXP r_model)       \
    MACRO(r_instrumentr_model_is_alive, SEXP, SEXP r_model)            \
    MACRO(r_instrumentr_model_is_dead, SEXP, SEXP r_model)             \
    MACRO(r_instrumentr_model_is_local, SEXP, SEXP r_model)            \
    MACRO(r_instrumentr_model_is_foreign, SEXP, SEXP r_model)

#define INSTRUMENTR_MODEL_API_MAP(MACRO) \
    INSTRUMENTR_MODEL_C_API_MAP(MACRO)   \
    INSTRUMENTR_MODEL_R_API_MAP(MACRO)

/********************************************************************************
 APPLICATION API
 *******************************************************************************/

#define INSTRUMENTR_APPLICATION_C_API_MAP(MACRO)   \
    MACRO(instrumentr_application_get_directory,   \
          const char*,                             \
          instrumentr_application_t application)   \
    MACRO(instrumentr_application_get_code,        \
          SEXP,                                    \
          instrumentr_application_t application)   \
    MACRO(instrumentr_application_get_environment, \
          instrumentr_environment_t,               \
          instrumentr_application_t application)   \
    INSTRUMENTR_MODEL_DECLARE_DERIVED_C_API(       \
        MACRO, INSTRUMENTR_MODEL_TYPE_APPLICATION, application, application)

#define INSTRUMENTR_APPLICATION_R_API_MAP(MACRO)                               \
    MACRO(r_instrumentr_application_get_directory, SEXP, SEXP r_application)   \
    MACRO(r_instrumentr_application_get_code, SEXP, SEXP r_application)        \
    MACRO(r_instrumentr_application_get_environment, SEXP, SEXP r_application) \
    INSTRUMENTR_MODEL_DECLARE_DERIVED_R_API(                                   \
        MACRO, INSTRUMENTR_MODEL_TYPE_APPLICATION, application, application)

#define INSTRUMENTR_APPLICATION_API_MAP(MACRO) \
    INSTRUMENTR_APPLICATION_C_API_MAP(MACRO)   \
    INSTRUMENTR_APPLICATION_R_API_MAP(MACRO)

/********************************************************************************
 UNBOUND API
*******************************************************************************/

#define INSTRUMENTR_UNBOUND_C_API_MAP(MACRO) \
    INSTRUMENTR_VALUE_DECLARE_C_API(         \
        MACRO, INSTRUMENTR_VALUE_TYPE_UNBOUND, unbound, unboundval)

#define INSTRUMENTR_UNBOUND_R_API_MAP(MACRO) \
    INSTRUMENTR_VALUE_DECLARE_R_API(         \
        MACRO, INSTRUMENTR_VALUE_TYPE_UNBOUND, unbound, unboundval)

#define INSTRUMENTR_UNBOUND_API_MAP(MACRO) \
    INSTRUMENTR_UNBOUND_C_API_MAP(MACRO)   \
    INSTRUMENTR_UNBOUND_R_API_MAP(MACRO)

/********************************************************************************
 MISSING API
*******************************************************************************/

#define INSTRUMENTR_MISSING_C_API_MAP(MACRO) \
    INSTRUMENTR_VALUE_DECLARE_C_API(         \
        MACRO, INSTRUMENTR_VALUE_TYPE_MISSING, missing, missingval)

#define INSTRUMENTR_MISSING_R_API_MAP(MACRO) \
    INSTRUMENTR_VALUE_DECLARE_R_API(         \
        MACRO, INSTRUMENTR_VALUE_TYPE_MISSING, missing, missingval)

#define INSTRUMENTR_MISSING_API_MAP(MACRO) \
    INSTRUMENTR_MISSING_C_API_MAP(MACRO)   \
    INSTRUMENTR_MISSING_R_API_MAP(MACRO)

/********************************************************************************
 NULL API
*******************************************************************************/

#define INSTRUMENTR_NULL_C_API_MAP(MACRO) \
    INSTRUMENTR_VALUE_DECLARE_C_API(      \
        MACRO, INSTRUMENTR_VALUE_TYPE_NULL, null, nullval)

#define INSTRUMENTR_NULL_R_API_MAP(MACRO) \
    INSTRUMENTR_VALUE_DECLARE_R_API(      \
        MACRO, INSTRUMENTR_VALUE_TYPE_NULL, null, nullval)

#define INSTRUMENTR_NULL_API_MAP(MACRO) \
    INSTRUMENTR_NULL_C_API_MAP(MACRO)   \
    INSTRUMENTR_NULL_R_API_MAP(MACRO)

/********************************************************************************
 EXTERNALPTR API
*******************************************************************************/

#define INSTRUMENTR_EXTERNALPTR_C_API_MAP(MACRO)                        \
    INSTRUMENTR_VALUE_DECLARE_C_API(MACRO,                              \
                                    INSTRUMENTR_VALUE_TYPE_EXTERNALPTR, \
                                    externalptr,                        \
                                    externalptrval)

#define INSTRUMENTR_EXTERNALPTR_R_API_MAP(MACRO)                        \
    INSTRUMENTR_VALUE_DECLARE_R_API(MACRO,                              \
                                    INSTRUMENTR_VALUE_TYPE_EXTERNALPTR, \
                                    externalptr,                        \
                                    externalptrval)

#define INSTRUMENTR_EXTERNALPTR_API_MAP(MACRO) \
    INSTRUMENTR_EXTERNALPTR_C_API_MAP(MACRO)   \
    INSTRUMENTR_EXTERNALPTR_R_API_MAP(MACRO)

/********************************************************************************
 WEAKREF API
*******************************************************************************/

#define INSTRUMENTR_WEAKREF_C_API_MAP(MACRO) \
    INSTRUMENTR_VALUE_DECLARE_C_API(         \
        MACRO, INSTRUMENTR_VALUE_TYPE_WEAKREF, weakref, weakrefval)

#define INSTRUMENTR_WEAKREF_R_API_MAP(MACRO) \
    INSTRUMENTR_VALUE_DECLARE_R_API(         \
        MACRO, INSTRUMENTR_VALUE_TYPE_WEAKREF, weakref, weakrefval)

#define INSTRUMENTR_WEAKREF_API_MAP(MACRO) \
    INSTRUMENTR_WEAKREF_C_API_MAP(MACRO)   \
    INSTRUMENTR_WEAKREF_R_API_MAP(MACRO)

/********************************************************************************
 BYTECODE API
*******************************************************************************/

#define INSTRUMENTR_BYTECODE_C_API_MAP(MACRO) \
    INSTRUMENTR_VALUE_DECLARE_C_API(          \
        MACRO, INSTRUMENTR_VALUE_TYPE_BYTECODE, bytecode, bytecodeval)

#define INSTRUMENTR_BYTECODE_R_API_MAP(MACRO) \
    INSTRUMENTR_VALUE_DECLARE_R_API(          \
        MACRO, INSTRUMENTR_VALUE_TYPE_BYTECODE, bytecode, bytecodeval)

#define INSTRUMENTR_BYTECODE_API_MAP(MACRO) \
    INSTRUMENTR_BYTECODE_C_API_MAP(MACRO)   \
    INSTRUMENTR_BYTECODE_R_API_MAP(MACRO)

/********************************************************************************
 S4 API
*******************************************************************************/

#define INSTRUMENTR_S4_C_API_MAP(MACRO) \
    INSTRUMENTR_VALUE_DECLARE_C_API(MACRO, INSTRUMENTR_VALUE_TYPE_S4, s4, s4val)

#define INSTRUMENTR_S4_R_API_MAP(MACRO) \
    INSTRUMENTR_VALUE_DECLARE_R_API(MACRO, INSTRUMENTR_VALUE_TYPE_S4, s4, s4val)

#define INSTRUMENTR_S4_API_MAP(MACRO) \
    INSTRUMENTR_S4_C_API_MAP(MACRO)   \
    INSTRUMENTR_S4_R_API_MAP(MACRO)

/********************************************************************************
 CHAR API
 *******************************************************************************/

#define INSTRUMENTR_CHAR_C_API_MAP(MACRO)                                      \
    MACRO(                                                                     \
        instrumentr_char_get_element, const char*, instrumentr_char_t charval) \
    MACRO(instrumentr_char_is_na, bool, instrumentr_char_t charval)            \
    INSTRUMENTR_VALUE_DECLARE_C_API(                                           \
        MACRO, INSTRUMENTR_VALUE_TYPE_CHAR, char, charval)

#define INSTRUMENTR_CHAR_R_API_MAP(MACRO)                    \
    MACRO(r_instrumentr_char_get_element, SEXP, SEXP r_char) \
    MACRO(r_instrumentr_char_is_na, SEXP, SEXP r_char)       \
    INSTRUMENTR_VALUE_DECLARE_R_API(                         \
        MACRO, INSTRUMENTR_VALUE_TYPE_CHAR, char, charval)

#define INSTRUMENTR_CHAR_API_MAP(MACRO) \
    INSTRUMENTR_CHAR_C_API_MAP(MACRO)   \
    INSTRUMENTR_CHAR_R_API_MAP(MACRO)

/********************************************************************************
 SYMBOL API
 *******************************************************************************/

#define INSTRUMENTR_SYMBOL_C_API_MAP(MACRO) \
    MACRO(instrumentr_symbol_get_element,   \
          instrumentr_char_t,               \
          instrumentr_symbol_t symbol)      \
    INSTRUMENTR_VALUE_DECLARE_C_API(        \
        MACRO, INSTRUMENTR_VALUE_TYPE_SYMBOL, symbol, symbolval)

#define INSTRUMENTR_SYMBOL_R_API_MAP(MACRO)                      \
    MACRO(r_instrumentr_symbol_get_element, SEXP, SEXP r_symbol) \
    INSTRUMENTR_VALUE_DECLARE_R_API(                             \
        MACRO, INSTRUMENTR_VALUE_TYPE_SYMBOL, symbol, symbolval)

#define INSTRUMENTR_SYMBOL_API_MAP(MACRO) \
    INSTRUMENTR_SYMBOL_C_API_MAP(MACRO)   \
    INSTRUMENTR_SYMBOL_R_API_MAP(MACRO)

/********************************************************************************
 LANGUAGE API
*******************************************************************************/

#define INSTRUMENTR_LANGUAGE_C_API_MAP(MACRO)                                  \
    MACRO(                                                                     \
        instrumentr_language_get_length, int, instrumentr_language_t language) \
    MACRO(instrumentr_language_get_car,                                        \
          instrumentr_value_t,                                                 \
          instrumentr_language_t language)                                     \
    MACRO(instrumentr_language_get_cdr,                                        \
          instrumentr_value_t,                                                 \
          instrumentr_language_t language)                                     \
    MACRO(instrumentr_language_get_tag,                                        \
          instrumentr_value_t,                                                 \
          instrumentr_language_t language)                                     \
    MACRO(instrumentr_language_get_element,                                    \
          instrumentr_value_t,                                                 \
          instrumentr_language_t language,                                     \
          int index)                                                           \
    INSTRUMENTR_VALUE_DECLARE_C_API(                                           \
        MACRO, INSTRUMENTR_VALUE_TYPE_LANGUAGE, language, language)

#define INSTRUMENTR_LANGUAGE_R_API_MAP(MACRO) \
    INSTRUMENTR_VALUE_DECLARE_R_API(          \
        MACRO, INSTRUMENTR_VALUE_TYPE_LANGUAGE, language, languageval)

#define INSTRUMENTR_LANGUAGE_API_MAP(MACRO) \
    INSTRUMENTR_LANGUAGE_C_API_MAP(MACRO)   \
    INSTRUMENTR_LANGUAGE_R_API_MAP(MACRO)

/********************************************************************************
 DOT API
*******************************************************************************/

#define INSTRUMENTR_DOT_C_API_MAP(MACRO)                                       \
    MACRO(instrumentr_dot_get_length, int, instrumentr_dot_t dot)              \
    MACRO(instrumentr_dot_get_car, instrumentr_value_t, instrumentr_dot_t dot) \
    MACRO(instrumentr_dot_get_cdr, instrumentr_value_t, instrumentr_dot_t dot) \
    MACRO(instrumentr_dot_get_tag, instrumentr_value_t, instrumentr_dot_t dot) \
    MACRO(instrumentr_dot_get_element,                                         \
          instrumentr_value_t,                                                 \
          instrumentr_dot_t dot,                                               \
          int index)                                                           \
    INSTRUMENTR_VALUE_DECLARE_C_API(MACRO, INSTRUMENTR_VALUE_TYPE_DOT, dot, dot)

#define INSTRUMENTR_DOT_R_API_MAP(MACRO) \
    INSTRUMENTR_VALUE_DECLARE_R_API(     \
        MACRO, INSTRUMENTR_VALUE_TYPE_DOT, dot, dotval)

#define INSTRUMENTR_DOT_API_MAP(MACRO) \
    INSTRUMENTR_DOT_C_API_MAP(MACRO)   \
    INSTRUMENTR_DOT_R_API_MAP(MACRO)

/********************************************************************************
 PAIRLIST API
*******************************************************************************/

#define INSTRUMENTR_PAIRLIST_C_API_MAP(MACRO)                                  \
    MACRO(                                                                     \
        instrumentr_pairlist_get_length, int, instrumentr_pairlist_t pairlist) \
    MACRO(instrumentr_pairlist_get_car,                                        \
          instrumentr_value_t,                                                 \
          instrumentr_pairlist_t pairlist)                                     \
    MACRO(instrumentr_pairlist_get_cdr,                                        \
          instrumentr_value_t,                                                 \
          instrumentr_pairlist_t pairlist)                                     \
    MACRO(instrumentr_pairlist_get_tag,                                        \
          instrumentr_value_t,                                                 \
          instrumentr_pairlist_t pairlist)                                     \
    MACRO(instrumentr_pairlist_get_element,                                    \
          instrumentr_value_t,                                                 \
          instrumentr_pairlist_t pairlist,                                     \
          int index)                                                           \
    INSTRUMENTR_VALUE_DECLARE_C_API(                                           \
        MACRO, INSTRUMENTR_VALUE_TYPE_PAIRLIST, pairlist, pairlist)

#define INSTRUMENTR_PAIRLIST_R_API_MAP(MACRO) \
    INSTRUMENTR_VALUE_DECLARE_R_API(          \
        MACRO, INSTRUMENTR_VALUE_TYPE_PAIRLIST, pairlist, pairlistval)

#define INSTRUMENTR_PAIRLIST_API_MAP(MACRO) \
    INSTRUMENTR_PAIRLIST_C_API_MAP(MACRO)   \
    INSTRUMENTR_PAIRLIST_R_API_MAP(MACRO)

/********************************************************************************
 LIST API
*******************************************************************************/

#define INSTRUMENTR_LIST_C_API_MAP(MACRO)                            \
    MACRO(instrumentr_list_get_length, int, instrumentr_list_t list) \
    MACRO(instrumentr_list_get_element,                              \
          instrumentr_value_t,                                       \
          instrumentr_list_t list,                                   \
          int index)                                                 \
    INSTRUMENTR_VALUE_DECLARE_C_API(                                 \
        MACRO, INSTRUMENTR_VALUE_TYPE_LIST, list, list)

#define INSTRUMENTR_LIST_R_API_MAP(MACRO) \
    INSTRUMENTR_VALUE_DECLARE_R_API(      \
        MACRO, INSTRUMENTR_VALUE_TYPE_LIST, list, listval)

#define INSTRUMENTR_LIST_API_MAP(MACRO) \
    INSTRUMENTR_LIST_C_API_MAP(MACRO)   \
    INSTRUMENTR_LIST_R_API_MAP(MACRO)

/********************************************************************************
 EXPRESSION API
*******************************************************************************/

#define INSTRUMENTR_EXPRESSION_C_API_MAP(MACRO) \
    MACRO(instrumentr_expression_get_length,    \
          int,                                  \
          instrumentr_expression_t expression)  \
    MACRO(instrumentr_expression_get_element,   \
          instrumentr_value_t,                  \
          instrumentr_expression_t expression,  \
          int index)                            \
    INSTRUMENTR_VALUE_DECLARE_C_API(            \
        MACRO, INSTRUMENTR_VALUE_TYPE_EXPRESSION, expression, expressionval)

#define INSTRUMENTR_EXPRESSION_R_API_MAP(MACRO) \
    INSTRUMENTR_VALUE_DECLARE_R_API(            \
        MACRO, INSTRUMENTR_VALUE_TYPE_EXPRESSION, expression, expressionval)

#define INSTRUMENTR_EXPRESSION_API_MAP(MACRO) \
    INSTRUMENTR_EXPRESSION_C_API_MAP(MACRO)   \
    INSTRUMENTR_EXPRESSION_R_API_MAP(MACRO)

/********************************************************************************
 CHARACTER API
 *******************************************************************************/

#define INSTRUMENTR_CHARACTER_C_API_MAP(MACRO) \
    MACRO(instrumentr_character_get_size,      \
          int,                                 \
          instrumentr_character_t character)   \
    MACRO(instrumentr_character_get_element,   \
          instrumentr_char_t,                  \
          instrumentr_character_t character,   \
          int index)                           \
    MACRO(instrumentr_character_is_na,         \
          bool,                                \
          instrumentr_character_t character,   \
          int index)                           \
    INSTRUMENTR_VALUE_DECLARE_C_API(           \
        MACRO, INSTRUMENTR_VALUE_TYPE_CHARACTER, character, characterval)

#define INSTRUMENTR_CHARACTER_R_API_MAP(MACRO)                                 \
    MACRO(r_instrumentr_character_get_size, SEXP, SEXP r_character)            \
    MACRO(r_instrumentr_character_get_element,                                 \
          SEXP,                                                                \
          SEXP r_character,                                                    \
          SEXP r_index)                                                        \
    MACRO(r_instrumentr_character_is_na, SEXP, SEXP r_character, SEXP r_index) \
    INSTRUMENTR_VALUE_DECLARE_R_API(                                           \
        MACRO, INSTRUMENTR_VALUE_TYPE_CHARACTER, character, characterval)

#define INSTRUMENTR_CHARACTER_API_MAP(MACRO) \
    INSTRUMENTR_CHARACTER_C_API_MAP(MACRO)   \
    INSTRUMENTR_CHARACTER_R_API_MAP(MACRO)

/********************************************************************************
 INTEGER API
 *******************************************************************************/

#define INSTRUMENTR_INTEGER_C_API_MAP(MACRO)                                \
    MACRO(instrumentr_integer_get_size, int, instrumentr_integer_t integer) \
    MACRO(instrumentr_integer_get_element,                                  \
          int,                                                              \
          instrumentr_integer_t integer,                                    \
          int index)                                                        \
    MACRO(instrumentr_integer_is_na,                                        \
          bool,                                                             \
          instrumentr_integer_t integer,                                    \
          int index)                                                        \
    INSTRUMENTR_VALUE_DECLARE_C_API(                                        \
        MACRO, INSTRUMENTR_VALUE_TYPE_INTEGER, integer, integerval)

#define INSTRUMENTR_INTEGER_R_API_MAP(MACRO)                                   \
    MACRO(r_instrumentr_integer_get_size, SEXP, SEXP r_integer)                \
    MACRO(                                                                     \
        r_instrumentr_integer_get_element, SEXP, SEXP r_integer, SEXP r_index) \
    MACRO(r_instrumentr_integer_is_na, SEXP, SEXP r_integer, SEXP r_index)     \
    INSTRUMENTR_VALUE_DECLARE_R_API(                                           \
        MACRO, INSTRUMENTR_VALUE_TYPE_INTEGER, integer, integerval)

#define INSTRUMENTR_INTEGER_API_MAP(MACRO) \
    INSTRUMENTR_INTEGER_C_API_MAP(MACRO)   \
    INSTRUMENTR_INTEGER_R_API_MAP(MACRO)

/********************************************************************************
 REAL API
 *******************************************************************************/

#define INSTRUMENTR_REAL_C_API_MAP(MACRO)                                   \
    MACRO(instrumentr_real_get_size, int, instrumentr_real_t real)          \
    MACRO(instrumentr_real_get_element,                                     \
          double,                                                           \
          instrumentr_real_t real,                                          \
          int index)                                                        \
    MACRO(instrumentr_real_is_na, bool, instrumentr_real_t real, int index) \
    INSTRUMENTR_VALUE_DECLARE_C_API(                                        \
        MACRO, INSTRUMENTR_VALUE_TYPE_REAL, real, realval)

#define INSTRUMENTR_REAL_R_API_MAP(MACRO)                                  \
    MACRO(r_instrumentr_real_get_size, SEXP, SEXP r_real)                  \
    MACRO(r_instrumentr_real_get_element, SEXP, SEXP r_real, SEXP r_index) \
    MACRO(r_instrumentr_real_is_na, SEXP, SEXP r_real, SEXP r_index)       \
    INSTRUMENTR_VALUE_DECLARE_R_API(                                       \
        MACRO, INSTRUMENTR_VALUE_TYPE_REAL, real, realval)

#define INSTRUMENTR_REAL_API_MAP(MACRO) \
    INSTRUMENTR_REAL_C_API_MAP(MACRO)   \
    INSTRUMENTR_REAL_R_API_MAP(MACRO)

/********************************************************************************
 LOGICAL API
 *******************************************************************************/

#define INSTRUMENTR_LOGICAL_C_API_MAP(MACRO)                                \
    MACRO(instrumentr_logical_get_size, int, instrumentr_logical_t logical) \
    MACRO(instrumentr_logical_get_element,                                  \
          int,                                                              \
          instrumentr_logical_t logical,                                    \
          int index)                                                        \
    MACRO(instrumentr_logical_is_na,                                        \
          bool,                                                             \
          instrumentr_logical_t logical,                                    \
          int index)                                                        \
    INSTRUMENTR_VALUE_DECLARE_C_API(                                        \
        MACRO, INSTRUMENTR_VALUE_TYPE_LOGICAL, logical, logicalval)

#define INSTRUMENTR_LOGICAL_R_API_MAP(MACRO)                                   \
    MACRO(r_instrumentr_logical_get_size, SEXP, SEXP r_logical)                \
    MACRO(                                                                     \
        r_instrumentr_logical_get_element, SEXP, SEXP r_logical, SEXP r_index) \
    MACRO(r_instrumentr_logical_is_na, SEXP, SEXP r_logical, SEXP r_index)     \
    INSTRUMENTR_VALUE_DECLARE_R_API(                                           \
        MACRO, INSTRUMENTR_VALUE_TYPE_LOGICAL, logical, logicalval)

#define INSTRUMENTR_LOGICAL_API_MAP(MACRO) \
    INSTRUMENTR_LOGICAL_C_API_MAP(MACRO)   \
    INSTRUMENTR_LOGICAL_R_API_MAP(MACRO)

/********************************************************************************
 COMPLEX API
 *******************************************************************************/

#define INSTRUMENTR_COMPLEX_C_API_MAP(MACRO)                                \
    MACRO(instrumentr_complex_get_size, int, instrumentr_complex_t complex) \
    MACRO(instrumentr_complex_get_element,                                  \
          Rcomplex,                                                         \
          instrumentr_complex_t complex,                                    \
          int index)                                                        \
    MACRO(instrumentr_complex_is_na,                                        \
          bool,                                                             \
          instrumentr_complex_t complex,                                    \
          int index)                                                        \
    INSTRUMENTR_VALUE_DECLARE_C_API(                                        \
        MACRO, INSTRUMENTR_VALUE_TYPE_COMPLEX, complex, complexval)

#define INSTRUMENTR_COMPLEX_R_API_MAP(MACRO)                                   \
    MACRO(r_instrumentr_complex_get_size, SEXP, SEXP r_complex)                \
    MACRO(                                                                     \
        r_instrumentr_complex_get_element, SEXP, SEXP r_complex, SEXP r_index) \
    MACRO(r_instrumentr_complex_is_na, SEXP, SEXP r_complex, SEXP r_index)     \
    INSTRUMENTR_VALUE_DECLARE_R_API(                                           \
        MACRO, INSTRUMENTR_VALUE_TYPE_COMPLEX, complex, complexval)

#define INSTRUMENTR_COMPLEX_API_MAP(MACRO) \
    INSTRUMENTR_COMPLEX_C_API_MAP(MACRO)   \
    INSTRUMENTR_COMPLEX_R_API_MAP(MACRO)

/********************************************************************************
 RAW API
 *******************************************************************************/

#define INSTRUMENTR_RAW_C_API_MAP(MACRO)                                      \
    MACRO(instrumentr_raw_get_size, int, instrumentr_raw_t raw)               \
    MACRO(                                                                    \
        instrumentr_raw_get_element, Rbyte, instrumentr_raw_t raw, int index) \
    INSTRUMENTR_VALUE_DECLARE_C_API(                                          \
        MACRO, INSTRUMENTR_VALUE_TYPE_RAW, raw, rawval)

#define INSTRUMENTR_RAW_R_API_MAP(MACRO)                                 \
    MACRO(r_instrumentr_raw_get_size, SEXP, SEXP r_raw)                  \
    MACRO(r_instrumentr_raw_get_element, SEXP, SEXP r_raw, SEXP r_index) \
    INSTRUMENTR_VALUE_DECLARE_R_API(                                     \
        MACRO, INSTRUMENTR_VALUE_TYPE_RAW, raw, rawval)

#define INSTRUMENTR_RAW_API_MAP(MACRO) \
    INSTRUMENTR_RAW_C_API_MAP(MACRO)   \
    INSTRUMENTR_RAW_R_API_MAP(MACRO)

/********************************************************************************
 ENVIRONMENT API
 *******************************************************************************/

#define INSTRUMENTR_ENVIRONMENT_C_API_MAP(MACRO)                        \
    MACRO(instrumentr_environment_get_name,                             \
          const char*,                                                  \
          instrumentr_environment_t environment)                        \
    MACRO(instrumentr_environment_get_type,                             \
          instrumentr_environment_type_t,                               \
          instrumentr_environment_t environment)                        \
    MACRO(instrumentr_environment_type_to_string,                       \
          const char*,                                                  \
          instrumentr_environment_type_t type)                          \
    MACRO(instrumentr_environment_get_size,                             \
          int,                                                          \
          instrumentr_environment_t environment)                        \
    MACRO(instrumentr_environment_lookup,                               \
          instrumentr_value_t,                                          \
          instrumentr_environment_t environment,                        \
          instrumentr_symbol_t symbol)                                  \
    MACRO(instrumentr_environment_contains,                             \
          int,                                                          \
          instrumentr_environment_t environment,                        \
          const char* name)                                             \
    MACRO(instrumentr_environment_get_call,                             \
          instrumentr_call_t,                                           \
          instrumentr_environment_t environment)                        \
    MACRO(instrumentr_environment_get_last_read_time,                   \
          int,                                                          \
          instrumentr_environment_t environment)                        \
    MACRO(instrumentr_environment_get_last_write_time,                  \
          int,                                                          \
          instrumentr_environment_t environment)                        \
    INSTRUMENTR_VALUE_DECLARE_C_API(MACRO,                              \
                                    INSTRUMENTR_VALUE_TYPE_ENVIRONMENT, \
                                    environment,                        \
                                    environmentval)

#define INSTRUMENTR_ENVIRONMENT_R_API_MAP(MACRO)                            \
    MACRO(r_instrumentr_environment_get_name, SEXP, SEXP r_environment)     \
    MACRO(r_instrumentr_environment_get_size, SEXP, SEXP r_environment)     \
    MACRO(r_instrumentr_environment_lookup,                                 \
          SEXP,                                                             \
          SEXP r_environment,                                               \
          SEXP r_name)                                                      \
    MACRO(r_instrumentr_environment_contains,                               \
          SEXP,                                                             \
          SEXP r_environment,                                               \
          SEXP r_name)                                                      \
    MACRO(r_instrumentr_environment_get_bindings, SEXP, SEXP r_environment) \
    INSTRUMENTR_VALUE_DECLARE_R_API(MACRO,                                  \
                                    INSTRUMENTR_VALUE_TYPE_ENVIRONMENT,     \
                                    environment,                            \
                                    environmentval)

#define INSTRUMENTR_ENVIRONMENT_API_MAP(MACRO) \
    INSTRUMENTR_ENVIRONMENT_C_API_MAP(MACRO)   \
    INSTRUMENTR_ENVIRONMENT_R_API_MAP(MACRO)

/********************************************************************************
 BUILTIN API
 *******************************************************************************/

#define INSTRUMENTR_BUILTIN_C_API_MAP(MACRO)                                   \
    MACRO(instrumentr_builtin_get_name,                                        \
          const char*,                                                         \
          instrumentr_builtin_t builtin)                                       \
    MACRO(instrumentr_builtin_get_parameter_count,                             \
          int,                                                                 \
          instrumentr_builtin_t builtin)                                       \
    MACRO(instrumentr_builtin_is_internal, int, instrumentr_builtin_t builtin) \
    MACRO(                                                                     \
        instrumentr_builtin_is_primitive, int, instrumentr_builtin_t builtin)  \
    INSTRUMENTR_VALUE_DECLARE_C_API(                                           \
        MACRO, INSTRUMENTR_VALUE_TYPE_BUILTIN, builtin, builtinval)

#define INSTRUMENTR_BUILTIN_R_API_MAP(MACRO)                               \
    MACRO(r_instrumentr_builtin_get_name, SEXP, SEXP r_builtin)            \
    MACRO(r_instrumentr_builtin_get_parameter_count, SEXP, SEXP r_builtin) \
    MACRO(r_instrumentr_builtin_is_internal, SEXP, SEXP r_builtin)         \
    MACRO(r_instrumentr_builtin_is_primitive, SEXP, SEXP r_builtin)        \
    INSTRUMENTR_VALUE_DECLARE_R_API(                                       \
        MACRO, INSTRUMENTR_VALUE_TYPE_BUILTIN, builtin, builtinval)

#define INSTRUMENTR_BUILTIN_API_MAP(MACRO) \
    INSTRUMENTR_BUILTIN_C_API_MAP(MACRO)   \
    INSTRUMENTR_BUILTIN_R_API_MAP(MACRO)

/********************************************************************************
 SPECIAL API
 *******************************************************************************/

#define INSTRUMENTR_SPECIAL_C_API_MAP(MACRO)                                   \
    MACRO(instrumentr_special_get_name,                                        \
          const char*,                                                         \
          instrumentr_special_t special)                                       \
    MACRO(instrumentr_special_get_parameter_count,                             \
          int,                                                                 \
          instrumentr_special_t special)                                       \
    MACRO(instrumentr_special_is_internal, int, instrumentr_special_t special) \
    MACRO(                                                                     \
        instrumentr_special_is_primitive, int, instrumentr_special_t special)  \
    INSTRUMENTR_VALUE_DECLARE_C_API(                                           \
        MACRO, INSTRUMENTR_VALUE_TYPE_SPECIAL, special, specialval)

#define INSTRUMENTR_SPECIAL_R_API_MAP(MACRO)                               \
    MACRO(r_instrumentr_special_get_name, SEXP, SEXP r_special)            \
    MACRO(r_instrumentr_special_get_parameter_count, SEXP, SEXP r_special) \
    MACRO(r_instrumentr_special_is_internal, SEXP, SEXP r_special)         \
    MACRO(r_instrumentr_special_is_primitive, SEXP, SEXP r_special)        \
    INSTRUMENTR_VALUE_DECLARE_R_API(                                       \
        MACRO, INSTRUMENTR_VALUE_TYPE_SPECIAL, special, specialval)

#define INSTRUMENTR_SPECIAL_API_MAP(MACRO) \
    INSTRUMENTR_SPECIAL_C_API_MAP(MACRO)   \
    INSTRUMENTR_SPECIAL_R_API_MAP(MACRO)

/********************************************************************************
 CLOSURE API
 *******************************************************************************/

#define INSTRUMENTR_CLOSURE_C_API_MAP(MACRO)                                   \
    MACRO(instrumentr_closure_has_name, int, instrumentr_closure_t closure)    \
    MACRO(instrumentr_closure_get_name,                                        \
          const char*,                                                         \
          instrumentr_closure_t closure)                                       \
    MACRO(instrumentr_closure_get_formals,                                     \
          instrumentr_value_t,                                                 \
          instrumentr_closure_t closure)                                       \
    MACRO(instrumentr_closure_get_environment,                                 \
          instrumentr_environment_t,                                           \
          instrumentr_closure_t closure)                                       \
    MACRO(instrumentr_closure_is_inner, int, instrumentr_closure_t closure)    \
    MACRO(instrumentr_closure_get_parent,                                      \
          instrumentr_closure_t,                                               \
          instrumentr_closure_t closure)                                       \
    MACRO(instrumentr_closure_get_parameter_count,                             \
          int,                                                                 \
          instrumentr_closure_t closure)                                       \
    MACRO(instrumentr_closure_is_exported, int, instrumentr_closure_t closure) \
    MACRO(                                                                     \
        instrumentr_closure_is_s3_generic, int, instrumentr_closure_t closure) \
    MACRO(                                                                     \
        instrumentr_closure_is_s3_method, int, instrumentr_closure_t closure)  \
    INSTRUMENTR_VALUE_DECLARE_C_API(                                           \
        MACRO, INSTRUMENTR_VALUE_TYPE_CLOSURE, closure, closureval)

#define INSTRUMENTR_CLOSURE_R_API_MAP(MACRO)                               \
    MACRO(r_instrumentr_closure_has_name, SEXP, SEXP r_closure)            \
    MACRO(r_instrumentr_closure_get_name, SEXP, SEXP r_closure)            \
    MACRO(r_instrumentr_closure_get_environment, SEXP, SEXP r_closure)     \
    MACRO(r_instrumentr_closure_is_inner, SEXP, SEXP r_closure)            \
    MACRO(r_instrumentr_closure_get_parameter_count, SEXP, SEXP r_closure) \
    MACRO(r_instrumentr_closure_is_exported, SEXP, SEXP r_closure)         \
    MACRO(r_instrumentr_closure_is_s3_generic, SEXP, SEXP r_closure)       \
    MACRO(r_instrumentr_closure_is_s3_method, SEXP, SEXP r_closure)        \
    INSTRUMENTR_VALUE_DECLARE_R_API(                                       \
        MACRO, INSTRUMENTR_VALUE_TYPE_CLOSURE, closure, closureval)

#define INSTRUMENTR_CLOSURE_API_MAP(MACRO) \
    INSTRUMENTR_CLOSURE_C_API_MAP(MACRO)   \
    INSTRUMENTR_CLOSURE_R_API_MAP(MACRO)

/********************************************************************************
 CALL API
 *******************************************************************************/

#define INSTRUMENTR_CALL_C_API_MAP(MACRO)                            \
    MACRO(instrumentr_call_get_function,                             \
          instrumentr_value_t,                                       \
          instrumentr_call_t call)                                   \
    MACRO(instrumentr_call_get_expression,                           \
          instrumentr_language_t,                                    \
          instrumentr_call_t call)                                   \
    MACRO(instrumentr_call_get_arguments,                            \
          instrumentr_value_t,                                       \
          instrumentr_call_t call)                                   \
    MACRO(instrumentr_call_get_environment,                          \
          instrumentr_environment_t,                                 \
          instrumentr_call_t call)                                   \
    MACRO(instrumentr_call_is_active, int, instrumentr_call_t call)  \
    MACRO(instrumentr_call_has_result, int, instrumentr_call_t call) \
    MACRO(instrumentr_call_get_result,                               \
          instrumentr_value_t,                                       \
          instrumentr_call_t call)                                   \
    INSTRUMENTR_MODEL_DECLARE_DERIVED_C_API(                         \
        MACRO, INSTRUMENTR_MODEL_TYPE_CALL, call, call)

#define INSTRUMENTR_CALL_R_API_MAP(MACRO)                        \
    MACRO(r_instrumentr_call_get_function, SEXP, SEXP r_call)    \
    MACRO(r_instrumentr_call_get_expression, SEXP, SEXP r_call)  \
    MACRO(r_instrumentr_call_get_arguments, SEXP, SEXP r_call)   \
    MACRO(r_instrumentr_call_get_environment, SEXP, SEXP r_call) \
    MACRO(r_instrumentr_call_is_active, SEXP, SEXP r_call)       \
    MACRO(r_instrumentr_call_has_result, SEXP, SEXP r_call)      \
    MACRO(r_instrumentr_call_get_result, SEXP, SEXP r_call)      \
    INSTRUMENTR_MODEL_DECLARE_DERIVED_R_API(                     \
        MACRO, INSTRUMENTR_MODEL_TYPE_CALL, call, call)

#define INSTRUMENTR_CALL_API_MAP(MACRO) \
    INSTRUMENTR_CALL_C_API_MAP(MACRO)   \
    INSTRUMENTR_CALL_R_API_MAP(MACRO)

/********************************************************************************
 CALL STACK API
 *******************************************************************************/

#define INSTRUMENTR_CALL_STACK_C_API_MAP(MACRO) \
    MACRO(instrumentr_call_stack_get_size,      \
          int,                                  \
          instrumentr_call_stack_t call_stack)  \
    MACRO(instrumentr_call_stack_get_frame,     \
          instrumentr_frame_t,                  \
          instrumentr_call_stack_t call_stack,  \
          int index)                            \
    MACRO(instrumentr_call_stack_peek_frame,    \
          instrumentr_frame_t,                  \
          instrumentr_call_stack_t call_stack,  \
          int index)                            \
    INSTRUMENTR_MODEL_DECLARE_DERIVED_C_API(    \
        MACRO, INSTRUMENTR_MODEL_TYPE_CALL_STACK, call_stack, call_stack)

#define INSTRUMENTR_CALL_STACK_R_API_MAP(MACRO)                       \
    MACRO(r_instrumentr_call_stack_get_size, SEXP, SEXP r_call_stack) \
    MACRO(r_instrumentr_call_stack_get_frame,                         \
          SEXP,                                                       \
          SEXP r_call_stack,                                          \
          SEXP r_index)                                               \
    MACRO(r_instrumentr_call_stack_peek_frame,                        \
          SEXP,                                                       \
          SEXP r_call_stack,                                          \
          SEXP r_index)                                               \
    INSTRUMENTR_MODEL_DECLARE_DERIVED_R_API(                          \
        MACRO, INSTRUMENTR_MODEL_TYPE_CALL_STACK, call_stack, call_stack)

#define INSTRUMENTR_CALL_STACK_API_MAP(MACRO) \
    INSTRUMENTR_CALL_STACK_C_API_MAP(MACRO)   \
    INSTRUMENTR_CALL_STACK_R_API_MAP(MACRO)

/********************************************************************************
 FRAME API
*******************************************************************************/

#define INSTRUMENTR_FRAME_C_API_MAP(MACRO)                              \
    MACRO(instrumentr_frame_is_call, int, instrumentr_frame_t frame)    \
    MACRO(instrumentr_frame_as_call,                                    \
          instrumentr_call_t,                                           \
          instrumentr_frame_t frame)                                    \
    MACRO(instrumentr_frame_is_promise, int, instrumentr_frame_t frame) \
    MACRO(instrumentr_frame_as_promise,                                 \
          instrumentr_promise_t,                                        \
          instrumentr_frame_t frame)                                    \
    MACRO(instrumentr_frame_is_context, int, instrumentr_frame_t frame) \
    MACRO(instrumentr_frame_as_context,                                 \
          instrumentr_context_t,                                        \
          instrumentr_frame_t frame)                                    \
    INSTRUMENTR_MODEL_DECLARE_DERIVED_C_API(                            \
        MACRO, INSTRUMENTR_MODEL_TYPE_FRAME, frame, frame)

#define INSTRUMENTR_FRAME_R_API_MAP(MACRO)                    \
    MACRO(r_instrumentr_frame_is_call, SEXP, SEXP r_frame)    \
    MACRO(r_instrumentr_frame_as_call, SEXP, SEXP r_frame)    \
    MACRO(r_instrumentr_frame_is_promise, SEXP, SEXP r_frame) \
    MACRO(r_instrumentr_frame_as_promise, SEXP, SEXP r_frame) \
    MACRO(r_instrumentr_frame_is_context, SEXP, SEXP r_frame) \
    MACRO(r_instrumentr_frame_as_context, SEXP, SEXP r_frame) \
    INSTRUMENTR_MODEL_DECLARE_DERIVED_R_API(                  \
        MACRO, INSTRUMENTR_MODEL_TYPE_FRAME, frame, frame)

#define INSTRUMENTR_FRAME_API_MAP(MACRO) \
    INSTRUMENTR_FRAME_C_API_MAP(MACRO)   \
    INSTRUMENTR_FRAME_R_API_MAP(MACRO)

/********************************************************************************
 PROMISE API
 *******************************************************************************/

#define INSTRUMENTR_PROMISE_C_API_MAP(MACRO)                                   \
    MACRO(instrumentr_promise_is_forced, int, instrumentr_promise_t promise)   \
    MACRO(instrumentr_promise_get_expression,                                  \
          instrumentr_value_t,                                                 \
          instrumentr_promise_t promise)                                       \
    MACRO(instrumentr_promise_get_value,                                       \
          instrumentr_value_t,                                                 \
          instrumentr_promise_t promise)                                       \
    MACRO(instrumentr_promise_get_type,                                        \
          instrumentr_promise_type_t,                                          \
          instrumentr_promise_t promise)                                       \
    MACRO(instrumentr_promise_is_argument, int, instrumentr_promise_t promise) \
    MACRO(instrumentr_promise_is_delayed_assign,                               \
          int,                                                                 \
          instrumentr_promise_t promise)                                       \
    MACRO(                                                                     \
        instrumentr_promise_is_lazy_load, int, instrumentr_promise_t promise)  \
    MACRO(instrumentr_promise_is_unknown, int, instrumentr_promise_t promise)  \
    MACRO(instrumentr_promise_get_calls,                                       \
          const std::vector<instrumentr_call_t>&,                              \
          instrumentr_promise_t promise)                                       \
    MACRO(instrumentr_promise_get_call,                                        \
          instrumentr_call_t,                                                  \
          instrumentr_promise_t promise)                                       \
    INSTRUMENTR_VALUE_DECLARE_C_API(                                           \
        MACRO, INSTRUMENTR_MODEL_TYPE_PROMISE, promise, promiseval)            \
    MACRO(instrumentr_promise_get_force_entry_time,                            \
          int,                                                                 \
          instrumentr_promise_t promise)                                       \
    MACRO(instrumentr_promise_get_force_exit_time,                             \
          int,                                                                 \
          instrumentr_promise_t promise)

#define INSTRUMENTR_PROMISE_R_API_MAP(MACRO)                             \
    MACRO(r_instrumentr_promise_is_forced, SEXP, SEXP r_promise)         \
    MACRO(r_instrumentr_promise_get_expression, SEXP, SEXP r_promise)    \
    MACRO(r_instrumentr_promise_get_value, SEXP, SEXP r_promise)         \
    MACRO(r_instrumentr_promise_get_type, SEXP, SEXP r_promise)          \
    MACRO(r_instrumentr_promise_is_argument, SEXP, SEXP r_promise)       \
    MACRO(r_instrumentr_promise_is_delayed_assign, SEXP, SEXP r_promise) \
    MACRO(r_instrumentr_promise_is_lazy_load, SEXP, SEXP r_promise)      \
    MACRO(r_instrumentr_promise_is_unknown, SEXP, SEXP r_promise)        \
    MACRO(r_instrumentr_promise_get_call, SEXP, SEXP r_promise)          \
    INSTRUMENTR_VALUE_DECLARE_R_API(                                     \
        MACRO, INSTRUMENTR_MODEL_TYPE_PROMISE, promise, promiseval)

#define INSTRUMENTR_PROMISE_API_MAP(MACRO) \
    INSTRUMENTR_PROMISE_C_API_MAP(MACRO)   \
    INSTRUMENTR_PROMISE_R_API_MAP(MACRO)

/********************************************************************************
 MISCELLANEOUS API
 *******************************************************************************/

#define INSTRUMENTR_MISCELLANEOUS_C_API_MAP(MACRO) \
    INSTRUMENTR_VALUE_DECLARE_C_API(               \
        MACRO, INSTRUMENTR_MODEL_TYPE_MISCELLANEOUS, miscellaneous, miscval)

#define INSTRUMENTR_MISCELLANEOUS_R_API_MAP(MACRO) \
    INSTRUMENTR_VALUE_DECLARE_R_API(               \
        MACRO, INSTRUMENTR_MODEL_TYPE_MISCELLANEOUS, miscellaneous, miscval)

#define INSTRUMENTR_MISCELLANEOUS_API_MAP(MACRO) \
    INSTRUMENTR_MISCELLANEOUS_C_API_MAP(MACRO)   \
    INSTRUMENTR_MISCELLANEOUS_R_API_MAP(MACRO)

/********************************************************************************
 VALUE API
 *******************************************************************************/

#define INSTRUMENTR_VALUE_C_API_MAP(MACRO)                                     \
    MACRO(instrumentr_value_wrap, SEXP, instrumentr_value_t value)             \
    MACRO(instrumentr_value_unwrap,                                            \
          instrumentr_value_t,                                                 \
          SEXP r_value,                                                        \
          instrumentr_value_type_t type)                                       \
    MACRO(                                                                     \
        instrumentr_value_get_reference_count, int, instrumentr_value_t value) \
    MACRO(                                                                     \
        instrumentr_value_get_id, instrumentr_id_t, instrumentr_value_t value) \
    MACRO(instrumentr_value_get_type,                                          \
          instrumentr_value_type_t,                                            \
          instrumentr_value_t value)                                           \
    MACRO(instrumentr_value_get_birth_time, int, instrumentr_value_t value)    \
    MACRO(instrumentr_value_get_death_time, int, instrumentr_value_t value)    \
    MACRO(instrumentr_value_get_life_time, int, instrumentr_value_t value)     \
    MACRO(instrumentr_value_is_alive, int, instrumentr_value_t value)          \
    MACRO(instrumentr_value_is_dead, int, instrumentr_value_t value)           \
    MACRO(instrumentr_value_is_local, int, instrumentr_value_t value)          \
    MACRO(instrumentr_value_is_foreign, int, instrumentr_value_t value)        \
    MACRO(instrumentr_value_get_state,                                         \
          instrumentr_state_t,                                                 \
          instrumentr_value_t value)                                           \
    MACRO(instrumentr_value_get_sexp, SEXP, instrumentr_value_t value)

#define INSTRUMENTR_VALUE_R_API_MAP(MACRO)                             \
    MACRO(r_instrumentr_value_get_reference_count, SEXP, SEXP r_value) \
    MACRO(r_instrumentr_value_get_id, SEXP, SEXP r_value)              \
    MACRO(r_instrumentr_value_get_birth_time, SEXP, SEXP r_value)      \
    MACRO(r_instrumentr_value_get_death_time, SEXP, SEXP r_value)      \
    MACRO(r_instrumentr_value_get_life_time, SEXP, SEXP r_value)       \
    MACRO(r_instrumentr_value_is_alive, SEXP, SEXP r_value)            \
    MACRO(r_instrumentr_value_is_dead, SEXP, SEXP r_value)             \
    MACRO(r_instrumentr_value_is_local, SEXP, SEXP r_value)            \
    MACRO(r_instrumentr_value_is_foreign, SEXP, SEXP r_value)          \
    MACRO(r_instrumentr_value_get_sexp, SEXP, SEXP r_value)

#define INSTRUMENTR_VALUE_API_MAP(MACRO) \
    INSTRUMENTR_VALUE_C_API_MAP(MACRO)   \
    INSTRUMENTR_VALUE_R_API_MAP(MACRO)

/********************************************************************************
 CONTEXT API
*******************************************************************************/

#define INSTRUMENTR_CONTEXT_C_API_MAP(MACRO)                                   \
    MACRO(                                                                     \
        instrumentr_context_get_pointer, void*, instrumentr_context_t context) \
    MACRO(instrumentr_context_get_promargs,                                    \
          instrumentr_value_t,                                                 \
          instrumentr_context_t context)                                       \
    MACRO(instrumentr_context_get_cloenv,                                      \
          instrumentr_value_t,                                                 \
          instrumentr_context_t context)                                       \
    INSTRUMENTR_MODEL_DECLARE_DERIVED_C_API(                                   \
        MACRO, INSTRUMENTR_MODEL_TYPE_CONTEXT, context, context)

#define INSTRUMENTR_CONTEXT_R_API_MAP(MACRO)                       \
    MACRO(r_instrumentr_context_get_pointer, SEXP, SEXP r_context) \
    INSTRUMENTR_MODEL_DECLARE_DERIVED_R_API(                       \
        MACRO, INSTRUMENTR_MODEL_TYPE_CONTEXT, context, context)

#define INSTRUMENTR_CONTEXT_API_MAP(MACRO) \
    INSTRUMENTR_CONTEXT_C_API_MAP(MACRO)   \
    INSTRUMENTR_CONTEXT_R_API_MAP(MACRO)

/********************************************************************************
 VALUE_TYPE API
*******************************************************************************/
#define INSTRUMENTR_VALUE_TYPE_C_API_MAP(MACRO) \
    MACRO(instrumentr_value_type_get_name,      \
          const char*,                          \
          instrumentr_value_type_t value_type)

#define INSTRUMENTR_VALUE_TYPE_R_API_MAP(MACRO)

#define INSTRUMENTR_VALUE_TYPE_API_MAP(MACRO) \
    INSTRUMENTR_VALUE_TYPE_C_API_MAP(MACRO)   \
    INSTRUMENTR_VALUE_TYPE_R_API_MAP(MACRO)

/********************************************************************************
 MODEL_TYPE API
*******************************************************************************/
#define INSTRUMENTR_MODEL_TYPE_C_API_MAP(MACRO) \
    MACRO(instrumentr_model_type_get_name,      \
          const char*,                          \
          instrumentr_model_type_t model_type)

#define INSTRUMENTR_MODEL_TYPE_R_API_MAP(MACRO)

#define INSTRUMENTR_MODEL_TYPE_API_MAP(MACRO) \
    INSTRUMENTR_MODEL_TYPE_C_API_MAP(MACRO)   \
    INSTRUMENTR_MODEL_TYPE_R_API_MAP(MACRO)

#define INSTRUMENTR_API_MAP(MACRO)           \
    INSTRUMENTR_INTEROP_API_MAP(MACRO)       \
    INSTRUMENTR_OBJECT_API_MAP(MACRO)        \
    INSTRUMENTR_TRACER_API_MAP(MACRO)        \
    INSTRUMENTR_CALLBACK_API_MAP(MACRO)      \
    INSTRUMENTR_EXEC_STATS_API_MAP(MACRO)    \
    INSTRUMENTR_ALLOC_STATS_API_MAP(MACRO)   \
    INSTRUMENTR_STATE_API_MAP(MACRO)         \
    INSTRUMENTR_MODEL_API_MAP(MACRO)         \
    INSTRUMENTR_APPLICATION_API_MAP(MACRO)   \
    INSTRUMENTR_CALL_API_MAP(MACRO)          \
    INSTRUMENTR_CALL_STACK_API_MAP(MACRO)    \
    INSTRUMENTR_FRAME_API_MAP(MACRO)         \
    INSTRUMENTR_CONTEXT_API_MAP(MACRO)       \
    INSTRUMENTR_VALUE_API_MAP(MACRO)         \
    INSTRUMENTR_NULL_API_MAP(MACRO)          \
    INSTRUMENTR_SYMBOL_API_MAP(MACRO)        \
    INSTRUMENTR_PAIRLIST_API_MAP(MACRO)      \
    INSTRUMENTR_CLOSURE_API_MAP(MACRO)       \
    INSTRUMENTR_ENVIRONMENT_API_MAP(MACRO)   \
    INSTRUMENTR_PROMISE_API_MAP(MACRO)       \
    INSTRUMENTR_LANGUAGE_API_MAP(MACRO)      \
    INSTRUMENTR_SPECIAL_API_MAP(MACRO)       \
    INSTRUMENTR_BUILTIN_API_MAP(MACRO)       \
    INSTRUMENTR_CHAR_API_MAP(MACRO)          \
    INSTRUMENTR_LOGICAL_API_MAP(MACRO)       \
    INSTRUMENTR_INTEGER_API_MAP(MACRO)       \
    INSTRUMENTR_REAL_API_MAP(MACRO)          \
    INSTRUMENTR_COMPLEX_API_MAP(MACRO)       \
    INSTRUMENTR_CHARACTER_API_MAP(MACRO)     \
    INSTRUMENTR_DOT_API_MAP(MACRO)           \
    INSTRUMENTR_LIST_API_MAP(MACRO)          \
    INSTRUMENTR_EXPRESSION_API_MAP(MACRO)    \
    INSTRUMENTR_BYTECODE_API_MAP(MACRO)      \
    INSTRUMENTR_EXTERNALPTR_API_MAP(MACRO)   \
    INSTRUMENTR_WEAKREF_API_MAP(MACRO)       \
    INSTRUMENTR_RAW_API_MAP(MACRO)           \
    INSTRUMENTR_S4_API_MAP(MACRO)            \
    INSTRUMENTR_UNBOUND_API_MAP(MACRO)       \
    INSTRUMENTR_MISSING_API_MAP(MACRO)       \
    INSTRUMENTR_MISCELLANEOUS_API_MAP(MACRO) \
    INSTRUMENTR_VALUE_TYPE_API_MAP(MACRO)    \
    INSTRUMENTR_MODEL_TYPE_API_MAP(MACRO)

#define INSTRUMENTR_R_API_MAP(MACRO)           \
    INSTRUMENTR_INTEROP_R_API_MAP(MACRO)       \
    INSTRUMENTR_OBJECT_R_API_MAP(MACRO)        \
    INSTRUMENTR_TRACER_R_API_MAP(MACRO)        \
    INSTRUMENTR_CALLBACK_R_API_MAP(MACRO)      \
    INSTRUMENTR_EXEC_STATS_R_API_MAP(MACRO)    \
    INSTRUMENTR_ALLOC_STATS_R_API_MAP(MACRO)   \
    INSTRUMENTR_STATE_R_API_MAP(MACRO)         \
    INSTRUMENTR_MODEL_R_API_MAP(MACRO)         \
    INSTRUMENTR_APPLICATION_R_API_MAP(MACRO)   \
    INSTRUMENTR_CALL_R_API_MAP(MACRO)          \
    INSTRUMENTR_CALL_STACK_R_API_MAP(MACRO)    \
    INSTRUMENTR_FRAME_R_API_MAP(MACRO)         \
    INSTRUMENTR_CONTEXT_R_API_MAP(MACRO)       \
    INSTRUMENTR_VALUE_R_API_MAP(MACRO)         \
    INSTRUMENTR_NULL_R_API_MAP(MACRO)          \
    INSTRUMENTR_SYMBOL_R_API_MAP(MACRO)        \
    INSTRUMENTR_PAIRLIST_R_API_MAP(MACRO)      \
    INSTRUMENTR_CLOSURE_R_API_MAP(MACRO)       \
    INSTRUMENTR_ENVIRONMENT_R_API_MAP(MACRO)   \
    INSTRUMENTR_PROMISE_R_API_MAP(MACRO)       \
    INSTRUMENTR_LANGUAGE_R_API_MAP(MACRO)      \
    INSTRUMENTR_SPECIAL_R_API_MAP(MACRO)       \
    INSTRUMENTR_BUILTIN_R_API_MAP(MACRO)       \
    INSTRUMENTR_CHAR_R_API_MAP(MACRO)          \
    INSTRUMENTR_LOGICAL_R_API_MAP(MACRO)       \
    INSTRUMENTR_INTEGER_R_API_MAP(MACRO)       \
    INSTRUMENTR_REAL_R_API_MAP(MACRO)          \
    INSTRUMENTR_COMPLEX_R_API_MAP(MACRO)       \
    INSTRUMENTR_CHARACTER_R_API_MAP(MACRO)     \
    INSTRUMENTR_DOT_R_API_MAP(MACRO)           \
    INSTRUMENTR_LIST_R_API_MAP(MACRO)          \
    INSTRUMENTR_EXPRESSION_R_API_MAP(MACRO)    \
    INSTRUMENTR_BYTECODE_R_API_MAP(MACRO)      \
    INSTRUMENTR_EXTERNALPTR_R_API_MAP(MACRO)   \
    INSTRUMENTR_WEAKREF_R_API_MAP(MACRO)       \
    INSTRUMENTR_RAW_R_API_MAP(MACRO)           \
    INSTRUMENTR_S4_R_API_MAP(MACRO)            \
    INSTRUMENTR_UNBOUND_R_API_MAP(MACRO)       \
    INSTRUMENTR_MISSING_R_API_MAP(MACRO)       \
    INSTRUMENTR_MISCELLANEOUS_R_API_MAP(MACRO) \
    INSTRUMENTR_VALUE_TYPE_R_API_MAP(MACRO)    \
    INSTRUMENTR_MODEL_TYPE_R_API_MAP(MACRO)

#define INSTRUMENTR_API_INCLUDER(FUNCTION, OUTPUT_TYPE, ...) \
    OUTPUT_TYPE FUNCTION(__VA_ARGS__);

#define INSTRUMENTR_API_DEFINER(FUNCTION, OUTPUT_TYPE, ...) \
    OUTPUT_TYPE (*FUNCTION)(__VA_ARGS__);

#define INSTRUMENTR_DEFINE_API() INSTRUMENTR_API_MAP(INSTRUMENTR_API_DEFINER)

#define INSTRUMENTR_API_DECLARATOR(FUNCTION, OUTPUT_TYPE, ...) \
    extern OUTPUT_TYPE (*FUNCTION)(__VA_ARGS__);

#define INSTRUMENTR_DECLARE_API() \
    INSTRUMENTR_API_MAP(INSTRUMENTR_API_DECLARATOR)

#define INSTRUMENTR_API_INITIALIZER(FUNCTION, OUTPUT_TYPE, ...) \
    FUNCTION = (OUTPUT_TYPE(*)(__VA_ARGS__))(                   \
        R_GetCCallable("instrumentr", #FUNCTION));

#define INSTRUMENTR_INITIALIZE_API() \
    INSTRUMENTR_API_MAP(INSTRUMENTR_API_INITIALIZER)

#define INSTRUMENTR_API_EXPORTER(FUNCTION, OUTPUT_TYPE, ...) \
    R_RegisterCCallable("instrumentr", #FUNCTION, (DL_FUNC)(FUNCTION));

#define INSTRUMENTR_EXPORT_API() INSTRUMENTR_API_MAP(INSTRUMENTR_API_EXPORTER)

#endif /* INSTRUMENTR_API_H */
