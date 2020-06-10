
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

    .Call(C_instrumentr_initialize_tracing)

    tryCatch({
        ## NOTE: we manually account for the following four stack frames
        ## introduced by tryCatch in excess of the existing frames:
        ## - tryCatch({ <all code> })
        ## - tryCatchList(expr, classes, parentenv, handlers)
        ## - tryCatchOne(expr, names, parentenv, handlers[[1L]])
        ## - doTryCatch(return(expr), name, parentenv, handler)
        n <- sys.nframe() + 4

        set_sys_call_base_index(n)

        application <- create_application(infer_application_name(),
                                          getwd(),
                                          substitute(code),
                                          environment)

        .Call(C_instrumentr_trace_application_load, context, application)

        insert_interception(context, application)

        .Call(C_instrumentr_trace_application_attach, context, application)

        value <- .Call(C_instrumentr_trace_code, code, environment)

        .Call(C_instrumentr_trace_application_detach, context, application)

        remove_interception(context, application)

        .Call(C_instrumentr_trace_application_unload, context, application)

        result <- create_result(value)
    },
    error = function(e) {
        print(e)

        result <<- create_result(e, peek_execution_context())
    },
    finally = {
        .Call(C_instrumentr_finalize_tracing)

        set_sys_call_base_index(0)
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

peek_execution_context <- function() {
    .Call(C_instrumentr_peek_execution_context)
}
