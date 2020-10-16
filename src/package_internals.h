#ifndef INSTRUMENTR_PACKAGE_INTERNALS_H
#define INSTRUMENTR_PACKAGE_INTERNALS_H

#include <instrumentr/package.h>

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
int instrumentr_package_get_function_count(instrumentr_package_t package);
SEXP r_instrumentr_package_get_function_count(SEXP r_package);

/* accessor  */
instrumentr_function_t
instrumentr_package_get_function_by_position(instrumentr_package_t package,
                                             int position);
SEXP r_instrumentr_package_get_function_by_position(SEXP r_package,
                                                    SEXP r_position);

/* accessor  */
instrumentr_function_t
instrumentr_package_get_function_by_name(instrumentr_package_t package,
                                         const char* name);
SEXP r_instrumentr_package_get_function_by_name(SEXP r_package, SEXP r_name);

/* accessor  */
SEXP r_instrumentr_package_get_functions(SEXP r_package);

/*  mutator  */
void instrumentr_package_append_function(instrumentr_package_t package,
                                         instrumentr_function_t function);

/*  mutator  */
void instrumentr_package_remove_function(instrumentr_package_t package,
                                         instrumentr_function_t function);
#endif /* INSTRUMENTR_PACKAGE_INTERNALS_H */
