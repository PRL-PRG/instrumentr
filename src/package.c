#include "package.h"
#include "interop.h"
#include "object.h"
#include "vec.h"
#include "utilities.h"

/********************************************************************************
 * definition
 *******************************************************************************/
typedef vec_t(instrumentr_function_t) instrumentr_function_vector_t;

struct instrumentr_package_impl_t {
    struct instrumentr_object_impl_t object;
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

    free((char*)(package->name));
    free((char*)(package->directory));

    instrumentr_sexp_release(package->r_namespace);

    int count = package->functions.length;
    instrumentr_function_t* functions = package->functions.data;

    for (int i = 0; i < count; ++i) {
        instrumentr_function_t function = functions[i];
        instrumentr_object_release(function);
    }

    vec_deinit(&package->functions);
}

/********************************************************************************
 * create
 *******************************************************************************/

instrumentr_package_t instrumentr_package_create(const char* name,
                                                 const char* directory,
                                                 SEXP r_namespace) {
     const char* duplicate_name = instrumentr_duplicate_string(name);

    const char* duplicate_directory = instrumentr_duplicate_string(directory);

    instrumentr_object_t object =
        instrumentr_object_create(sizeof(struct instrumentr_package_impl_t),
                                  INSTRUMENTR_PACKAGE,
                                  instrumentr_package_finalize);

    instrumentr_package_t package = (instrumentr_package_t)(object);

    package->name = duplicate_name;
    package->directory = duplicate_directory;

    instrumentr_sexp_acquire(r_namespace);
    package->r_namespace = r_namespace;

    vec_init(&package->functions);

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
int instrumentr_package_get_function_count(instrumentr_package_t package) {
    return package->functions.length;
}

SEXP r_instrumentr_package_get_function_count(SEXP r_package) {
    instrumentr_package_t package = instrumentr_package_unwrap(r_package);
    int count = instrumentr_package_get_function_count(package);
    return instrumentr_c_int_to_r_integer(count);
}

/* accessor  */
instrumentr_function_t
instrumentr_package_get_function_by_position(instrumentr_package_t package,
                                             int position) {
    int count = instrumentr_package_get_function_count(package);
    if (position < count && position >= 0) {
        return package->functions.data[position];
    } else {
        instrumentr_log_error(
            "attempt to access %d function of a package with %d functions",
            position,
            count);
        /* NOTE: not executed  */
        return NULL;
    }
}

SEXP r_instrumentr_package_get_function_by_position(SEXP r_package,
                                                    SEXP r_position) {
    instrumentr_package_t package = instrumentr_package_unwrap(r_package);
    /* NOTE: 1 based indexing in R */
    int position = instrumentr_r_integer_to_c_int(r_position) - 1;
    instrumentr_function_t function =
        instrumentr_package_get_function_by_position(package, position);
    return instrumentr_function_wrap(function);
}

/* accessor  */
instrumentr_function_t
instrumentr_package_get_function_by_name(instrumentr_package_t package,
                                         const char* name) {
    int count = instrumentr_package_get_function_count(package);
    instrumentr_function_t* functions = package->functions.data;

    for (int i = 0; i < count; ++i) {
        instrumentr_function_t function = functions[i];
        if (strcmp(instrumentr_function_get_name(function), name) == 0) {
            return function;
        }
    }

    instrumentr_log_error(
        "function with name '%s' does not exist for this package", name);

    /* NOTE: not executed  */
    return NULL;
}

SEXP r_instrumentr_package_get_function_by_name(SEXP r_package, SEXP r_name) {
    instrumentr_package_t package = instrumentr_package_unwrap(r_package);
    const char* name = instrumentr_r_character_to_c_string(r_name);
    instrumentr_function_t function =
        instrumentr_package_get_function_by_name(package, name);
    return instrumentr_function_wrap(function);
}

/* accessor  */
SEXP r_instrumentr_package_get_functions(SEXP r_package) {
    instrumentr_package_t package = instrumentr_package_unwrap(r_package);
    instrumentr_function_t* functions = package->functions.data;
    int count = instrumentr_package_get_function_count(package);

    SEXP r_functions = PROTECT(allocVector(VECSXP, count));
    SEXP r_names = PROTECT(allocVector(STRSXP, count));

    for (int i = 0; i < count; ++i) {
        instrumentr_function_t function = functions[i];
        const char* name = instrumentr_function_get_name(function);
        SET_VECTOR_ELT(r_functions, i, instrumentr_function_wrap(function));
        SET_STRING_ELT(r_names, i, mkChar(name));
    }

    Rf_setAttrib(r_functions, R_NamesSymbol, r_names);
    UNPROTECT(2);
    return r_functions;
}

/*  mutator  */
void instrumentr_package_add_function(instrumentr_package_t package,
                                      instrumentr_function_t function) {
    int index = -1;
    vec_find(&package->functions, function, index);
    if (index == -1) {
        vec_push(&package->functions, function);
    } else {
        instrumentr_log_error("function '%s' already added to package %s",
                              instrumentr_function_get_name(function),
                              instrumentr_package_get_name(package));
    }
}

SEXP r_instrumentr_package_add_function(SEXP r_package, SEXP r_function) {
    instrumentr_package_t package = instrumentr_package_unwrap(r_package);
    instrumentr_function_t function = instrumentr_function_unwrap(r_function);
    instrumentr_package_add_function(package, function);
    return R_NilValue;
}


/*  mutator  */
void instrumentr_package_remove_function(instrumentr_package_t package,
                                         instrumentr_function_t function) {
    vec_remove(&package->functions, function);
}
