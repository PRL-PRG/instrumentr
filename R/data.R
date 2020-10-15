
#' @export
has_c_data <- function(object, ...) {
    UseMethod("has_c_data")
}

#' @export
has_c_data.instrumentr_object <- function(object, ...) {
    .Call(C_instrumentr_object_has_c_data, object)
}

#' @export
has_r_data <- function(object, ...) {
    UseMethod("has_r_data")
}

#' @export
has_r_data.instrumentr_object <- function(object, ...) {
    .Call(C_instrumentr_object_has_r_data, object)
}

#' @export
get_c_data <- function(object, ...) {
    UseMethod("get_c_data")
}

#' @export
get_c_data.instrumentr_object <- function(object, ...) {
    .Call(C_instrumentr_object_get_c_data, object)
}

#' @export
get_r_data <- function(object, ...) {
    UseMethod("get_r_data")
}

#' @export
get_r_data.instrumentr_object <- function(object, ...) {
    .Call(C_instrumentr_object_get_r_data, object)
}

#' @export
set_c_data <- function(object, data, ...) {
    UseMethod("set_c_data")
}

#' @export
set_c_data.instrumentr_object <- function(object, data, ...) {
    .Call(C_instrumentr_object_set_c_data, object, data)
    invisible(NULL)
}

#' @export
set_r_data <- function(object, data, ...) {
    UseMethod("set_r_data")
}

#' @export
set_r_data.instrumentr_object <- function(object, data, ...) {
    .Call(C_instrumentr_object_set_r_data, object, data)
    invisible(NULL)
}

#' @export
remove_c_data <- function(object, ...) {
    UseMethod("remove_c_data")
}

#' @export
remove_c_data.instrumentr_object <- function(object, ...) {
    .Call(C_instrumentr_object_remove_c_data, object)
    invisible(NULL)
}

#' @export
remove_r_data <- function(object, ...) {
    UseMethod("remove_r_data")
}

#' @export
remove_r_data.instrumentr_object <- function(object, ...) {
    .Call(C_instrumentr_object_remove_r_data, object)
    invisible(NULL)
}
