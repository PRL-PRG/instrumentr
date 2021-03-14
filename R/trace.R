#' API for tracing code
#'
#' @description
#' TODO
#'
#' @name trace
NULL

#' @export
#' @rdname trace
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

        application <- create_application(get_state(tracer),
                                          infer_application_name(),
                                          getwd(),
                                          substitute(code),
                                          environment,
                                          frame_position)

        insert_package_hooks(tracer, application, get_state(tracer))

        .Call(C_instrumentr_trace_tracing_entry, tracer, application)

        value <- .Call(C_instrumentr_trace_code, tracer, code, environment)

        result <- create_result(value)
    },
    error = function(e) {
        print(e)

        # TODO
        callback_type <- "don't know"#.Call(C_instrumentr_tracer_get_callback_current_type, tracer)
        result <<- create_result(e, callback_type)
    })

    ##NOTE: all user callbacks are evaluated in tryCatch.
    ##      This code is tricky. If error has happened
    ##      previously, we do not execute user callback.
    ##      If user callback errors, we override the result
    ##      with the error object
    tryCatch({
        remove_package_hooks(tracer, application)
        ## NOTE: invoke callback if tracing does not error
        ##       or if error happened only in the code
        ##       being traced but not in the tracing code
        if (is_value(result) || get_source(get_error(result)) == "application") {
            state <- .Call(C_instrumentr_trace_tracing_exit, tracer, application)
            result <- set_state(result, state)
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

#' @export
#' @rdname trace
trace_file <- function(tracer, file, environment = .GlobalEnv, ...) {
    UseMethod("trace_file")
}

#' @export
trace_file.instrumentr_tracer <- function(tracer, file, environment = .GlobalEnv, ...) {
    code <- parse(file = file, keep.source = TRUE)

    trace_code(tracer, code, environment = environment, quote = FALSE)
}

#' @export
#' @rdname trace
trace_text <- function(tracer, text, environment = .GlobalEnv, ...) {
    UseMethod("trace_text")
}

#' @export
trace_text.instrumentr_tracer <- function(tracer, text, environment = .GlobalEnv, ...) {
    code <- parse(text = text, keep.source = TRUE)

    trace_code(tracer, code, environment = environment, quote = FALSE)
}

#' @export
#' @rdname trace
with_tracing_disabled <- function(tracer, code) {

    .Call(C_instrumentr_tracer_disable, tracer)

    on.exit(.Call(C_instrumentr_tracer_reinstate, tracer))

    code
}

#' @export
#' @rdname trace
with_tracing_enabled <- function(tracer, code) {

    on.exit(.Call(C_instrumentr_tracer_reinstate, tracer))

    .Call(C_instrumentr_tracer_enable, tracer)

    code
}

