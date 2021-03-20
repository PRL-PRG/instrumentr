
#' @export
is_empty.instrumentr_state <- function(state) {
    .Call(C_instrumentr_state_is_empty, state)
}

#' @export
get_size.instrumentr_state <- function(object) {
    .Call(C_instrumentr_state_get_size, object)
}

#' @export
clear.instrumentr_state <- function(state) {
    .Call(C_instrumentr_state_clear, state)
}

#' @export
has_key.instrumentr_state <- function(state, key) {
    .Call(C_instrumentr_state_has_key, state, key)
}

#' @export
as_list.instrumentr_state <- function(state) {
    .Call(C_instrumentr_state_as_list, state)
}

#' @export
lookup.instrumentr_state <- function(state, key, alternative = NULL) {
    .Call(C_instrumentr_state_lookup, state, key, alternative)
}

#' @export
insert.instrumentr_state <- function(state, key, value, overwrite = FALSE) {
    .Call(C_instrumentr_state_insert, state, key, value, overwrite)
}

#' @export
erase.instrumentr_state <- function(state, key) {
    .Call(C_instrumentr_state_erase, state, key)
}

#' @export
#' @rdname state
get_call_stack.instrumentr_state <- function(object) { # nolint
    .Call(C_instrumentr_state_get_call_stack, object)
}

#' @export
#' @rdname state
get_exec_stats.instrumentr_state <- function(object, ...) {
    .Call(C_instrumentr_state_get_exec_stats, object)
}

#' @export
#' @rdname state
get_packages.instrumentr_state <- function(object, ...) {
    .Call(C_instrumentr_state_get_packages, object)
}

#' @export
#' @rdname state
get_namespaces.instrumentr_state <- function(object, ...) {
    .Call(C_instrumentr_state_get_namespaces, object)
}

