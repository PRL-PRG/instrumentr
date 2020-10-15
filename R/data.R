

#' @export
has_data <- function(object, ...) {
    UseMethod("has_data")
}

#' @export
has_data.instrumentr_object <- function(object, ...) {
    .Call(C_instrumentr_object_has_data, object)
}


#' @export
get_data <- function(object, ...) {
    UseMethod("get_data")
}

#' @export
get_data.instrumentr_object <- function(object, ...) {
    .Call(C_instrumentr_object_get_data, object)
}


#' @export
set_data <- function(object, data, ...) {
    UseMethod("set_data")
}

#' @export
set_data.instrumentr_object <- function(object, data, ...) {
    .Call(C_instrumentr_object_set_data, object, data)
    invisible(NULL)
}

#' @export
remove_data <- function(object, ...) {
    UseMethod("remove_data")
}

#' @export
remove_data.instrumentr_object <- function(object, ...) {
    .Call(C_instrumentr_object_remove_data, object)
    invisible(NULL)
}
