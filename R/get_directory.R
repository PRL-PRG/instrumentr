
#' @export
get_directory <- function(object, ...) {
    UseMethod("get_directory")
}

#' @export
get_directory.instrumentr_application <- function(object, ...) { # nolint
    .Call(C_instrumentr_application_get_directory, object)
}

#' @export
get_directory.instrumentr_package <- function(object, ...) { # nolint
    .Call(C_instrumentr_package_get_directory, object)
}
