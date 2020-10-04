#include <instrumentr/package.h>
#include <instrumentr/memory.h>

/********************************************************************************
 * definition
 *******************************************************************************/

struct instrumentr_package_impl_t {
    instrumentr_object_impl_t object;
    const char* name;
    const char* directory;
    SEXP r_namespace;
    instrumentr_function_vector_t functions;
};

/********************************************************************************
 * finalize
 *******************************************************************************/

void instrumentr_package_finalize(instrumentr_object_t object) {
    instrumentr_package_t package = (instrumentr_package_t)(object);

    free(package->name);
    free(package->directory);

    instrumentr_release_sexp(package->r_namespace);

    instrumentr_object_decrement_reference(package->functions);
}

/********************************************************************************
 * create
 *******************************************************************************/

instrumentr_package_t instrumentr_package_create(const char* name,
                                                 const char* directory,
                                                 SEXP r_namespace) {
    instrumentr_function_vector_t functions =
        instrumentr_function_vector_create();

    const char* duplicate_name = instrumentr_duplicate_string(name);

    const char* duplicate_directory = instrumentr_duplicate_string(directory);

    instrumentr_object_t object =
        instrumentr_object_create(sizeof(instrumentr_package_impl_t),
                                  INSTRUMENTR_PACKAGE,
                                  instrumentr_package_finalize);

    instrumentr_package_t package = (instrumentr_package_t)(object);

    package->name = duplicate_name;
    package->directory = duplicate_directory;

    instrumentr_acquire_sexp(r_namespace);
    package->r_namespace = r_namespace;

    package->functions = functions;

    return package;
}

SEXP r_instrumentr_package_create(SEXP r_name,
                                  SEXP r_directory,
                                  SEXP r_namespace) {
    const char* name = instrumentr_r_character_to_c_string(r_name);
    const char* directory = instrumentr_r_character_to_c_string(r_directory);

    instrumentr_package_t package =
        instrumentr_package_create(name, directory, r_namespace);

    return instrumentr_package_wrap(package);
}

/********************************************************************************
 * interop
 *******************************************************************************/

SEXP instrumentr_package_wrap(instrumentr_package_t package) {
    return instrumentr_object_wrap((instrumentr_object_t)(package));
}

instrumentr_package_t instrumentr_package_unwrap(SEXP r_package) {
    instrumentr_object_t object =
        instrumentr_object_unwrap(r_package, INSTRUMENTR_PACKAGE);
    return (instrumentr_package_t)(object);
}

/********************************************************************************
 * name
 *******************************************************************************/

/* accessor  */
const char* instrumentr_package_get_name(instrumentr_package_t package) {
    return package->name;
}

SEXP r_instrumentr_package_get_name(SEXP r_package) {
    instrumentr_package_t package = instrumentr_package_unwrap(r_package);
    const char* name = instrumentr_package_get_name(package);
    return instrumentr_c_string_to_r_character(name);
}

/********************************************************************************
 * directory
 *******************************************************************************/

/* accessor  */
const char* instrumentr_package_get_directory(instrumentr_package_t package) {
    return package->directory;
}

SEXP r_instrumentr_package_get_directory(SEXP r_package) {
    instrumentr_package_t package = instrumentr_package_unwrap(r_package);
    const char* directory = instrumentr_package_get_directory(package);
    return instrumentr_c_string_to_r_character(directory);
}

/********************************************************************************
 * r_namespace
 *******************************************************************************/

/* accessor  */
SEXP instrumentr_package_get_namespace(instrumentr_package_t package) {
    return package->r_namespace;
}

SEXP r_instrumentr_package_get_namespace(SEXP r_package) {
    instrumentr_package_t package = instrumentr_package_unwrap(r_package);
    return instrumentr_package_get_namespace(package);
}

/********************************************************************************
 * functions
 *******************************************************************************/

/* accessor  */
function_vector_t
instrumentr_package_get_functions(instrumentr_package_t package) {
    return package->functions;
}

SEXP r_instrumentr_package_get_functions(SEXP r_package) {
    instrumentr_package_t package = instrumentr_package_unwrap(r_package);

    instrumentr_function_vector_t functions =
        instrumentr_package_get_functions(package);

    return instrumentr_function_vector_wrap(functions);
}
