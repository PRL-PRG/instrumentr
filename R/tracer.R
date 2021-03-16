#' API for tracer objects
#'
#' @description
#' TODO
#'
#' @name tracer
NULL

#' @export
#' @rdname tracer
create_tracer <- function(...) {
    #environment <- new.env(parent = sys.frame(-1))

    tracer <- .Call(C_instrumentr_tracer_create)#, environment)

    functions <- list(...)
    events <- names(functions)

    Map(function(func, event) {
        callback <- create_callback(func, event)
        set_callback(tracer, callback)
    },
    functions,
    events)

    tracer
}

#' @export
#' @rdname tracer
get_state.instrumentr_tracer <- function(object) {
    .Call(C_instrumentr_tracer_get_state, object)
}

#' @export
#' @rdname tracer
get_application.instrumentr_tracer <- function(object) { # nolint
    .Call(C_instrumentr_tracer_get_application, object)
}

#' @export
#' @rdname tracer
get_environment.instrumentr_tracer <- function(object, ...) { # nolint
    .Call(C_instrumentr_tracer_get_environment, object)
}

#' @export
#' @rdname tracer
get_callback.instrumentr_tracer <- function(tracer, event) {
    .Call(C_instrumentr_tracer_get_callback, tracer, event)
}

#' @export
#' @rdname tracer
set_callback.instrumentr_tracer <- function(tracer, callback) { # nolint

    stopifnot(is_callback(callback))

    .Call(C_instrumentr_tracer_set_callback, tracer, callback)

    invisible(NULL)
}

#' @export
#' @rdname tracer
has_callback.instrumentr_tracer <- function(tracer, event) { # nolint
    .Call(C_instrumentr_tracer_has_callback, tracer, event)
}

#' @export
#' @rdname tracer
remove_callback.instrumentr_tracer <- function(tracer, event) { # nolint
    .Call(C_instrumentr_tracer_remove_callback, tracer, event)
}

#' @export
#' @rdname tracer
is_active.instrumentr_tracer <- function(object) {
    .Call(C_instrumentr_tracer_is_active, object)
}

#' @export
#' @rdname tracer
is_enabled.instrumentr_tracer <- function(tracer) {
    .Call(C_instrumentr_tracer_is_enabled, tracer)
}

#' @export
#' @rdname tracer
enable.instrumentr_tracer <- function(tracer) {
    .Call(C_instrumentr_tracer_enable, tracer)
}

#' @export
#' @rdname tracer
disable.instrumentr_tracer <- function(tracer) {
    .Call(C_instrumentr_tracer_disable, tracer)
}

#' @export
#' @rdname tracer
reinstate.instrumentr_tracer <- function(tracer) {
    .Call(C_instrumentr_tracer_reinstate, tracer)
}

#' @export
#' @rdname tracer
get_active_callback.instrumentr_tracer <- function(tracer) {
    .Call(C_instrumentr_tracer_get_active_callback, tracer)
}
