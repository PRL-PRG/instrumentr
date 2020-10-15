#ifndef INSTRUMENTR_OBJECT_H
#define INSTRUMENTR_OBJECT_H

#include <instrumentr/Rincludes.h>

typedef int instrumentr_id_t;

typedef struct instrumentr_object_impl_t* instrumentr_object_t;

typedef void (*instrumentr_object_finalizer_t)(instrumentr_object_t object);

/*******************************************************************************
 * reference_count
 *******************************************************************************/

/* mutator  */
int instrumentr_object_acquire(void* object);

/* mutator  */
int instrumentr_object_release(void* object);

/*******************************************************************************
 * id
 *******************************************************************************/

/* accessor */
instrumentr_id_t instrumentr_object_get_id(void* object);
SEXP r_instrumentr_object_get_id(SEXP r_object);

/*******************************************************************************
 * c_data
 *******************************************************************************/

/* accessor */
int instrumentr_object_has_c_data(void* object);
SEXP r_instrumentr_object_has_c_data(SEXP r_object);

/* mutator  */
void instrumentr_object_set_c_data(void* object, void* c_data);
SEXP r_instrumentr_object_set_c_data(SEXP r_object, SEXP r_c_data);

/* accessor */
void* instrumentr_object_get_c_data(void* object);
SEXP r_instrumentr_object_get_c_data(SEXP r_object);

/* mutator  */
void instrumentr_object_remove_c_data(void* object);
SEXP r_instrumentr_object_remove_c_data(SEXP r_object);

/*******************************************************************************
 * r_data
 *******************************************************************************/

/* accessor */
int instrumentr_object_has_r_data(void* object);
SEXP r_instrumentr_object_has_r_data(SEXP r_object);

/* mutator  */
void instrumentr_object_set_r_data(void* object, SEXP r_data);
SEXP r_instrumentr_object_set_r_data(SEXP r_object, SEXP r_data);

/* accessor */
SEXP instrumentr_object_get_r_data(void* object);
SEXP r_instrumentr_object_get_r_data(SEXP r_object);

/* mutator  */
void instrumentr_object_remove_r_data(void* object);
SEXP r_instrumentr_object_remove_r_data(SEXP r_object);

#endif /* INSTRUMENTR_OBJECT_H */
