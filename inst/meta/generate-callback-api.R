library(stringr)

callback_specs <- list(
    list(name = "tracing_initialization", parameter_count = 2),
    list(name = "tracing_finalization", parameter_count = 2),

    list(name = "closure_call_entry", parameter_count = 6),
    list(name = "closure_call_exit", parameter_count = 6),
    list(name = "builtin_call_entry", parameter_count = 6),
    list(name = "builtin_call_exit", parameter_count = 6),
    list(name = "special_call_entry", parameter_count = 6),
    list(name = "special_call_exit", parameter_count = 6),

    list(name = "eval_entry", parameter_count = 2),
    list(name = "eval_exit", parameter_count = 2),

    list(name = "gc_allocation", parameter_count = 1),

    list(name = "package_attach", parameter_count = 3),
    list(name = "package_detach", parameter_count = 3),
    list(name = "package_load", parameter_count = 3),
    list(name = "package_unload", parameter_count = 3),

    list(name = "variable_assignment", parameter_count = 3),
    list(name = "variable_definition", parameter_count = 3),
    list(name = "variable_lookup", parameter_count = 3),
    list(name = "variable_removal", parameter_count = 2)
)

generate_get_api <- function(callback_spec) {
    str_glue(
        "#' @export",
        "get_{NAME}_callback <- function(tracer, ...) {{",
        "    UseMethod(\"get_{NAME}_callback\")",
        "}}",
        "",
        "#' @export",
        "get_{NAME}_callback.instrumentr_tracer <- function(tracer, ...) {{ # nolint",
        "    .Call(C_instrumentr_tracer_get_callback_{NAME}, tracer)",
        "}}",
        .sep = "\n",
        NAME = callback_spec$name
    )
}

generate_set_api <- function(callback_spec) {
    str_glue(
        "#' @export",
        "set_{NAME}_callback <- function(tracer, callback, ...) {{",
        "    UseMethod(\"set_{NAME}_callback\")",
        "}}",
        "",
        "#' @export",
        "set_{NAME}_callback.instrumentr_tracer <- function(tracer, callback, ...) {{ # nolint",
        "",
        "    if (!is_instrumentr_{NAME}_callback(callback)) {{",
        "        callback <- create_{NAME}_callback(callback)",
        "    }}",
        "",
        "    .Call(C_instrumentr_tracer_set_callback_{NAME}, tracer, callback)",
        "",
        "    invisible(NULL)",
        "}}",
        .sep = "\n",
        NAME = callback_spec$name
    )
}


generate_has_api <- function(callback_spec) {

    str_glue(
        "#' @export",
        "has_{NAME}_callback <- function(object, ...) {{ # nolint",
        "    UseMethod(\"has_{NAME}_callback\")",
        "}}",
        "",
        "#' @export",
        "has_{NAME}_callback.instrumentr_tracer <- function(object, ...) {{ # nolint",
        "    .Call(C_instrumentr_tracer_has_callback_{NAME}, object)",
        "}}",
        .sep = "\n",
        NAME = callback_spec$name
    )

}


generate_create_api <- function(callback_spec) {

    str_glue(
        "#' @export",
        "create_{NAME}_callback <- function(object) {{ # nolint",
        "    UseMethod(\"create_{NAME}_callback\")",
        "}}",
        "",
        "#' @export",
        "create_{NAME}_callback.function <- function(object) {{ # nolint",
        "    stopifnot(is_closure(object) && has_parameters(object, {PARAMETER_COUNT}))",
        "",
        "    .Call(C_instrumentr_callback_{NAME}_create_from_r_function, object)",
        "}}",
        "",
        "#' @export",
        "create_{NAME}_callback.externalptr <- function(object) {{ # nolint",
        "    .Call(C_instrumentr_callback_{NAME}_create_from_c_function, object)",
        "}}",
        .sep = "\n",
        NAME = callback_spec$name,
        PARAMETER_COUNT = callback_spec$parameter_count
    )
}

generate_is_api <- function(callback_spec) {

    str_glue(
        "is_instrumentr_{NAME}_callback <- function(object) {{ # nolint",
        "    inherits(object, 'instrumentr_{NAME}_callback')",
        "}}",
        .sep = "\n",
        NAME = callback_spec$name
    )
}

generate_api <- function(callback_spec) {

    str_glue(
        "################################################################################",
        "## {NAME}",
        "################################################################################",
        "",
        "## GET #########################################################################",
        "",
        "{GET_API}",
        "",
        "## SET #########################################################################",
        "",
        "{SET_API}",
        "",
        "## HAS #########################################################################",
        "",
        "{HAS_API}",
        "",
        "## CREATE ######################################################################",
        "",
        "{CREATE_API}",
        "",
        "## IS ##########################################################################",
        "",
        "{IS_API}",
        .sep = "\n",
        NAME = callback_spec$name,
        GET_API = generate_get_api(callback_spec),
        SET_API = generate_set_api(callback_spec),
        HAS_API = generate_has_api(callback_spec),
        CREATE_API = generate_create_api(callback_spec),
        IS_API = generate_is_api(callback_spec)
    )
}

main <- function() {

    args = commandArgs(trailingOnly=TRUE)

    if (length(args)==0) {
        stop("Script requires output file argument", call.=FALSE)
    }

    filepath <- args[1]

    apis <- paste(Map(generate_api, callback_specs), collapse = "\n\n\n")

    write(apis, filepath)

}

main()
