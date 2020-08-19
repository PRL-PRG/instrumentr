
#' @export
create_gc_allocation_callback <- function(object) {
    UseMethod("create_gc_allocation_callback")
}

#' @export
create_gc_allocation_callback.function <- function(object) { # nolint
    stopifnot(is_closure(object) && has_parameters(object, 1))

    .Call(C_gc_allocation_callback_create_from_r_function, object)
}

#' @export
create_gc_allocation_callback.externalptr <- function(object) { # nolint
    .Call(C_gc_allocation_callback_create_from_c_function, object)
}
