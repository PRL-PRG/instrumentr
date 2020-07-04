
#' @export
is_tracing_enabled <- function() {
    .Call(C_instrumentr_is_tracing_enabled)
}

#' @export
disable_tracing <- function() {
    .Call(C_instrumentr_disable_tracing)
    invisible(NULL)
}

#' @export
enable_tracing <- function() {
    .Call(C_instrumentr_enable_tracing)
    invisible(NULL)
}

#' @export
reinstate_tracing <- function() {
    .Call(C_instrumentr_reinstate_tracing)
    invisible(NULL)
}

#' @export
trace_code <- function(code, context, environment = .GlobalEnv, quote = TRUE) {

    if (quote) {
        code <- substitute(code)
    }

    stopifnot(is_instrumentr_context(context))

    result <- NULL

    .Call(C_context_initialize_tracing, context)

    tryCatch({
        ## nolint NOTE: we manually account for the following four stack frames
        ## nolint introduced by tryCatch in excess of the existing frames:
        ## nolint - tryCatch({ <all code> })
        ## nolint - tryCatchList(expr, classes, parentenv, handlers)
        ## nolint - tryCatchOne(expr, names, parentenv, handlers[[1L]])
        ## nolint - doTryCatch(return(expr), name, parentenv, handler)
        frame_position <- (function() sys.nframe())()

        application <- create_application(infer_application_name(),
                                          getwd(),
                                          substitute(code),
                                          environment,
                                          frame_position)

        .Call(C_instrumentr_trace_application_load, context, application)

        insert_instrumentation(context, application)

        .Call(C_instrumentr_trace_application_attach, context, application)

        value <- .Call(C_instrumentr_trace_code, context, code, environment)

        .Call(C_instrumentr_trace_application_detach, context, application)

        result <- create_result(value)
    },
    error = function(e) {
        print(e)

        execution_context <- .Call(C_context_get_current_execution_context, context)
        result <<- create_result(e, execution_context)
    })

    ##NOTE: all user callbacks are evaluated in tryCatch.
    ##      This code is tricky. If error has happened
    ##      previously, we do not execute user callback.
    ##      If user callback errors, we override the result
    ##      with the error object
    tryCatch({
        remove_instrumentation(context, application)

        ## NOTE: invoke callback if tracing does not error
        if (is_value(result)) {
            .Call(C_instrumentr_trace_application_unload, context, application)
        }
    },
    error = function(e) {
        print(e)

        execution_context <- .Call(C_context_get_current_execution_context, context)
        result <<- create_result(e, execution_context)
    },
    finally = {
        .Call(C_context_finalize_tracing, context)
    })

    result
}

#' @export
with_tracing_enabled <- function(code) {

    enable_tracing()

    on.exit({
        reinstate_tracing()
    })

    code
}

#' @export
with_tracing_disabled <- function(code) {

    disable_tracing()

    on.exit({
        reinstate_tracing()
    })

    code
}
