#include "application.h"
#include "interop.h"
#include "utilities.h"
#include "state.h"

/********************************************************************************
 * definition
 *******************************************************************************/

struct instrumentr_application_impl_t {
    struct instrumentr_model_impl_t model;
    const char* name;
    const char* directory;
    SEXP r_code;
    SEXP r_environment;
    int frame_position;
};

/********************************************************************************
 * finalize
 *******************************************************************************/

void instrumentr_application_finalize(instrumentr_model_t model) {
    instrumentr_application_t application = (instrumentr_application_t)(model);

    free((char*) (application->name));

    free((char*) (application->directory));

    instrumentr_sexp_release(application->r_code);
    application->r_code = NULL;

    instrumentr_sexp_release(application->r_environment);
    application->r_environment = NULL;

    application->frame_position = 0;
}

/********************************************************************************
 * create
 *******************************************************************************/

instrumentr_application_t
instrumentr_application_create(instrumentr_state_t state,
                               const char* const name,
                               const char* const directory,
                               SEXP r_code,
                               SEXP r_environment,
                               int frame_position) {
    const char* duplicate_name = instrumentr_duplicate_string(name);

    const char* duplicate_directory = instrumentr_duplicate_string(directory);

    instrumentr_model_t model =
        instrumentr_model_create(state,
                                 sizeof(struct instrumentr_application_impl_t),
                                 INSTRUMENTR_MODEL_TYPE_APPLICATION,
                                 instrumentr_application_finalize,
                                 INSTRUMENTR_ORIGIN_LOCAL);

    instrumentr_application_t application = (instrumentr_application_t)(model);

    application->name = duplicate_name;
    application->directory = duplicate_directory;

    application->r_code = r_code;
    instrumentr_sexp_acquire(application->r_code);

    application->r_environment = r_environment;
    instrumentr_sexp_acquire(application->r_environment);

    application->frame_position = frame_position;

    return application;
}

SEXP r_instrumentr_application_create(SEXP r_state,
                                      SEXP r_name,
                                      SEXP r_directory,
                                      SEXP r_code,
                                      SEXP r_environment,
                                      SEXP r_frame_position) {
    instrumentr_state_t state = instrumentr_state_unwrap(r_state);
    const char* const name = instrumentr_r_character_to_c_string(r_name);
    const char* const directory =
        instrumentr_r_character_to_c_string(r_directory);
    int frame_position = instrumentr_r_integer_to_c_int(r_frame_position);
    instrumentr_application_t application = instrumentr_application_create(
        state, name, directory, r_code, r_environment, frame_position);
    SEXP r_application = instrumentr_application_wrap(application);
    instrumentr_model_release(application);
    return r_application;
}

/********************************************************************************
 * interop
 *******************************************************************************/

INSTRUMENTR_MODEL_INTEROP_DEFINE_API(application,
                                     INSTRUMENTR_MODEL_TYPE_APPLICATION);

/*******************************************************************************
 * name
 *******************************************************************************/

/* accessor  */
const char*
instrumentr_application_get_name(instrumentr_application_t application) {
    return application->name;
}

SEXP r_instrumentr_application_get_name(SEXP r_application) {
    instrumentr_application_t application =
        instrumentr_application_unwrap(r_application);
    const char* name = instrumentr_application_get_name(application);
    return instrumentr_c_string_to_r_character(name);
}

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
 * r_environment
 *******************************************************************************/

/* accessor  */
SEXP instrumentr_application_get_environment(
    instrumentr_application_t application) {
    return application->r_environment;
}

SEXP r_instrumentr_application_get_environment(SEXP r_application) {
    instrumentr_application_t application =
        instrumentr_application_unwrap(r_application);
    return instrumentr_application_get_environment(application);
}

/*******************************************************************************
 * frame_position
 *******************************************************************************/

/* accessor  */
int instrumentr_application_get_frame_position(
    instrumentr_application_t application) {
    return application->frame_position;
}

SEXP r_instrumentr_application_get_frame_position(SEXP r_application) {
    instrumentr_application_t application =
        instrumentr_application_unwrap(r_application);
    int frame_position =
        instrumentr_application_get_frame_position(application);
    return instrumentr_c_int_to_r_integer(frame_position);
}
