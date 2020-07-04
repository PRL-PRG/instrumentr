 
#' @export
trace_code <- function(context, code, environment = .GlobalEnv, quote = TRUE, ...) {
    UseMethod("trace_code")
}

#' @export
trace_code.instrumentr_context <- function(context, code, environment = .GlobalEnv, quote = TRUE, ...) {

    if (quote) {
        code <- substitute(code)
    }

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
