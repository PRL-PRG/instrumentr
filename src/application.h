#ifndef INSTRUMENTR_APPLICATION_H
#define INSTRUMENTR_APPLICATION_H

#include <instrumentr/Rincludes.h>
#include <instrumentr/types.h>
#include "call_stack.h"
#include "package.h"

/********************************************************************************
 * create
 *******************************************************************************/

SEXP r_instrumentr_application_create(SEXP r_name,
                                      SEXP r_directory,
                                      SEXP r_code,
                                      SEXP r_environment,
                                      SEXP r_frame_position);

/********************************************************************************
 * interop
 *******************************************************************************/

SEXP instrumentr_application_wrap(instrumentr_application_t application);

instrumentr_application_t instrumentr_application_unwrap(SEXP r_application);

/*******************************************************************************
 * name
 *******************************************************************************/

/* accessor */
const char*
instrumentr_application_get_name(instrumentr_application_t application);
SEXP r_instrumentr_application_get_name(SEXP r_application);

/*******************************************************************************
 * directory
 *******************************************************************************/

/* accessor */
const char*
instrumentr_application_get_directory(instrumentr_application_t application);
SEXP r_instrumentr_application_get_directory(SEXP r_application);

/*******************************************************************************
 * code
 *******************************************************************************/

/* accessor */
SEXP instrumentr_application_get_code(instrumentr_application_t application);
SEXP r_instrumentr_application_get_code(SEXP r_application);

/*******************************************************************************
 * environment
 *******************************************************************************/

/* accessor */
SEXP instrumentr_application_get_environment(
    instrumentr_application_t application);
SEXP r_instrumentr_application_get_environment(SEXP r_application);

/*******************************************************************************
 * frame_position
 *******************************************************************************/

/* accessor */
int instrumentr_application_get_frame_position(
    instrumentr_application_t application);
SEXP r_instrumentr_application_get_frame_position(SEXP r_application);

/*******************************************************************************
 * call_stack
 *******************************************************************************/

/* accessor */
instrumentr_call_stack_t
instrumentr_application_get_call_stack(instrumentr_application_t application);
SEXP r_instrumentr_application_get_call_stack(SEXP r_application);

/*******************************************************************************
 * packages
 *******************************************************************************/

/* accessor */
int instrumentr_application_get_package_count(
    instrumentr_application_t application);
SEXP r_instrumentr_application_get_package_count(SEXP r_application);

/* accessor */
instrumentr_package_t instrumentr_application_get_package_by_position(
    instrumentr_application_t application,
    int position);
SEXP r_instrumentr_application_get_package_by_position(SEXP r_application,
                                                       SEXP r_position);

/* accessor */
instrumentr_package_t instrumentr_application_get_package_by_name(
    instrumentr_application_t application,
    const char* name);
SEXP r_instrumentr_application_get_package_by_name(SEXP r_application,
                                                   SEXP r_name);

/* accessor */
instrumentr_package_t
instrumentr_application_get_base_package(instrumentr_application_t application);
SEXP r_instrumentr_application_get_base_package(SEXP r_application);

/* accessor */
SEXP r_instrumentr_application_get_packages(SEXP r_application);

/* mutator  */
void instrumentr_application_add_package(instrumentr_application_t application,
                                         instrumentr_package_t package);
SEXP r_instrumentr_application_add_package(SEXP r_application, SEXP r_package);

/* mutator  */
void instrumentr_application_remove_package(
    instrumentr_application_t application,
    instrumentr_package_t package);

SEXP r_instrumentr_application_remove_package(SEXP r_application,
                                              SEXP r_package);

#endif /* INSTRUMENTR_APPLICATION_H */
