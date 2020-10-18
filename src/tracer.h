#ifndef INSTRUMENTR_TRACER_H
#define INSTRUMENTR_TRACER_H

#include <instrumentr/Rincludes.h>
#include <instrumentr/types.h>
#include "callback_type.h"
#include "exec_stats.h"

/********************************************************************************
 * create
 *******************************************************************************/

instrumentr_tracer_t instrumentr_tracer_create();

SEXP r_instrumentr_tracer_create();

/********************************************************************************
 * interop
 *******************************************************************************/

SEXP instrumentr_tracer_wrap(instrumentr_tracer_t tracer);

instrumentr_tracer_t instrumentr_tracer_unwrap(SEXP r_tracer);

/********************************************************************************
 * reset
 *******************************************************************************/

void instrumentr_tracer_reset(instrumentr_tracer_t tracer);

#ifdef USING_DYNTRACE

/********************************************************************************
 * dyntracer
 *******************************************************************************/

dyntracer_t* instrumentr_tracer_get_dyntracer(instrumentr_tracer_t tracer);

#endif

/********************************************************************************
 * application
 *******************************************************************************/

/*  accessor  */
instrumentr_application_t
instrumentr_tracer_get_application(instrumentr_tracer_t tracer);
SEXP r_instrumentr_tracer_get_application(SEXP r_tracer);

/*  mutator  */
void instrumentr_tracer_set_application(instrumentr_tracer_t tracer,
                                        instrumentr_application_t application);

/*  mutator  */
void instrumentr_tracer_remove_application(instrumentr_tracer_t tracer);

/********************************************************************************
 * r_environment
 *******************************************************************************/

/*  accessor  */
SEXP instrumentr_tracer_get_environment(instrumentr_tracer_t tracer);
SEXP r_instrumentr_tracer_get_environment(SEXP r_tracer);

/********************************************************************************
 * active_callback
 *******************************************************************************/

/* accessor  */
int instrumentr_tracer_is_active(instrumentr_tracer_t tracer);
SEXP r_instrumentr_tracer_is_active(SEXP r_tracer);

/* accessor  */
instrumentr_callback_t
instrumentr_tracer_get_active_callback(instrumentr_tracer_t tracer);
SEXP r_instrumentr_tracer_get_active_callback(SEXP r_tracer);

/* mutator  */
void instrumentr_tracer_set_active_callback(instrumentr_tracer_t tracer,
                                            instrumentr_callback_t callback);

/* mutator  */
void instrumentr_tracer_unset_active_callback(instrumentr_tracer_t tracer);

/********************************************************************************
 * status
 *******************************************************************************/

/* accessor  */
int instrumentr_tracer_is_enabled(instrumentr_tracer_t tracer);
SEXP r_instrumentr_tracer_is_enabled(SEXP r_tracer);

/* mutator  */
void instrumentr_tracer_enable(instrumentr_tracer_t tracer);
SEXP r_instrumentr_tracer_enable(SEXP r_tracer);

/* mutator  */
void instrumentr_tracer_disable(instrumentr_tracer_t tracer);
SEXP r_instrumentr_tracer_disable(SEXP r_tracer);

/* mutator  */
void instrumentr_tracer_reinstate(instrumentr_tracer_t tracer);
SEXP r_instrumentr_tracer_reinstate(SEXP r_tracer);

/* mutator  */
void instrumentr_tracer_set_tracing_status(instrumentr_tracer_t tracer,
                                           int status);

/********************************************************************************
 * packages
 *******************************************************************************/

int instrumentr_tracer_get_traced_package_count(instrumentr_tracer_t tracer);
SEXP r_instrumentr_tracer_get_traced_package_count(SEXP r_tracer);

int instrumentr_tracer_get_traced_function_count(instrumentr_tracer_t tracer,
                                                 const char* package_name);
SEXP r_instrumentr_tracer_get_traced_function_count(SEXP r_tracer,
                                                    SEXP r_package_name);

int instrumentr_tracer_is_package_traced(instrumentr_tracer_t tracer,
                                         const char* name);
SEXP r_instrumentr_tracer_is_package_traced(SEXP r_tracer, SEXP r_package_name);

int instrumentr_tracer_is_function_traced(instrumentr_tracer_t tracer,
                                          const char* package_name,
                                          const char* function_name);
SEXP r_instrumentr_tracer_is_function_traced(SEXP r_tracer,
                                             SEXP r_package_name,
                                             SEXP r_function_name);

SEXP r_instrumentr_tracer_get_traced_packages(SEXP r_tracer);
SEXP r_instrumentr_tracer_get_traced_functions(SEXP r_tracer,
                                               SEXP r_package_name);

void instrumentr_tracer_trace_package(instrumentr_tracer_t tracer,
                                      const char* package_name);
SEXP r_instrumentr_tracer_trace_package(SEXP r_tracer, SEXP r_package_name);

void instrumentr_tracer_trace_function(instrumentr_tracer_t tracer,
                                       const char* package_name,
                                       const char* function_name);
SEXP r_instrumentr_tracer_trace_function(SEXP r_tracer,
                                         SEXP r_package_name,
                                         SEXP r_function_name);

/********************************************************************************
 * callbacks
 *******************************************************************************/

#define TRACER_CALLBACK_API(TYPE, NAME, ...)                                 \
                                                                             \
    /* accessor */                                                           \
    int instrumentr_tracer_has_callback_##NAME(instrumentr_tracer_t tracer); \
    SEXP r_instrumentr_tracer_has_callback_##NAME(SEXP r_tracer);            \
                                                                             \
    /* accessor */                                                           \
    instrumentr_callback_t instrumentr_tracer_get_callback_##NAME(           \
        instrumentr_tracer_t tracer);                                        \
    SEXP r_instrumentr_tracer_get_callback_##NAME(SEXP r_tracer);            \
                                                                             \
    /* accessor */                                                           \
    void instrumentr_tracer_set_callback_##NAME(                             \
        instrumentr_tracer_t tracer, instrumentr_callback_t callback);       \
    SEXP r_instrumentr_tracer_set_callback_##NAME(SEXP r_tracer,             \
                                                  SEXP r_callback);          \
                                                                             \
    /* mutator */                                                            \
    void instrumentr_tracer_remove_callback_##NAME(                          \
        instrumentr_tracer_t tracer);                                        \
    SEXP r_instrumentr_tracer_remove_callback_##NAME(SEXP r_tracer);

INSTRUMENTR_CALLBACK_TYPE_MAP_MACRO(TRACER_CALLBACK_API)

#undef TRACER_CALLBACK_API

/********************************************************************************
 * exec_stats
 *******************************************************************************/

/* accessor */
instrumentr_exec_stats_t
instrumentr_tracer_get_tracing_exec_stats(instrumentr_tracer_t tracer);

/* accessor */
SEXP r_instrumentr_tracer_get_tracing_exec_stats(SEXP r_tracer);

#define TRACER_EXEC_STATS_API(TYPE, NAME, ...)               \
    /* accessor */                                           \
    instrumentr_exec_stats_t                                 \
        instrumentr_tracer_get_callback_##NAME##_exec_stats( \
            instrumentr_tracer_t tracer);                    \
                                                             \
    /* accessor */                                           \
    SEXP r_instrumentr_tracer_get_callback_##NAME##_exec_stats(SEXP r_tracer);

INSTRUMENTR_CALLBACK_TYPE_MAP_MACRO(TRACER_EXEC_STATS_API)

#undef TRACER_EXEC_STATS_API

SEXP r_instrumentr_tracer_get_exec_stats(SEXP r_tracer);

#endif /* INSTRUMENTR_TRACER_H */
