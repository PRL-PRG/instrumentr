

#' @export
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
get_environment.instrumentr_tracer <- function(object, ...) { # nolint
    .Call(C_instrumentr_tracer_get_environment, object)
}

#' @export
print.instrumentr_tracer <- function(x, ...) {
    cat(to_string(x), "\n")
}

#' @export
get_exec_stats.instrumentr_tracer <- function(object, ...) {
    .Call(C_instrumentr_tracer_get_exec_stats, object)
}

#' @export
to_string.instrumentr_tracer <- function(object, ...) {

    packages <- get_traced_packages(object)
    functions <- character(0)
    for (package in packages) {
        functions <- c(functions, get_traced_functions(object, package))
    }

    get_callback_representation <- function(name, fun) {
        callback <- fun(object)
        representation <- ""
        if (is_defined(callback)) {
            representation <- sprintf("%s=%s,\n", name, to_string(callback))
        }
        representation
    }

    get_field_representation <- function(name, fun, check = is_defined, sep = ", ") {
        get_value_representation(name, fun(object), check, sep)
    }

    get_value_representation <- function(name, value, check, sep) {
        sprintf("%s=%s%s", name, to_string(value), sep)
    }

    representation <-
      sprintf("Context(%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s",
              get_field_representation("application_load_callback", get_application_load_callback),
              get_field_representation("application_unload_callback", get_application_unload_callback),
              get_field_representation("application_attach_callback", get_application_attach_callback),
              get_field_representation("application_detach_callback", get_application_detach_callback),
              get_field_representation("package_load_callback", get_package_load_callback),
              get_field_representation("package_unload_callback", get_package_unload_callback),
              get_field_representation("package_attach_callback", get_package_attach_callback),
              get_field_representation("package_detach_callback", get_package_detach_callback),
              get_field_representation("function_attach_callback", get_function_attach_callback),
              get_field_representation("function_detach_callback", get_function_detach_callback),
              get_field_representation("call_entry_callback", get_call_entry_callback),
              get_field_representation("call_exit_callback", get_call_exit_callback),
              get_field_representation("gc_allocation_callback", get_gc_allocation_callback),
              get_field_representation("variable_definition_callback", get_variable_definition_callback),
              get_field_representation("variable_assignment_callback", get_variable_assignment_callback),
              get_field_representation("variable_removal_callback", get_variable_removal_callback),
              get_field_representation("variable_lookup_callback", get_variable_lookup_callback),
              get_value_representation("packages", packages, function(v) length(v) > 0, ", "),
              get_value_representation("functions", functions, function(v) length(v) > 0, ")"))

    representation
}
