#include "object_layout.h"
#include <instrumentr/application.h>
#include <instrumentr/package.h>
#include <instrumentr/call_stack.h>
#include <instrumentr/callback.h>

/********************************************************************************
 * declaration
 *******************************************************************************/

typedef struct instrumentr_tracer_impl_t* instrumentr_tracer_t;

/********************************************************************************
 * application
 *******************************************************************************/

/*  accessor  */
instrumentr_application_t
instrumentr_tracer_get_application(instrumentr_tracer_t tracer);
SEXP r_instrumentr_tracer_get_application(SEXP r_tracer);

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
void r_instrumentr_tracer_disable(instrumentr_tracer_t tracer);

/* mutator  */
void instrumentr_tracer_reinstate(instrumentr_tracer_t tracer);
void r_instrumentr_tracer_reinstate(instrumentr_tracer_t tracer);

/********************************************************************************
 * callbacks
 *******************************************************************************/

#define TRACER_CALLBACK_API(TYPE, NAME)                                       \
                                                                              \
    /* accessor */                                                            \
    int instrumentr_tracer_has_##NAME##_callback(instrumentr_trace_t tracer); \
    SEXP r_instrumentr_tracer_has_##NAME##_callback(SEXP r_tracer);           \
                                                                              \
    /* accessor */                                                            \
    instrumentr_callback_t instrumentr_tracer_get_##NAME##_callback(          \
        instrumentr_trace_t tracer);                                          \
    SEXP r_instrumentr_tracer_get_##NAME##_callback(SEXP r_tracer);           \
                                                                              \
    /* accessor */                                                            \
    void instrumentr_tracer_set_##NAME##_callback(                            \
        instrumentr_trace_t tracer, instrumentr_callback_t callback);         \
    SEXP r_instrumentr_tracer_set_##NAME##_callback(SEXP r_tracer,            \
                                                    SEXP r_callback);         \
                                                                              \
    /* mutator */                                                             \
    void instrumentr_tracer_remove_##NAME##_callback(                         \
        instrumentr_trace_t tracer);                                          \
    SEXP r_instrumentr_tracer_remove_##NAME##_callback(SEXP r_tracer);

INSTRUMENTR_CALLBACK_TYPE_MAP_MACRO(TRACER_CALLBACK_API)

#undef TRACER_CALLBACK_API
#undef INSTRUMENTR_CALLBACK_TYPE_MAP_MACRO
