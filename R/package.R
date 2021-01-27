#' API for package objects
#'
#' @description
#' A package object models R packages. This is created by instrumentr during
#' tracing when a package is loaded and deleted when the package is unloaded.
#' The package model object provides information about the package and its
#' functions.
#'
#' @details
#'
#' ## Name
#' The package name can be retrieved using `get_name` function.
#'
#' ## Directory
#' The package installation directory can be obtained from `get_directory` function.
#'
#' ## Namespace
#' The package namespace is an environment that contains all of package's
#' bindings. `get_namespace` returns a package's namespace.
#'
#' ## Attached
#' All packages active in an R session are not necessarily imported by the library
#' function. Packages imported by calls to library function or attached
#' explicitly using the `attach` function are added to the search path.
#' The search path is used for symbol lookup during program evaluation.
#' For these attached packages `is_attached` returns `TRUE`.
#' For packages which are loaded but not attached in the R session,
#' `is_attached` returns `FALSE`.
#'
#' ## Functions
#' All functions present in a package (public, private, S3 methods and generics)
#' can be obtained using the `get_functions` method which returns a list of
#' function model objects.
#'
#' @family package api
#'
#' @examples
#' tracer <- create_tracer(package_attach_callback =
#'     function(tracer, callback, application, package) {
#'         cat("Name           : ", get_name(package), "\n")
#'         cat("Directory      : ", get_directory(package), "\n")
#'         cat("Namespace      : ", get_object_details(package)$address, "\n")
#'         cat("Attached       : ", is_attached(package), "\n")
#'         cat("Functions      : ", length(get_functions(package)), "\n")
#'    }
#' )
#'
#' trace_code(tracer, {
#'     library(MASS)
#'     unloadNamespace("MASS")
#' })
#'
#' @name package
NULL

#' @export
#' @rdname package
get_name.instrumentr_package <- function(object) {
    .Call(C_instrumentr_package_get_name, object)
}

#' @export
#' @rdname package
get_directory.instrumentr_package <- function(object) { # nolint
    .Call(C_instrumentr_package_get_directory, object)
}

#' @export
#' @rdname package
get_namespace.instrumentr_package <- function(object) { # nolint
    .Call(C_instrumentr_package_get_namespace, object)
}

## TODO: document
#' @export
#' @rdname package
is_attached.instrumentr_package <- function(object) { # nolint
    .Call(C_instrumentr_package_is_attached, object)
}

#' @export
#' @rdname package
get_functions.instrumentr_package <- function(object) { # nolint
    .Call(C_instrumentr_package_get_functions, object)
}

#' @export
to_string.instrumentr_package <- function(object) {
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

create_package <- function(package_name,
                           package_directory = dirname(system.file(package=package_name)),
                           package_environment = getNamespace(package_name),
                           attached = FALSE) {
    stopifnot(is_scalar_character(package_name))
    stopifnot(is_scalar_character(package_directory))
    stopifnot(is_environment(package_environment))
    stopifnot(is_scalar_logical(attached))

    package_ptr <- .Call(C_instrumentr_package_create, package_name, package_directory, package_environment, attached)

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

