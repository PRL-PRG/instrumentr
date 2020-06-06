
#' @export
get_directory <- function(object, ...) {
    UseMethod("get_directory")
}

#' @export
get_directory.lightr_application <- function(object, ...) { # nolint
    .Call(C_application_get_directory, object)
}

#' @export
get_directory.lightr_package <- function(object, ...) {
    .Call(C_package_get_directory, object)
}
