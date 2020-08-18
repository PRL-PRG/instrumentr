
#' @export
create_call_exit_callback <- function(object) {
    UseMethod("create_call_exit_callback")
}

#' @export
create_call_exit_callback.function <- function(object) {
    stopifnot(is_closure(object) && has_parameters(object, 5))

    .Call(C_call_exit_callback_create_from_r_function, object)
}

#' @export
create_call_exit_callback.externalptr <- function(object) {
    .Call(C_call_exit_callback_create_from_c_function, object)
}
