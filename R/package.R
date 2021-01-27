
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

#' Get name of a package object
#' @export
#'
#' @param object A package object
#' @return       Name of the package object
#'
#' @examples
#' tracer <- create_tracer(
#'     package_attach_callback = function(tracer, callback, application, package) {
#'         package_name <- get_name(package)
#'         cat("Attached", package_name, "\n")
#'     }
#' )
#'
#' trace_code(tracer, {
#'     library(stringr)
#'     unloadNamespace('stringr')
#' })
#'
#'
get_name.instrumentr_package <- function(object, ...) {
    .Call(C_instrumentr_package_get_name, object)
}

#' Get the installation directory of a package object
#' @export
#'
#' @param object A package object
#' @return       Installation directory of the package object
#'
#' @examples
#' tracer <- create_tracer(
#'     package_attach_callback = function(tracer, callback, application, package) {
#'         package_name <- get_name(package)
#'         package_directory <- get_directory(package)
#'         cat("Package", package_name, "installed in", package_directory, "\n")
#'     }
#' )
#'
#' trace_code(tracer, {
#'     library(stringr)
#'     unloadNamespace('stringr')
#' })
#'
get_directory.instrumentr_package <- function(object, ...) { # nolint
    .Call(C_instrumentr_package_get_directory, object)
}

#' Get namespace of a package object
#' @export
#'
#' @param object A package object
#' @return       Namespace of the package object
#'
#' @examples
#' tracer <- create_tracer(
#'     package_attach_callback = function(tracer, callback, application, package) {
#'         package_namespace <- get_namespace(package)
#'         print(package_namespace)
#'     }
#' )
#'
#' trace_code(tracer, {
#'     library(stringr)
#'     unloadNamespace('stringr')
#' })
#'
get_namespace.instrumentr_package <- function(object, ...) { # nolint
    .Call(C_instrumentr_package_get_namespace, object)
}

#' Get function objects of a package object
#' @export
#'
#' @param object A package object
#' @return       A named list of the function definitions of the package object
#'
#' @examples
#' tracer <- create_tracer(
#'     package_attach_callback = function(tracer, callback, application, package) {
#'         package_functions <- get_functions(package)
#'         print(package_functions[[1]])
#'     }
#' )
#'
#' trace_code(tracer, {
#'     library(stringr)
#'     unloadNamespace('stringr')
#' })
#'
get_functions.instrumentr_package <- function(object, ...) { # nolint
    .Call(C_instrumentr_package_get_functions, object)
}


#' Get character representation of a package object
#' @export
#'
#' @param object A package object
#' @return       Character representation of the package object
#'
#' @examples
#' tracer <- create_tracer(
#'     package_attach_callback = function(tracer, callback, application, package) {
#'         package_str <- to_string(package)
#'         cat(package_str)
#'     }
#' )
#'
#' trace_code(tracer, {
#'     library(stringr)
#'     unloadNamespace('stringr')
#' })
#'
#' @export
to_string.instrumentr_package <- function(object, ...) {
    representation <- sprintf("Package(name='%s', directory='%s', namespace=%s)",
                              get_name(object),
                              get_directory(object),
                              to_string(get_namespace(object)))

    representation
}

#' Function for getting a character representation of the package object
#' @export
#'
#' @param x      A package object
#' @param ...    Ignored by the function
#' @return       Character representation of the package object
#' @export
print.instrumentr_package <- function(x, ...) {
    cat(to_string(x), "\n")
}
