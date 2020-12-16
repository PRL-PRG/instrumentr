 
#' @export
trace_code <- function(context, code, environment = .GlobalEnv, quote = TRUE, ...) {
    UseMethod("trace_code")
}

#' @export
trace_code.instrumentr_context <- function(context, code, environment = .GlobalEnv, quote = TRUE, ...) {

    if (quote) {
        code <- substitute(code)
    }

    code <- substitute(tryCatch(list(value = CODE), function(e) list(error = e)), CODE = code)

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

        .Call(C_context_trace_application_load, context, application)

        insert_instrumentation(context, application)

        .Call(C_context_trace_application_attach, context, application)

        value <- .Call(C_context_trace_code, context, code, environment)

        if(is.null(value$error)) {
            .Call(C_context_trace_application_detach, context, application)

            result <- create_result(value$value)
        } else {
            callback_type <- .Call(C_context_get_current_callback_type, context)

            result <- create_result(value$error, callback_type)
        }
    },
    error = function(e) {
        print(e)

        callback_type <- .Call(C_context_get_current_callback_type, context)
        result <<- create_result(e, callback_type)
    })

    ##NOTE: all user callbacks are evaluated in tryCatch.
    ##      This code is tricky. If error has happened
    ##      previously, we do not execute user callback.
    ##      If user callback errors, we override the result
    ##      with the error object
    tryCatch({

        remove_instrumentation(context, application)

        ## NOTE: invoke callback if tracing does not error
        ##       or if error happened only in the code
        ##       being traced but not in the tracing code
        if (is_value(result) || get_source(get_error(result)) == "application") {
            .Call(C_context_trace_application_unload, context, application)
        }
    },
    error = function(e) {
        print(e)

        callback_type <- .Call(C_context_get_current_callback_type, context)
        result <<- create_result(e, callback_type)
    },
    finally = {
        .Call(C_context_finalize_tracing, context)
    })

    result
}
