#ifndef INSTRUMENTR_CALLBACK_H
#define INSTRUMENTR_CALLBACK_H

#include <Rincludes.h>

/********************************************************************************
 * create
 *******************************************************************************/

#define INSTRUMENTR_GENERATE_DECLARATION_CALLBACK_CREATE(TYPE, NAME)          \
    instrumentr_callback_t instrumentr_callback_create_NAME_from_r_function(  \
        SEXP r_function);                                                     \
                                                                              \
    SEXP r_instrumentr_callback_create_NAME_from_r_function(SEXP r_function); \
                                                                              \
    instrumentr_callback_t instrumentr_callback_create_NAME_from_c_function(  \
        void* c_function);                                                    \
                                                                              \
    SEXP r_instrumentr_callback_create_NAME_from_c_function(SEXP r_c_function);

INSTRUMENTR_CALLBACK_TYPE_MAP_MACRO(
    INSTRUMENTR_GENERATE_DECLARATION_CALLBACK_CREATE)

#undef INSTRUMENTR_GENERATE_DECLARATION_CALLBACK_CREATE

/********************************************************************************
 * interop
 *******************************************************************************/

SEXP instrumentr_callback_wrap(instrumentr_callback_t callback);

instrumentr_callback_t instrumentr_callback_unwrap(SEXP r_callback);

/********************************************************************************
 * type
 *******************************************************************************/

/* accessor  */
enum instrumentr_callback_type_t
instrumentr_callback_get_type(instrumentr_callback_t callback);
SEXP r_instrumentr_callback_get_type(SEXP r_callback);

/* accessor  */
#define INSTRUMENTR_GENERATE_DECLARATION_CALLBACK_IS_TYPE(TYPE, NAME)    \
    int instrumentr_callback_is_##NAME(instrumentr_callback_t callback); \
                                                                         \
    SEXP r_instrumentr_callback_is_##NAME(SEXP r_callback);

INSTRUMENTR_CALLBACK_TYPE_MAP_MACRO(
    INSTRUMENTR_GENERATE_DECLARATION_CALLBACK_IS_TYPE)

#undef INSTRUMENTR_GENERATE_DECLARATION_CALLBACK_IS_TYPE

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
SEXP r_instrumentr_callback_activate(SEXP r_callback);

/* mutator  */
void instrumentr_callback_deactivate(instrumentr_callback_t callback);
void r_instrumentr_callback_deactivate(instrumentr_callback_t callback);

/* mutator  */
void instrumentr_callback_reinstate(instrumentr_callback_t callback);
void r_instrumentr_callback_reinstate(instrumentr_callback_t callback);

#endif /* INSTRUMENTR_CALLBACK_H */
