library(stringr)

callback_specs <- list(
    list(name = "application_attach", parameter_count = 2),
    list(name = "application_detach", parameter_count = 2),
    list(name = "application_load", parameter_count = 2),
    list(name = "application_unload", parameter_count = 2),

    list(name = "call_entry", parameter_count = 5),
    list(name = "call_exit", parameter_count = 5),

    list(name = "object_coerce", parameter_count = 4),
    list(name = "object_duplicate", parameter_count = 5),
    list(name = "vector_copy", parameter_count = 4),
    list(name = "matrix_copy", parameter_count = 4),

    list(name = "closure_call_entry", parameter_count = 6),
    list(name = "closure_call_exit", parameter_count = 7),
    list(name = "builtin_call_entry", parameter_count = 6),
    list(name = "builtin_call_exit", parameter_count = 7),
    list(name = "special_call_entry", parameter_count = 6),
    list(name = "special_call_exit", parameter_count = 7),

    list(name = "eval_entry", parameter_count = 4),
    list(name = "eval_exit", parameter_count = 5),

    list(name = "gc_allocation", parameter_count = 3),
    list(name = "gc_unmark", parameter_count = 3),

    list(name = "function_attach", parameter_count = 4),
    list(name = "function_detach", parameter_count = 4),

    list(name = "package_attach", parameter_count = 3),
    list(name = "package_detach", parameter_count = 3),
    list(name = "package_load", parameter_count = 3),
    list(name = "package_unload", parameter_count = 3),

    list(name = "variable_assignment", parameter_count = 5),
    list(name = "variable_definition", parameter_count = 5),
    list(name = "variable_lookup", parameter_count = 5),
    list(name = "variable_removal", parameter_count = 4),

    list(name = "context_entry", parameter_count = 3),
    list(name = "context_exit", parameter_count = 3),
    list(name = "context_jump", parameter_count = 3)
)

generate_get_api <- function(callback_spec) {
    str_glue(
        "#' @export",
        "get_{NAME}_callback <- function(context, ...) {{",
        "    UseMethod(\"get_{NAME}_callback\")",
        "}}",
        "",
        "#' @export",
        "get_{NAME}_callback.instrumentr_context <- function(context, ...) {{ # nolint",
        "    .Call(C_context_get_{NAME}_callback, context)",
        "}}",
        .sep = "\n",
        NAME = callback_spec$name
    )
}

generate_set_api <- function(callback_spec) {
    str_glue(
        "#' @export",
        "set_{NAME}_callback <- function(context, callback, ...) {{",
        "    UseMethod(\"set_{NAME}_callback\")",
        "}}",
        "",
        "#' @export",
        "set_{NAME}_callback.instrumentr_context <- function(context, callback, ...) {{ # nolint",
        "",
        "    if (!is_instrumentr_{NAME}_callback(callback)) {{",
        "        callback <- create_{NAME}_callback(callback)",
        "    }}",
        "",
        "    .Call(C_context_set_{NAME}_callback, context, callback)",
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
        "has_{NAME}_callback.instrumentr_context <- function(object, ...) {{ # nolint",
        "    .Call(C_context_has_{NAME}_callback, object)",
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
        "    .Call(C_{NAME}_callback_create_from_r_function, object)",
        "}}",
        "",
        "#' @export",
        "create_{NAME}_callback.externalptr <- function(object) {{ # nolint",
        "    .Call(C_{NAME}_callback_create_from_c_function, object)",
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
