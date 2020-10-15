/*
#ifndef INSTRUMENTR_INSTRUMENTR_H
#define INSTRUMENTR_INSTRUMENTR_H

#include <instrumentr/tracer.h>

#define INSTRUMENTR_TRACER_CALLBACK_API(TYPE, NAME)                         \
    MACRO(instrumentr_tracer_has_callback_##NAME,                           \
          int,                                                              \
          instrumentr_tracer_t tracer)                                      \
    MACRO(r_instrumentr_tracer_has_callback_##NAME, SEXP, SEXP r_tracer)    \
    MACRO(instrumentr_tracer_get_callback_##NAME,                           \
          instrumentr_callback_t,                                           \
          instrumentr_tracer_t tracer)                                      \
    MACRO(r_instrumentr_tracer_get_callback_##NAME, SEXP, SEXP r_tracer)    \
    MACRO(instrumentr_tracer_set_callback_##NAME,                           \
          void,                                                             \
          instrumentr_tracer_t tracer,                                      \
          instrumentr_callback_t callback)                                  \
    MACRO(r_instrumentr_tracer_set_callback_##NAME,                         \
          SEXP,                                                             \
          SEXP r_tracer,                                                    \
          SEXP r_callback)                                                  \
    MACRO(instrumentr_tracer_remove_callback_##NAME,                        \
          void,                                                             \
          instrumentr_tracer_t tracer)                                      \
    MACRO(r_instrumentr_tracer_remove_callback_##NAME, SEXP, SEXP r_tracer)

#define INSTRUMENTR_TRACER_API_MACRO_MAP(MACRO)                                \
    MACRO(SEXP, instrumentr_tracer_wrap, instrumentr_tracer_t tracer)          \
    MACRO(instrumentr_tracer_t, instrumentr_tracer_unwrap, SEXP r_tracer)      \
    MACRO(instrumentr_tracer_get_application,                                  \
          instrumentr_application_t,                                           \
          instrumentr_tracer_t tracer)                                         \
    MACRO(r_instrumentr_tracer_get_application, SEXP, SEXP r_tracer)           \
    MACRO(                                                                     \
        instrumentr_tracer_get_environment, SEXP, instrumentr_tracer_t tracer) \
    MACRO(r_instrumentr_tracer_get_environment, SEXP, SEXP r_tracer)           \
    MACRO(instrumentr_tracer_is_active, int, instrumentr_tracer_t tracer)      \
    MACRO(r_instrumentr_tracer_is_active, SEXP, SEXP r_tracer)                 \
    MACRO(instrumentr_tracer_get_active_callback,                              \
          instrumentr_callback_t,                                              \
          instrumentr_tracer_t tracer)                                         \
    MACRO(r_instrumentr_tracer_get_active_callback, SEXP, SEXP r_tracer)       \
    MACRO(instrumentr_tracer_set_active_callback,                              \
          void,                                                                \
          instrumentr_tracer_t tracer,                                         \
          instrumentr_callback_t callback)                                     \
    MACRO(instrumentr_tracer_unset_active_callback,                            \
          void,                                                                \
          instrumentr_tracer_t tracer)                                         \
    MACRO(instrumentr_tracer_is_enabled, int, instrumentr_tracer_t tracer)     \
    MACRO(r_instrumentr_tracer_is_enabled, SEXP, SEXP r_tracer)                \
    MACRO(instrumentr_tracer_enable, void, instrumentr_tracer_t tracer)        \
    MACRO(r_instrumentr_tracer_enable, SEXP, SEXP r_tracer)                    \
    MACRO(instrumentr_tracer_disable, void, instrumentr_tracer_t tracer)       \
    MACRO(r_instrumentr_tracer_disable, SEXP, SEXP r_tracer)                   \
    MACRO(instrumentr_tracer_reinstate, void, instrumentr_tracer_t tracer)     \
    MACRO(r_instrumentr_tracer_reinstate, SEXP, SEXP r_tracer)                 \
    MACRO(instrumentr_tracer_get_traced_package_count,                         \
          int,                                                                 \
          instrumentr_tracer_t tracer)                                         \
    MACRO(r_instrumentr_tracer_get_traced_package_count, SEXP, SEXP r_tracer)  \
    MACRO(instrumentr_tracer_get_traced_function_count,                        \
          int,                                                                 \
          instrumentr_tracer_t tracer,                                         \
          const char* package_name)                                            \
    MACRO(r_instrumentr_tracer_get_traced_function_count,                      \
          SEXP,                                                                \
          SEXP r_tracer,                                                       \
          SEXP r_package_name)                                                 \
    MACRO(instrumentr_tracer_is_package_traced,                                \
          int,                                                                 \
          instrumentr_tracer_t tracer,                                         \
          const char* name)                                                    \
    MACRO(r_instrumentr_tracer_is_package_traced,                              \
          SEXP,                                                                \
          SEXP r_tracer,                                                       \
          SEXP r_package_name)                                                 \
    MACRO(instrumentr_tracer_is_function_traced,                               \
          int,                                                                 \
          instrumentr_tracer_t tracer,                                         \
          const char* package_name,                                            \
          const char* function_name)                                           \
    MACRO(r_instrumentr_tracer_is_function_traced,                             \
          SEXP,                                                                \
          SEXP r_tracer,                                                       \
          SEXP r_package_name,                                                 \
          SEXP r_function_name)                                                \
    MACRO(r_instrumentr_tracer_get_traced_packages, SEXP, SEXP r_tracer)       \
    MACRO(r_instrumentr_tracer_get_traced_functions,                           \
          SEXP,                                                                \
          SEXP r_tracer,                                                       \
          SEXP r_package_name)                                                 \
    MACRO(instrumentr_tracer_trace_package,                                    \
          void,                                                                \
          instrumentr_tracer_t tracer,                                         \
          const char* package_name)                                            \
    MACRO(r_instrumentr_tracer_trace_package,                                  \
          SEXP,                                                                \
          SEXP r_tracer,                                                       \
          SEXP r_package_name)                                                 \
    MACRO(instrumentr_tracer_trace_function,                                   \
          void,                                                                \
          instrumentr_tracer_t tracer,                                         \
          const char* package_name,                                            \
          const char* function_name)                                           \
    MACRO(r_instrumentr_tracer_trace_function,                                 \
          SEXP,                                                                \
          SEXP r_tracer,                                                       \
          SEXP r_package_name,                                                 \
          SEXP r_function_name)                                                \

INSTRUMENTR_API_MACRO_MAP(MACRO)
INSTRUMENTR_TRACER_API_MACRO_MAP(MACRO)

INSTRUMENTR_CALLBACK_TYPE_MAP_MACRO(TRACER_CALLBACK_API)

#endif /* INSTRUMENTR_INSTRUMENTR_H */
*/
