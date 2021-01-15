
#' @export
get_package <- function(application, package_name_or_position, ...) {
    UseMethod("get_package")
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
