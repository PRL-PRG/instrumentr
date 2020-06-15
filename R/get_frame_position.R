
#' @export
get_frame_position <- function(object, ...) {
    UseMethod("get_frame_position")
}

#' @export
get_frame_position.instrumentr_call <- function(object, ...) { # nolint
    .Call(C_call_get_frame_position, object)
}
