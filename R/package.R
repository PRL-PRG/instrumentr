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
#' tracer <- create_tracer(package_attach =
#'     function(tracer, callback, state, application, package) {
#'         cat("Name           : ", get_name(package), "\n")
#'         cat("Directory      : ", get_directory(package), "\n")
#'         cat("Namespace      : ", get_object_details(package)$address, "\n")
#'         cat("Attached       : ", is_attached(package), "\n")
#'         cat("Functions      : ", length(get_functions(package)), "\n")
#'    }
#' )
#'
#' trace_code(tracer, {
#'     library("Matrix")
#'     unloadNamespace("Matrix")
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

#' @export
#' @rdname package
is_attached.instrumentr_package <- function(object) { # nolint
    .Call(C_instrumentr_package_is_attached, object)
}

#' @export
#' @rdname package
get_function_count.instrumentr_package <- function(object) {
    .Call(C_instrumentr_package_get_function_count, object)
}

#' @export
#' @rdname package
get_function.instrumentr_package <- function(object, name_or_position) { # nolint
    if(is.character(name_or_position)) {
        .Call(C_instrumentr_package_get_function_by_name, object, name_or_position[1])
    }
    else if(is.numeric(name_or_position)) {
        .Call(C_instrumentr_package_get_function_by_position, object, as.integer(name_or_position)[1])
    }
    else {
        stop("Expecting character or numeric value for `name_or_position` argument to get_function")
    }
}

#' @export
#' @rdname package
has_function.instrumentr_package <- function(object, name) { # nolint
    stopifnot(is_scalar_character(name))
    .Call(C_instrumentr_package_has_function, object, name)
}

#' @export
#' @rdname package
get_functions.instrumentr_package <- function(object) { # nolint
    .Call(C_instrumentr_package_get_functions, object)
}

create_package <- function(application_ptr,
                           package_name,
                           package_directory = dirname(system.file(package=package_name)),
                           package_environment = getNamespace(package_name),
                           attached = FALSE) {
    stopifnot(is_scalar_character(package_name))
    stopifnot(is_scalar_character(package_directory))
    stopifnot(is_environment(package_environment))
    stopifnot(is_scalar_logical(attached))

    package_ptr <- .Call(C_instrumentr_package_create, package_name, package_directory, package_environment, attached)

    function_table <- get_function_table(package_environment)

    for (function_name in ls(envir = function_table, all.names = TRUE)) {

        function_info <- get(function_name, envir=function_table)

        ## NOTE: calling this also adds the function to package
        .Call(C_instrumentr_application_function_map_update_properties,
              application_ptr,
              package_ptr,
              function_name,
              function_info$definition,
              package_environment,
              function_info$public,
              function_info$s3_generic,
              function_info$s3_method)
    }

    message("Added ", length(get_functions(package_ptr)), " functions from ", package_name)

    package_ptr
}

