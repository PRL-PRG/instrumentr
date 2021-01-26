
infer_application_name <- function() {
    application_name <- NULL

    ## this works if script is sourced inside R session
    tryCatch({
        application_name <- sys.frame(1)$ofile
        if (!is.null(application_name)) {
            application_name <- substr(application_name,
                                       1,
                                       ## ignore `.R` suffix
                                       nchar(application_name) - 2)
        }
    }, error = function(e) {
    })

    if (is.null(application_name)) {
        ## this works if script is started from command line as
        ## R --file=<application_name>.R
        args <- commandArgs(trailingOnly = FALSE)
        for (arg in args) {
            if (startsWith(arg, "--file=")) {
                application_name <- substr(arg,
                                           ## ignore `--file=` prefix
                                           nchar("--file=") + 1,
                                           ## ignore `.R` suffix
                                           nchar(arg) - 2)
                break
            }
        }
    }

    if (is.null(application_name)) {
        application_name <- "<unknown>"
    }

    application_name
}

create_application <- function(name, directory, code, environment, frame_position) {
    stopifnot(is_scalar_character(name))
    stopifnot(is_scalar_character(directory))
    stopifnot(is_language(code))
    stopifnot(is_environment(environment))
    stopifnot(is_scalar_integer(frame_position))

    .Call(C_instrumentr_application_create, name, directory, code, environment, frame_position)
}

#' @export
get_code.instrumentr_application <- function(object, ...) { # nolint
    .Call(C_instrumentr_application_get_code, object)
}

#' @export
add_package <- function(object, package, ...) {
    UseMethod("add_package")
}

#' @export
add_package.instrumentr_application <- function(object, package, ...) {
    .Call(C_instrumentr_application_add_package, object, package)
}

#' @export
get_environment.instrumentr_application <- function(object, ...) { # nolint
    .Call(C_instrumentr_application_get_environment, object)
}

#' @export
get_directory.instrumentr_application <- function(object, ...) { # nolint
    .Call(C_instrumentr_application_get_directory, object)
}

#' @export
get_frame_position.instrumentr_application <- function(object, ...) { # nolint
    .Call(C_instrumentr_application_get_frame_position, object)
}

#' @export
get_name.instrumentr_application <- function(object, ...) { # nolint
    .Call(C_instrumentr_application_get_name, object)
}

#' @export
get_package.instrumentr_application <- function(application, package_name_or_position, ...) { # nolint
    if(is.character(package_name_or_position)) {
        .Call(C_instrumentr_application_get_package_by_name, application, package_name_or_position[1])
    }
    else if(is.numeric(package_name_or_position)) {
        .Call(C_instrumentr_application_get_package_by_position, application, as.integer(package_name_or_position)[1])
    }
    else {
        stop("Expecting character or numeric value for `parameter_name_or_position` argument to get_package")
    }
}

#' @export
get_packages.instrumentr_application <- function(application, ...) { # nolint
    .Call(C_instrumentr_application_get_packages, application)
}

#' @export
get_call_stack.instrumentr_application <- function(object, ...) { # nolint
    .Call(C_instrumentr_application_get_call_stack, object)
}

#' @export
print.instrumentr_application <- function(x, ...) {
    cat(to_string(x), "\n")
}

#' @export
to_string.instrumentr_application <- function(object, ...) { # nolint
    representation <- sprintf("Application(name='%s', directory='%s', environment=%s)",
                              get_name(object),
                              get_directory(object),
                              to_string(get_environment(object)))

    representation
}

