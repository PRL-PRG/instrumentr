#ifndef INSTRUMENTR_APPLICATION_INTERNALS_H
#define INSTRUMENTR_APPLICATION_INTERNALS_H

#include <instrumentr/application.h>

/********************************************************************************
 * create
 *******************************************************************************/

SEXP r_instrumentr_application_create(SEXP r_name,
                                      SEXP r_directory,
                                      SEXP r_code,
                                      SEXP r_environment,
                                      SEXP r_frame_position);

/*******************************************************************************
 * packages
 *******************************************************************************/

/* mutator  */
void instrumentr_application_append_package(
    instrumentr_application_t application,
    instrumentr_package_t package);
SEXP r_instrumentr_application_append_package(SEXP r_application,
                                              SEXP r_package);

/* mutator  */
void instrumentr_application_remove_package(
    instrumentr_application_t application,
    instrumentr_package_t package);

#endif /* INSTRUMENTR_APPLICATION_INTERNALS_H */
