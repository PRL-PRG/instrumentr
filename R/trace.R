
#' @export
is_tracing_enabled <- function() {
    .Call(C_lightr_is_tracing_enabled)
}

#' @export
disable_tracing <- function() {
    .Call(C_lightr_disable_tracing)
    invisible(NULL)
}

#' @export
enable_tracing <- function() {
    .Call(C_lightr_enable_tracing)
    invisible(NULL)
}

#' @export
reinstate_tracing <- function() {
    .Call(C_lightr_reinstate_tracing)
    invisible(NULL)
}

#' @export
trace_code <- function(code, context) {

    stopifnot(is_lightr_context(context))

    application <- create_application(infer_application_name(),
                                      getwd(),
                                      .GlobalEnv)

    .Call(C_lightr_trace_application_entry, context, application)

    insert_interception(context, application)

    on.exit({

        reinstate_tracing()

        remove_interception()

        .Call(C_lightr_trace_application_exit, context, application)

    })

    enable_tracing()

    code
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
