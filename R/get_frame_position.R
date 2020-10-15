
#' @export
get_frame_position <- function(object, ...) {
    UseMethod("get_frame_position")
}

#' @export
get_frame_position.instrumentr_application <- function(object, ...) { # nolint
    .Call(C_instrumentr_application_get_frame_position, object)
}

#' @export
get_frame_position.instrumentr_call <- function(object, ...) { # nolint
    .Call(C_instrumentr_call_get_frame_position, object)
}
