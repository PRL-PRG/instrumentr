#' API for application objects
#'
#' @description
#' An application object models the code that you want to trace. It is created
#' by instrumentr when tracing begins and passed as an argument to all the
#' callbacks.
#'
#' @details
#'
#' ## Name
#' An application's name is the filepath containing the code that is being
#' traced. This is a best-effort guess, obtained from an attribute of the frame
#' in which file is being sourced or from the `--file` argument passed to R on
#' invocation. If these  tricks fail, the name defaults to `"unknown"`.
#' `get_name` returns the application's name.
#'
#' ## Directory
#' Application's directory is the working directory of the process when the
#' tracing starts. It is not updated dynamically to match the working directory
#' during tracing.
#'
#' ## Code
#' Application's code is the code block that is being traced. This is a quoted R
#' expression.
#'
#' ## Environment
#' Application's environment is the environment in which the code is being
#' evaluated.
#'
#' ## Frame Position
#' Application's frame position is the depth of the call stack when tracing
#' begins. This can be used as an offset for dealing with stack operations
#' during tracing.
#'
#' ## Packages
#' As tracing progresses, packages are loaded, attached, unloaded and detached
#' in the R session by the code being traced. Application object keeps track
#' of these operations and dynamically updates its list of packages. The list of
#' currently loaded or attached packages can be obtained using application's
#' `get_packages` method. The order of package objects in this list reflects the
#' order in which they are loaded or attached. This list also includes packages
#' that are loaded in the R session prior to tracing.
#'
#' @family application api
#'
#' @examples
#' tracer <- create_tracer(tracing_initialization =
#'     function(tracer, callback, application) {
#'         cat("Name           : ", get_name(application), "\n")
#'         cat("Directory      : ", get_directory(application), "\n")
#'         cat("Code           : ", deparse(get_code(application)), "\n")
#'         cat("Frame Position : ", get_frame_position(application), "\n")
#'         cat("Packages       : ", length(get_packages(application)), "\n")
#'         print(get_environment(application))
#'         print(get_call_stack(application))
#'         print(get_package(application, 1))
#'         print(get_package(application, "base"))
#'     }
#' )
#'
#' trace_code(tracer, { library(MASS) } )
#'
#' @name application
NULL


#' @export
#' @rdname application
get_name.instrumentr_application <- function(object) { # nolint
    .Call(C_instrumentr_application_get_name, object)
}


#' @export
#' @rdname application
get_directory.instrumentr_application <- function(object) { # nolint
    .Call(C_instrumentr_application_get_directory, object)
}


#' @export
#' @rdname application
get_code.instrumentr_application <- function(object) { # nolint
    .Call(C_instrumentr_application_get_code, object)
}


#' @export
#' @rdname application
get_environment.instrumentr_application <- function(object) { # nolint
    .Call(C_instrumentr_application_get_environment, object)
}

#' @export
#' @rdname application
get_frame_position.instrumentr_application <- function(object) { # nolint
    .Call(C_instrumentr_application_get_frame_position, object)
}

#' @export
#' @rdname application
get_package_count.instrumentr_application <- function(object) { # nolint
    .Call(C_instrumentr_application_get_package_count, object)
}

#' @export
#' @rdname application
get_package.instrumentr_application <- function(application, name_or_position) { # nolint
    if(is.character(name_or_position)) {
        .Call(C_instrumentr_application_get_package_by_name, application, name_or_position[1])
    }
    else if(is.numeric(name_or_position)) {
        .Call(C_instrumentr_application_get_package_by_position, application, as.integer(name_or_position)[1])
    }
    else {
        stop("Expecting character or numeric value for `name_or_position` argument to get_package")
    }
}


#' @export
#' @rdname application
get_packages.instrumentr_application <- function(application) { # nolint
    .Call(C_instrumentr_application_get_packages, application)
}


#' @export
#' @rdname application
get_call_stack.instrumentr_application <- function(object) { # nolint
    .Call(C_instrumentr_application_get_call_stack, object)
}


create_application <- function(name, directory, code, environment, frame_position) {
    stopifnot(is_scalar_character(name))
    stopifnot(is_scalar_character(directory))
    stopifnot(is_language(code))
    stopifnot(is_environment(environment))
    stopifnot(is_scalar_integer(frame_position))

    .Call(C_instrumentr_application_create, name, directory, code, environment, frame_position)
}

add_package <- function(object, package, ...) {
    UseMethod("add_package")
}

add_package.instrumentr_application <- function(object, package, ...) {
    .Call(C_instrumentr_application_add_package, object, package)
}

