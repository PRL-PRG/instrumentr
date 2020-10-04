#ifndef INSTRUMENTR_PACKAGE_H
#define INSTRUMENTR_PACKAGE_H

#include <instrumentr/Rincludes.h>
#include <instrumentr/function_vector.h>

/********************************************************************************
 * declaration
 *******************************************************************************/

typedef struct instrumentr_package_impl_t* instrumentr_package_t;

/********************************************************************************
 * create
 *******************************************************************************/

instrumentr_package_t instrumentr_package_create(const char* name,
                                                 const char* directory,
                                                 SEXP r_namespace);

SEXP r_instrumentr_package_create(SEXP r_name,
                                  SEXP r_directory,
                                  SEXP r_namespace);

/********************************************************************************
 * interop
 *******************************************************************************/

SEXP instrumentr_package_wrap(instrumentr_package_t package);

instrumentr_package_t instrumentr_package_unwrap(SEXP r_package);

/********************************************************************************
 * name
 *******************************************************************************/

/* accessor  */
const char* instrumentr_package_get_name(instrumentr_package_t package);
SEXP r_instrumentr_package_get_name(SEXP r_package);

/********************************************************************************
 * directory
 *******************************************************************************/

/* accessor  */
const char* instrumentr_package_get_directory(instrumentr_package_t package);
SEXP r_instrumentr_package_get_directory(SEXP r_package);

/********************************************************************************
 * r_namespace
 *******************************************************************************/

/* accessor  */
SEXP instrumentr_package_get_namespace(instrumentr_package_t package);
SEXP r_instrumentr_package_get_namespace(SEXP r_package);

/********************************************************************************
 * functions
 *******************************************************************************/

/* accessor  */
function_vector_t
instrumentr_package_get_functions(instrumentr_package_t package);
SEXP r_instrumentr_package_get_functions(SEXP r_package);

#endif /* INSTRUMENTR_PACKAGE_H */
