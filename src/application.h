#ifndef INSTRUMENTR_APPLICATION_H
#define INSTRUMENTR_APPLICATION_H

#ifdef __cplusplus
extern "C" {
#endif

#include <instrumentr/Rincludes.h>
#include <instrumentr/types.h>

/********************************************************************************
 * create
 *******************************************************************************/

SEXP r_instrumentr_application_create(SEXP r_state,
                                      SEXP r_name,
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

#ifdef __cplusplus
}
#endif

#endif /* INSTRUMENTR_APPLICATION_H */
