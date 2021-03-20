#' API for environment objects
#'
#' @description
#' A environment object models R environments. This is created by instrumentr during
#' tracing when a environment is loaded and deleted when the environment is unloaded.
#' The environment model object provides information about the environment and its
#' functions.
#'
#' @details
#'
#' ## Name
#' The environment name can be retrieved using `get_name` function.
#'
#' ## Directory
#' The environment installation directory can be obtained from `get_directory` function.
#'
#' ## Namespace
#' The environment namespace is an environment that contains all of environment's
#' bindings. `get_namespace` returns a environment's namespace.
#'
#' ## Attached
#' All environments active in an R session are not necessarily imported by the library
#' function. Environments imported by calls to library function or attached
#' explicitly using the `attach` function are added to the search path.
#' The search path is used for symbol lookup during program evaluation.
#' For these attached environments `is_attached` returns `TRUE`.
#' For environments which are loaded but not attached in the R session,
#' `is_attached` returns `FALSE`.
#'
#' ## Functions
#' All functions present in a environment (public, private, S3 methods and generics)
#' can be obtained using the `get_functions` method which returns a list of
#' function model objects.
#'
#' @family environment api
#'
#' @examples
#' tracer <- create_tracer(package_attach =
#'     function(tracer, callback, state, application, environment) {
#'         cat("Name           : ", get_name(environment), "\n")
#'         cat("Namespace      : ", get_object_details(get_environment(environment)$address), "\n")
#'         cat("Bindings       : ", length(get_size(environment)), "\n")
#'    }
#' )
#'
#' trace_code(tracer, {
#'     library("Matrix")
#'     unloadNamespace("Matrix")
#' })
#'
#' @name environment
NULL

#' @export
#' @rdname environment
get_name.instrumentr_environment <- function(object) {
    .Call(C_instrumentr_environment_get_name, object)
}

#' @export
#' @rdname environment
get_environment.instrumentr_environment <- function(object) { # nolint
    .Call(C_instrumentr_environment_get_environment, object)
}

#' @export
#' @rdname environment
get_size.instrumentr_environment <- function(object) {
    .Call(C_instrumentr_environment_get_size, object)
}

#' @export
#' @rdname environment
lookup.instrumentr_environment <- function(object, name) { # nolint
    .Call(C_instrumentr_environment_lookup, object, name)
}

#' @export
#' @rdname environment
contains.instrumentr_environment <- function(object, name) { # nolint
    .Call(C_instrumentr_environment_contains, object, name)
}

#' @export
#' @rdname environment
get_bindings.instrumentr_environment <- function(object) { # nolint
    .Call(C_instrumentr_environment_get_bindings, object)
}
