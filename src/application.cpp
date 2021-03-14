#include "object.h"
#include "application.h"
#include "package.h"
#include "function.h"
#include "interop.h"
#include "utilities.h"
#include "vec.h"
#include "frame.h"
#include "state.h"
#include "call.h"

/********************************************************************************
 * definition
 *******************************************************************************/

typedef vec_t(instrumentr_package_t) instrumentr_package_vector_t;

struct instrumentr_application_impl_t {
    struct instrumentr_object_impl_t object;
    const char* name;
    const char* directory;
    SEXP r_code;
    SEXP r_environment;
    int frame_position;
    instrumentr_package_vector_t packages;
};

/********************************************************************************
 * finalize
 *******************************************************************************/

void instrumentr_application_finalize(instrumentr_object_t object) {
    instrumentr_application_t application = (instrumentr_application_t)(object);

    free((char*) (application->name));

    free((char*) (application->directory));

    instrumentr_sexp_release(application->r_code);
    application->r_code = NULL;

    instrumentr_sexp_release(application->r_environment);
    application->r_environment = NULL;

    application->frame_position = 0;

    int count = application->packages.length;
    instrumentr_package_t* packages = application->packages.data;

    for (int i = 0; i < count; ++i) {
        instrumentr_package_t package = packages[i];
        instrumentr_object_release(package);
    }

    vec_deinit(&application->packages);
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

    instrumentr_object_t object = instrumentr_object_create_and_initialize(
        sizeof(struct instrumentr_application_impl_t),
        state,
        INSTRUMENTR_APPLICATION,
        instrumentr_application_finalize,
        INSTRUMENTR_ORIGIN_FOREIGN);

    instrumentr_application_t application = (instrumentr_application_t)(object);

    application->name = duplicate_name;
    application->directory = duplicate_directory;

    application->r_code = r_code;
    instrumentr_sexp_acquire(application->r_code);

    application->r_environment = r_environment;
    instrumentr_sexp_acquire(application->r_environment);

    application->frame_position = frame_position;

    vec_init(&application->packages);

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
    instrumentr_object_release(application);
    return r_application;
}

/********************************************************************************
 * interop
 *******************************************************************************/

SEXP instrumentr_application_wrap(instrumentr_application_t application) {
    return instrumentr_object_wrap((instrumentr_object_t)(application));
}

instrumentr_application_t instrumentr_application_unwrap(SEXP r_application) {
    instrumentr_object_t object =
        instrumentr_object_unwrap(r_application, INSTRUMENTR_APPLICATION);
    return (instrumentr_application_t)(object);
}

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

/*******************************************************************************
 * packages
 *******************************************************************************/

/* accessor  */
int instrumentr_application_get_package_count(
    instrumentr_application_t application) {
    return application->packages.length;
}

SEXP r_instrumentr_application_get_package_count(SEXP r_application) {
    instrumentr_application_t application =
        instrumentr_application_unwrap(r_application);
    int count = instrumentr_application_get_package_count(application);
    return instrumentr_c_int_to_r_integer(count);
}

/* accessor  */
instrumentr_package_t instrumentr_application_get_package_by_position(
    instrumentr_application_t application,
    int position) {
    int count = instrumentr_application_get_package_count(application);
    if (position < count && position >= 0) {
        return application->packages.data[position];
    } else {
        instrumentr_log_error(
            "attempt to access %d package of a application with %d packages",
            position,
            count);
        /* NOTE: never executed */
        return NULL;
    }
}

SEXP r_instrumentr_application_get_package_by_position(SEXP r_application,
                                                       SEXP r_position) {
    instrumentr_application_t application =
        instrumentr_application_unwrap(r_application);
    /* NOTE: 1 based indexing in R */
    int position = instrumentr_r_integer_to_c_int(r_position) - 1;
    instrumentr_package_t package =
        instrumentr_application_get_package_by_position(application, position);
    return instrumentr_package_wrap(package);
}

/* accessor  */
instrumentr_package_t instrumentr_application_get_package_by_name(
    instrumentr_application_t application,
    const char* name) {
    int count = instrumentr_application_get_package_count(application);
    instrumentr_package_t* packages = application->packages.data;

    for (int i = 0; i < count; ++i) {
        instrumentr_package_t package = packages[i];
        const char* actual_name = instrumentr_package_get_name(package);
        if (actual_name != NULL && strcmp(actual_name, name) == 0) {
            return package;
        }
    }

    instrumentr_log_error(
        "package with name '%s' does not exist for this application", name);
    /* NOTE: never executed */
    return NULL;
}

SEXP r_instrumentr_application_get_package_by_name(SEXP r_application,
                                                   SEXP r_name) {
    instrumentr_application_t application =
        instrumentr_application_unwrap(r_application);
    const char* name = instrumentr_r_character_to_c_string(r_name);
    instrumentr_package_t package =
        instrumentr_application_get_package_by_name(application, name);
    return instrumentr_package_wrap(package);
}

/* accessor */
instrumentr_package_t instrumentr_application_get_base_package(
    instrumentr_application_t application) {
    return instrumentr_application_get_package_by_name(application, "base");
}

SEXP r_instrumentr_application_get_base_package(SEXP r_application) {
    instrumentr_application_t application =
        instrumentr_application_unwrap(r_application);
    instrumentr_package_t package =
        instrumentr_application_get_base_package(application);
    return instrumentr_package_wrap(package);
}

/* accessor */
SEXP r_instrumentr_application_get_packages(SEXP r_application) {
    instrumentr_application_t application =
        instrumentr_application_unwrap(r_application);
    instrumentr_package_t* packages = application->packages.data;
    int count = instrumentr_application_get_package_count(application);

    SEXP r_packages = PROTECT(allocVector(VECSXP, count));
    SEXP r_names = PROTECT(allocVector(STRSXP, count));

    for (int i = 0; i < count; ++i) {
        instrumentr_package_t package = packages[i];
        const char* name = instrumentr_package_get_name(package);
        SET_VECTOR_ELT(r_packages, i, instrumentr_package_wrap(package));
        SET_STRING_ELT(r_names, i, name == NULL ? NA_STRING : mkChar(name));
    }

    Rf_setAttrib(r_packages, R_NamesSymbol, r_names);
    UNPROTECT(2);
    return r_packages;
}

/* mutator */
void instrumentr_application_add_package(instrumentr_application_t application,
                                         instrumentr_package_t package) {
    vec_push(&application->packages, package);

    int fun_count = instrumentr_package_get_function_count(package);
}

/* mutator */
SEXP r_instrumentr_application_add_package(SEXP r_application, SEXP r_package) {
    instrumentr_application_t application =
        instrumentr_application_unwrap(r_application);
    instrumentr_package_t package = instrumentr_package_unwrap(r_package);
    instrumentr_application_add_package(application, package);
    return R_NilValue;
}

/* mutator */
void instrumentr_application_remove_package(
    instrumentr_application_t application,
    instrumentr_state_t state,
    instrumentr_package_t package) {
    vec_remove(&application->packages, package);

    int fun_count = instrumentr_package_get_function_count(package);

    for (int i = 0; i < fun_count; ++i) {
        instrumentr_function_t function =
            instrumentr_package_get_function_by_position(package, i);

        SEXP r_closure = instrumentr_function_get_definition(function).sexp;

        instrumentr_state_function_table_remove(state, r_closure);
    }
}
