#include "object_internals.h"
#include <instrumentr/application.h>
#include "application_internals.h"
#include <instrumentr/package.h>
#include <instrumentr/call_stack.h>
#include "vec.h"

/********************************************************************************
 * definition
 *******************************************************************************/

typedef vec_t(instrumentr_package_t) instrumentr_package_vector_t;

struct instrumentr_application_impl_t {
    instrumentr_object_impl_t object;
    const char* name;
    const char* directory;
    SEXP r_code;
    SEXP r_environment;
    int frame_position;
    instrumentr_call_stack_t call_stack;
    instrumentr_package_vector_t packages;
};

/********************************************************************************
 * finalize
 *******************************************************************************/

void instrumentr_application_finalize(instrumentr_object_t object) {
    instrumentr_application_t application = (instrumentr_application_t)(object);

    free(application -> name);

    free(application->directory);

    instrumentr_release_sexp(application->r_code);
    application->r_code = NULL;

    instrumentr_release_sexp(application->r_environment);
    application->r_environment = NULL;

    application->frame_position = 0;

    instrumentr_object_decrement_reference(instrumentr->call_stack);
    application->call_stack = NULL;

    int count = application->packages.length;
    instrumentr_package_t packages[] = application->packages.data;

    for (int i = 0; i < count; ++i) {
        instrumentr_package_t package = packages[i];
        instrumentr_object_decrement_reference(package);
    }

    vec_deinit(&application->packages);
}

/********************************************************************************
 * create
 *******************************************************************************/

instrumentr_application_t
instrumentr_application_create(const char* const name,
                               const char* const directory,
                               SEXP r_code,
                               SEXP r_environment,
                               int frame_position) {
    const char* duplicate_name = instrumentr_duplicate_string(name);

    const char* duplicate_directory = instrumentr_duplicate_string(directory);

    instrumentr_object_t object =
        instrumentr_object_create(sizeof(instrumentr_application_impl_t),
                                  INSTRUMENTR_APPLICATION,
                                  instrumentr_application_finalize);

    instrumentr_application_t application = (instrumentr_application_t)(object);

    application->name = duplicate_name;
    application->directory = duplicate_directory;

    application->r_code = r_code;
    instrumentr_sexp_acquire(application->r_code);

    application->r_environment = r_environment;
    instrumentr_sexp_acquire(application->r_environment);

    application->frame_position = frame_position;

    application->call_stack = instrumentr_call_stack_create();

    vec_init(&application->packages);

    return application;
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
 * call_stack
 *******************************************************************************/

/* accessor  */
instrumentr_call_stack_t
instrumentr_application_get_call_stack(instrumentr_application_t application) {
    return application->call_stack;
}

SEXP r_instrumentr_application_get_call_stack(SEXP r_application) {
    instrumentr_application_t application =
        instrumentr_application_unwrap(r_application);
    instrumentr_call_stack_t call_stack =
        instrumentr_application_get_call_stack(application);
    return instrumentr_call_stack_wrap(call_stack);
}

/*******************************************************************************
 * packages
 *******************************************************************************/

/* accessor  */
int instrumentr_application_get_package_count(
    instrumentr_application_t application) {
    return application->packages.length;
}

SEXP r_instrumentr_application_get_package_count(
    instrumentr_application_t application) {
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
        instrumentr_raise_error(
            "attempt to access %d package of a application with %d packages",
            position,
            count);
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
instrumentr_package_t
instrumentr_application_get_package_by_name(instrumentr_application_t application,
                                           const char* name) {
    int count = instrumentr_application_get_package_count(application);
    instrumentr_package_t packages[] = application->packages.data;

    for (int i = 0; i < count; ++i) {
        instrumentr_package_t package = packages[i];
        const char* actual_name = instrumentr_package_get_name(package);
        if (actual_name != NULL && strcmp(actual_name, name) == 0) {
            return instrumentr_package_wrap(package);
        }
    }

    instrumentr_raise_errro(
        "package with name '%s' does not exist for this application", name);
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
SEXP r_instrumentr_application_get_packages(SEXP r_application) {
    instrumentr_application_t application =
        instrumentr_application_unwrap(r_application);
    instrumentr_package_t packages[] = application->packages.data;
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
void instrumentr_application_append_package(instrumentr_application_t application,
                                         instrumentr_package_t package) {
    vec_push(&application->packages, package);
}

/* mutator */
SEXP r_instrumentr_application_append_package(SEXP r_application, SEXP r_package) {
    instrumentr_application_t application =
        instrumentr_application_unwrap(r_application);
    instrumentr_package_t package = instrumentr_package_unwrap(r_package);
    instrumentr_application_add_package(application, package);
    return R_NilValue;
}

/* mutator */
void instrumentr_application_remove_package(
    instrumentr_application_t application,
    instrumentr_package_t package) {

    vec_remove(&application->packages, package);
}

