#' API for tracer objects
#'
#' @description
#' TODO
#'
#' @name tracer
NULL

#' @export
#' @rdname create_tracer
create_tracer <- function(tracing_initialization_callback,
                          tracing_finalization_callback,

                          package_load_callback,
                          package_unload_callback,
                          package_attach_callback,
                          package_detach_callback,

                          closure_call_entry_callback,
                          closure_call_exit_callback,
                          builtin_call_entry_callback,
                          builtin_call_exit_callback,
                          special_call_entry_callback,
                          special_call_exit_callback,

                          eval_entry_callback,
                          eval_exit_callback,

                          gc_allocation_callback,

                          variable_definition_callback,
                          variable_assignment_callback,
                          variable_removal_callback,
                          variable_lookup_callback,
                          packages = character(0),
                          functions = character(0)) {

    stopifnot(is_vector_character(packages))

    stopifnot(is_vector_character(functions))

    #environment <- new.env(parent = sys.frame(-1))

    tracer <- .Call(C_instrumentr_tracer_create)#, environment)

    #trace_packages(tracer, packages)
    #
    #trace_functions(tracer, functions)


    if (!missing(tracing_initialization_callback)) {
        set_tracing_initialization_callback(tracer, tracing_initialization_callback)
    }

    if (!missing(tracing_finalization_callback)) {
        set_tracing_finalization_callback(tracer, tracing_finalization_callback)
    }

    if (!missing(package_load_callback)) {
        set_package_load_callback(tracer, package_load_callback)
    }

    if (!missing(package_unload_callback)) {
        set_package_unload_callback(tracer, package_unload_callback)
    }

    if (!missing(package_attach_callback)) {
        set_package_attach_callback(tracer, package_attach_callback)
    }

    if (!missing(package_detach_callback)) {
        set_package_detach_callback(tracer, package_detach_callback)
    }

    if (!missing(builtin_call_entry_callback)) {
        set_builtin_call_entry_callback(tracer, builtin_call_entry_callback)
    }

    if (!missing(builtin_call_exit_callback)) {
        set_builtin_call_exit_callback(tracer, builtin_call_exit_callback)
    }

    if (!missing(special_call_entry_callback)) {
        set_special_call_entry_callback(tracer, special_call_entry_callback)
    }

    if (!missing(special_call_exit_callback)) {
        set_special_call_exit_callback(tracer, special_call_exit_callback)
    }

    if (!missing(closure_call_entry_callback)) {
        set_closure_call_entry_callback(tracer, closure_call_entry_callback)
    }

    if (!missing(closure_call_exit_callback)) {
        set_closure_call_exit_callback(tracer, closure_call_exit_callback)
    }

    if (!missing(eval_entry_callback)) {
        set_eval_entry_callback(tracer, eval_entry_callback)
    }

    if (!missing(eval_exit_callback)) {
        set_eval_exit_callback(tracer, eval_exit_callback)
    }

    if (!missing(gc_allocation_callback)) {
        set_gc_allocation_callback(tracer, gc_allocation_callback)
    }

    if (!missing(variable_definition_callback)) {
        set_variable_definition_callback(tracer, variable_definition_callback)
    }

    if (!missing(variable_assignment_callback)) {
        set_variable_assignment_callback(tracer, variable_assignment_callback)
    }

    if (!missing(variable_removal_callback)) {
        set_variable_removal_callback(tracer, variable_removal_callback)
    }

    if (!missing(variable_lookup_callback)) {
        set_variable_lookup_callback(tracer, variable_lookup_callback)
    }

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
