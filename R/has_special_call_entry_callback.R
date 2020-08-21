
#' @export
has_special_call_entry_callback <- function(object, ...) { # nolint
    UseMethod("has_special_call_entry_callback")
}

#' @export
has_special_call_entry_callback.instrumentr_context <- function(object, ...) { # nolint
    .Call(C_context_has_special_call_entry_callback, object)
}
