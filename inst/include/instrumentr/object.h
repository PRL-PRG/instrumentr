#ifndef INSTRUMENTR_OBJECT_H
#define INSTRUMENTR_OBJECT_H

#include "Rincludes.h"

typedef struct instrumentr_object_impl_t* instrumentr_object_t;

typedef void (*instrumentr_object_finalizer_t)(instrumentr_object_t object);

/*******************************************************************************
 * reference_count
 *******************************************************************************/

/* mutator  */
int instrumentr_object_increment_reference(instrumentr_object_t object);

/* mutator  */
int instrumentr_object_decrement_reference(instrumentr_object_t object);

/*******************************************************************************
 * id
 *******************************************************************************/

/* accessor */
instrumentr_id_t instrumentr_object_get_id(instrumentr_object_t object);
SEXP r_instrumentr_object_get_id(SEXP r_object);

/*******************************************************************************
 * c_data
 *******************************************************************************/

/* accessor */
int instrumentr_object_has_c_data(instrumentr_object_t object);
SEXP r_instrumentr_object_has_c_data(SEXP r_object);

/* mutator  */
void instrumentr_object_set_c_data(instrumentr_object_t object, void* c_data);
SEXP r_instrumentr_object_set_c_data(SEXP r_object, SEXP r_c_data);

/* accessor */
void* instrumentr_object_get_c_data(instrumentr_object_t object);
SEXP r_instrumentr_object_get_c_data(SEXP r_object);

/* mutator  */
void instrumentr_object_remove_c_data(instrumentr_object_t object);
SEXP r_instrumentr_object_remove_c_data(SEXP r_object);

/*******************************************************************************
 * r_data
 *******************************************************************************/

/* accessor */
int instrumentr_object_has_r_data(instrumentr_object_t object);
SEXP r_instrumentr_object_has_r_data(SEXP r_object);

/* mutator  */
void instrumentr_object_set_r_data(instrumentr_object_t object, SEXP r_data);
SEXP r_instrumentr_object_set_r_data(SEXP r_object, SEXP r_data);

/* accessor */
SEXP instrumentr_object_get_r_data(instrumentr_object_t object);
SEXP r_instrumentr_object_get_r_data(SEXP r_object);

/* mutator  */
void instrumentr_object_remove_r_data(instrumentr_object_t object);
SEXP r_instrumentr_object_remove_r_data(SEXP r_object);

#endif /* INSTRUMENTR_OBJECT_H */
