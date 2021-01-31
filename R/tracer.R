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
        print(event)
        callback <- create_callback(func, event)
        print(callback)
        set_callback(tracer, callback)
    },
    functions,
    events)

    tracer
}

#' @export
#' @rdname tracer
get_environment.instrumentr_tracer <- function(object, ...) { # nolint
    .Call(C_instrumentr_tracer_get_environment, object)
}

#' @export
#' @rdname tracer
get_exec_stats.instrumentr_tracer <- function(object, ...) {
    .Call(C_instrumentr_tracer_get_exec_stats, object)
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

