#include "application.h"
#include "interop.h"
#include "utilities.h"
#include "state.h"
#include "environment.h"

/********************************************************************************
 * definition
 *******************************************************************************/

struct instrumentr_application_impl_t {
    struct instrumentr_model_impl_t model;
    const char* directory;
    SEXP r_code;
    instrumentr_environment_t environment;
};

/********************************************************************************
 * finalize
 *******************************************************************************/

void instrumentr_application_finalize(instrumentr_model_t model) {
    instrumentr_application_t application = (instrumentr_application_t)(model);

    free((char*) (application->directory));

    instrumentr_sexp_release(application->r_code);
    application->r_code = NULL;

    instrumentr_environment_release(application->environment);
    application->environment = NULL;
}

/********************************************************************************
 * create
 *******************************************************************************/

instrumentr_application_t
instrumentr_application_create(instrumentr_state_t state,
                               const char* const directory,
                               SEXP r_code,
                               instrumentr_environment_t environment) {
    instrumentr_model_t model =
        instrumentr_model_create(state,
                                 sizeof(struct instrumentr_application_impl_t),
                                 INSTRUMENTR_MODEL_TYPE_APPLICATION,
                                 instrumentr_application_finalize,
                                 INSTRUMENTR_ORIGIN_LOCAL);

    instrumentr_application_t application = (instrumentr_application_t)(model);

    const char* duplicate_directory = instrumentr_duplicate_string(directory);
    application->directory = duplicate_directory;

    application->r_code = r_code;
    instrumentr_sexp_acquire(application->r_code);

    application->environment = environment;
    instrumentr_environment_acquire(application->environment);

    return application;
}

/********************************************************************************
 * interop
 *******************************************************************************/

INSTRUMENTR_MODEL_INTEROP_DEFINE_API(application,
                                     INSTRUMENTR_MODEL_TYPE_APPLICATION)

/*******************************************************************************
 * directory
 *******************************************************************************/

/* accessor  */
const char*
instrumentr_application_get_directory(instrumentr_application_t application) {
    return application->directory;
}

SEXP r_instrumentr_application_get_directory(SEXP r_application) {
    instrumentr_application_t application =
        instrumentr_application_unwrap(r_application);
    const char* directory = instrumentr_application_get_directory(application);
    return instrumentr_c_string_to_r_character(directory);
}

/*******************************************************************************
 * r_code
 *******************************************************************************/

/* accessor  */
SEXP instrumentr_application_get_code(instrumentr_application_t application) {
    return application->r_code;
}

SEXP r_instrumentr_application_get_code(SEXP r_application) {
    instrumentr_application_t application =
        instrumentr_application_unwrap(r_application);
    return instrumentr_application_get_code(application);
}

/*******************************************************************************
 * environment
 *******************************************************************************/

/* accessor  */
instrumentr_environment_t
instrumentr_application_get_environment(instrumentr_application_t application) {
    return application->environment;
}

SEXP r_instrumentr_application_get_environment(SEXP r_application) {
    instrumentr_application_t application =
        instrumentr_application_unwrap(r_application);
    instrumentr_environment_t environment =
        instrumentr_application_get_environment(application);
    return instrumentr_environment_wrap(environment);
}
