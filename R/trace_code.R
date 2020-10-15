
#' @export
trace_code <- function(tracer, code, environment = .GlobalEnv, quote = TRUE, ...) {
    UseMethod("trace_code")
}

#' @export
trace_code.instrumentr_tracer <- function(tracer, code, environment = .GlobalEnv, quote = TRUE, ...) {

    if (quote) {
        code <- substitute(code)
    }

    result <- NULL

    .Call(C_instrumentr_initialize_tracing, tracer)

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

        .Call(C_instrumentr_trace_application_load, tracer, application)

        insert_instrumentation(tracer, application)

        .Call(C_instrumentr_trace_application_attach, tracer, application)

        value <- .Call(C_instrumentr_trace_code, tracer, code, environment)

        .Call(C_instrumentr_trace_application_detach, tracer, application)

        result <- create_result(value)
    },
    error = function(e) {
        print(e)

        callback_type <- "don't know"#.Call(C_instrumentr_tracer_get_callback_current_type, tracer)
        result <<- create_result(e, callback_type)
    })

    ##NOTE: all user callbacks are evaluated in tryCatch.
    ##      This code is tricky. If error has happened
    ##      previously, we do not execute user callback.
    ##      If user callback errors, we override the result
    ##      with the error object
    tryCatch({

        remove_instrumentation(tracer, application)

        ## NOTE: invoke callback if tracing does not error
        ##       or if error happened only in the code
        ##       being traced but not in the tracing code
        if (is_value(result) || get_source(get_error(result)) == "application") {
            .Call(C_instrumentr_trace_application_unload, tracer, application)
        }
    },
    error = function(e) {
        print(e)

        callback_type <- "don't know"#.Call(C_instrumentr_tracer_get_callback_current_type, tracer)
        result <<- create_result(e, callback_type)
    },
    finally = {
        .Call(C_instrumentr_finalize_tracing, tracer)
    })

    result
}
