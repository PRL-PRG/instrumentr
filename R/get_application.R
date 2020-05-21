
#' @export
get_application <- function(package, ...) {
    if(missing(package)) {
        .Call(C_lightr_get_application)
    }
    else {
        UseMethod("get_application")
    }
}

#' @export
get_application.lightr_package <- function(package, ...) {
    .Call(C_package_get_application, package)
}
