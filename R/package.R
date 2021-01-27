
#' Package Functions

create_package <- function(package_name,
                           package_directory = dirname(system.file(package=package_name)),
                           package_environment = getNamespace(package_name)) {
    stopifnot(is_scalar_character(package_name))
    stopifnot(is_scalar_character(package_directory))
    stopifnot(is_environment(package_environment))

    package_ptr <- .Call(C_instrumentr_package_create, package_name, package_directory, package_environment)

    function_table <- get_function_table(package_environment)

    for (function_name in ls(function_table)) {

        function_info <- get(function_name, envir=function_table)

        function_ptr <- create_function(function_name,
                                        length(formals(function_info$definition)),
                                        function_info$definition,
                                        function_info$public,
                                        function_info$s3_generic,
                                        function_info$s3_method)

        .Call(C_instrumentr_package_add_function, package_ptr, function_ptr)
    }

    message("Added ", length(get_functions(package_ptr)), " functions from ", package_name)

    package_ptr
}

#' @param package Package object
#' @return Name of the package
#' @export
get_name.instrumentr_package <- function(object, ...) {
    .Call(C_instrumentr_package_get_name, object)
}

#' @export
get_directory.instrumentr_package <- function(object, ...) { # nolint
    .Call(C_instrumentr_package_get_directory, object)
}

#' @export
get_namespace.instrumentr_package <- function(object, ...) { # nolint
    .Call(C_instrumentr_package_get_namespace, object)
}

#' @export
get_functions.instrumentr_package <- function(object, ...) { # nolint
    .Call(C_instrumentr_package_get_functions, object)
}

#' @export
to_string.instrumentr_package <- function(object, ...) {
    representation <- sprintf("Package(name='%s', directory='%s', namespace=%s)",
                              get_name(object),
                              get_directory(object),
                              to_string(get_namespace(object)))

    representation
}

#' @export
print.instrumentr_package <- function(x, ...) {
    cat(to_string(x), "\n")
}
