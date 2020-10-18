#ifndef INSTRUMENTR_CALLBACK_H
#define INSTRUMENTR_CALLBACK_H

#include <instrumentr/types.h>
#include <instrumentr/Rincludes.h>
#include "callback_type.h"
#include "exec_stats.h"

/********************************************************************************
 * create
 *******************************************************************************/

#define CALLBACK_CREATE(TYPE, NAME)                                            \
    instrumentr_callback_t                                                     \
        instrumentr_callback_##NAME##_create_from_r_function(SEXP r_function); \
                                                                               \
    SEXP r_instrumentr_callback_##NAME##_create_from_r_function(               \
        SEXP r_function);                                                      \
                                                                               \
    instrumentr_callback_t                                                     \
        instrumentr_callback_##NAME##_create_from_c_function(                  \
            NAME##_function_t c_function);                                     \
                                                                               \
    SEXP r_instrumentr_callback_##NAME##_create_from_c_function(               \
        SEXP r_c_function);

INSTRUMENTR_CALLBACK_TYPE_MAP_MACRO(CALLBACK_CREATE)

#undef CALLBACK_CREATE

/********************************************************************************
 * interop
 *******************************************************************************/

SEXP instrumentr_callback_wrap(instrumentr_callback_t callback);

instrumentr_callback_t instrumentr_callback_unwrap(SEXP r_callback);

/********************************************************************************
 * type
 *******************************************************************************/

/* accessor  */
instrumentr_callback_type_t
instrumentr_callback_get_type(instrumentr_callback_t callback);
SEXP r_instrumentr_callback_get_type(SEXP r_callback);

/* accessor  */
#define CALLBACK_IS_TYPE(TYPE, NAME)                                     \
    int instrumentr_callback_is_##NAME(instrumentr_callback_t callback); \
                                                                         \
    SEXP r_instrumentr_callback_is_##NAME(SEXP r_callback);

INSTRUMENTR_CALLBACK_TYPE_MAP_MACRO(CALLBACK_IS_TYPE)

#undef CALLBACK_IS_TYPE

/********************************************************************************
 * name
 *******************************************************************************/

/* accessor  */
const char* instrumentr_callback_get_name(instrumentr_callback_t callback);
SEXP r_instrumentr_callback_get_name(SEXP r_callback);

/********************************************************************************
 * parameter_count
 *******************************************************************************/

/* accessor  */
int instrumentr_callback_get_parameter_count(instrumentr_callback_t callback);
SEXP r_instrumentr_callback_get_parameter_count(SEXP r_callback);

/********************************************************************************
 * r_function
 *******************************************************************************/

/* accessor  */
int instrumentr_callback_has_r_function(instrumentr_callback_t callback);
SEXP r_instrumentr_callback_has_r_function(SEXP r_callback);

/* accessor  */
SEXP instrumentr_callback_get_r_function(instrumentr_callback_t callback);
SEXP r_instrumentr_callback_get_r_function(SEXP r_callback);

/********************************************************************************
 * c_function
 *******************************************************************************/

/* accessor  */
int instrumentr_callback_has_c_function(instrumentr_callback_t callback);
SEXP r_instrumentr_callback_has_c_function(SEXP r_callback);

/* accessor  */
void* instrumentr_callback_get_c_function(instrumentr_callback_t callback);
SEXP r_instrumentr_callback_get_c_function(SEXP r_callback);

/********************************************************************************
 * active
 *******************************************************************************/

/* accessor  */
int instrumentr_callback_is_active(instrumentr_callback_t callback);
SEXP r_instrumentr_callback_is_active(SEXP r_callback);

/* mutator  */
void instrumentr_callback_activate(instrumentr_callback_t callback);

/* mutator  */
void instrumentr_callback_deactivate(instrumentr_callback_t callback);

/********************************************************************************
 * exec_stats
 *******************************************************************************/

/* accessor  */
instrumentr_exec_stats_t
instrumentr_callback_get_exec_stats(instrumentr_callback_t callback);

/* accessor  */
SEXP r_instrumentr_callback_get_exec_stats(SEXP r_callback);

#endif /* INSTRUMENTR_CALLBACK_H */