#ifndef INSTRUMENTR_APPLICATION_H
#define INSTRUMENTR_APPLICATION_H

#include <instrumentr/Rincludes.h>
#include <instrumentr/call_stack.h>
#include <instrumentr/package.h>

/*******************************************************************************
 * declaration
 *******************************************************************************/

typedef struct instrumentr_application_impl_t* instrumentr_application_t;

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
SEXP r_instrumentr_application_get_frame_position(
    instrumentr_application_t r_application);

/*******************************************************************************
 * CALL STACK
 *******************************************************************************/

/* accessor */
instrumentr_call_stack_t
instrumentr_application_get_call_stack(instrumentr_application_t application);
SEXP r_instrumentr_application_get_call_stack(SEXP r_application);

/*******************************************************************************
 * PACKAGES
 *******************************************************************************/

/* accessor */
int instrumentr_application_get_package_count(
    instrumentr_application_t application);
SEXP r_instrumentr_application_get_package_count(
    instrumentr_application_t application);

/* accessor */
instrumentr_package_t instrumentr_application_get_package_by_position(
    instrumentr_application_t application,
    int position);
SEXP r_instrumentr_application_get_package_by_position(
    instrumentr_application_t application,
    SEXP r_position);

/* accessor */
instrumentr_package_t instrumentr_application_get_package_by_name(
    instrumentr_application_t application,
    int name);
SEXP r_instrumentr_application_get_package_by_name(
    instrumentr_application_t application,
    SEXP r_name);

/* accessor */
SEXP r_instrumentr_application_get_packages(
    instrumentr_application_t application);

#endif /* INSTRUMENTR_APPLICATION_H */
