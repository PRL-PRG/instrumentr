################################################################################
## tracing_initialization
################################################################################

## GET #########################################################################

#' @export
get_tracing_initialization_callback <- function(tracer, ...) {
    UseMethod("get_tracing_initialization_callback")
}

#' @export
get_tracing_initialization_callback.instrumentr_tracer <- function(tracer, ...) { # nolint
    .Call(C_instrumentr_tracer_get_callback_tracing_initialization, tracer)
}

## SET #########################################################################

#' @export
set_tracing_initialization_callback <- function(tracer, callback, ...) {
    UseMethod("set_tracing_initialization_callback")
}

#' @export
set_tracing_initialization_callback.instrumentr_tracer <- function(tracer, callback, ...) { # nolint

    if (!is_instrumentr_tracing_initialization_callback(callback)) {
        callback <- create_tracing_initialization_callback(callback)
    }

    .Call(C_instrumentr_tracer_set_callback_tracing_initialization, tracer, callback)

    invisible(NULL)
}

## HAS #########################################################################

#' @export
has_tracing_initialization_callback <- function(object, ...) { # nolint
    UseMethod("has_tracing_initialization_callback")
}

#' @export
has_tracing_initialization_callback.instrumentr_tracer <- function(object, ...) { # nolint
    .Call(C_instrumentr_tracer_has_callback_tracing_initialization, object)
}

## CREATE ######################################################################

#' @export
create_tracing_initialization_callback <- function(object) { # nolint
    UseMethod("create_tracing_initialization_callback")
}

#' @export
create_tracing_initialization_callback.function <- function(object) { # nolint
    stopifnot(is_closure(object) && has_parameters(object, 2))

    .Call(C_instrumentr_callback_tracing_initialization_create_from_r_function, object)
}

#' @export
create_tracing_initialization_callback.externalptr <- function(object) { # nolint
    .Call(C_instrumentr_callback_tracing_initialization_create_from_c_function, object)
}


################################################################################
## tracing_finalization
################################################################################

## GET #########################################################################

#' @export
get_tracing_finalization_callback <- function(tracer, ...) {
    UseMethod("get_tracing_finalization_callback")
}

#' @export
get_tracing_finalization_callback.instrumentr_tracer <- function(tracer, ...) { # nolint
    .Call(C_instrumentr_tracer_get_callback_tracing_finalization, tracer)
}

## SET #########################################################################

#' @export
set_tracing_finalization_callback <- function(tracer, callback, ...) {
    UseMethod("set_tracing_finalization_callback")
}

#' @export
set_tracing_finalization_callback.instrumentr_tracer <- function(tracer, callback, ...) { # nolint

    if (!is_instrumentr_tracing_finalization_callback(callback)) {
        callback <- create_tracing_finalization_callback(callback)
    }

    .Call(C_instrumentr_tracer_set_callback_tracing_finalization, tracer, callback)

    invisible(NULL)
}

## HAS #########################################################################

#' @export
has_tracing_finalization_callback <- function(object, ...) { # nolint
    UseMethod("has_tracing_finalization_callback")
}

#' @export
has_tracing_finalization_callback.instrumentr_tracer <- function(object, ...) { # nolint
    .Call(C_instrumentr_tracer_has_callback_tracing_finalization, object)
}

## CREATE ######################################################################

#' @export
create_tracing_finalization_callback <- function(object) { # nolint
    UseMethod("create_tracing_finalization_callback")
}

#' @export
create_tracing_finalization_callback.function <- function(object) { # nolint
    stopifnot(is_closure(object) && has_parameters(object, 2))

    .Call(C_instrumentr_callback_tracing_finalization_create_from_r_function, object)
}

#' @export
create_tracing_finalization_callback.externalptr <- function(object) { # nolint
    .Call(C_instrumentr_callback_tracing_finalization_create_from_c_function, object)
}


################################################################################
## closure_call_entry
################################################################################

## GET #########################################################################

#' @export
get_closure_call_entry_callback <- function(tracer, ...) {
    UseMethod("get_closure_call_entry_callback")
}

#' @export
get_closure_call_entry_callback.instrumentr_tracer <- function(tracer, ...) { # nolint
    .Call(C_instrumentr_tracer_get_callback_closure_call_entry, tracer)
}

## SET #########################################################################

#' @export
set_closure_call_entry_callback <- function(tracer, callback, ...) {
    UseMethod("set_closure_call_entry_callback")
}

#' @export
set_closure_call_entry_callback.instrumentr_tracer <- function(tracer, callback, ...) { # nolint

    if (!is_instrumentr_closure_call_entry_callback(callback)) {
        callback <- create_closure_call_entry_callback(callback)
    }

    .Call(C_instrumentr_tracer_set_callback_closure_call_entry, tracer, callback)

    invisible(NULL)
}

## HAS #########################################################################

#' @export
has_closure_call_entry_callback <- function(object, ...) { # nolint
    UseMethod("has_closure_call_entry_callback")
}

#' @export
has_closure_call_entry_callback.instrumentr_tracer <- function(object, ...) { # nolint
    .Call(C_instrumentr_tracer_has_callback_closure_call_entry, object)
}

## CREATE ######################################################################

#' @export
create_closure_call_entry_callback <- function(object) { # nolint
    UseMethod("create_closure_call_entry_callback")
}

#' @export
create_closure_call_entry_callback.function <- function(object) { # nolint
    stopifnot(is_closure(object) && has_parameters(object, 6))

    .Call(C_instrumentr_callback_closure_call_entry_create_from_r_function, object)
}

#' @export
create_closure_call_entry_callback.externalptr <- function(object) { # nolint
    .Call(C_instrumentr_callback_closure_call_entry_create_from_c_function, object)
}


################################################################################
## closure_call_exit
################################################################################

## GET #########################################################################

#' @export
get_closure_call_exit_callback <- function(tracer, ...) {
    UseMethod("get_closure_call_exit_callback")
}

#' @export
get_closure_call_exit_callback.instrumentr_tracer <- function(tracer, ...) { # nolint
    .Call(C_instrumentr_tracer_get_callback_closure_call_exit, tracer)
}

## SET #########################################################################

#' @export
set_closure_call_exit_callback <- function(tracer, callback, ...) {
    UseMethod("set_closure_call_exit_callback")
}

#' @export
set_closure_call_exit_callback.instrumentr_tracer <- function(tracer, callback, ...) { # nolint

    if (!is_instrumentr_closure_call_exit_callback(callback)) {
        callback <- create_closure_call_exit_callback(callback)
    }

    .Call(C_instrumentr_tracer_set_callback_closure_call_exit, tracer, callback)

    invisible(NULL)
}

## HAS #########################################################################

#' @export
has_closure_call_exit_callback <- function(object, ...) { # nolint
    UseMethod("has_closure_call_exit_callback")
}

#' @export
has_closure_call_exit_callback.instrumentr_tracer <- function(object, ...) { # nolint
    .Call(C_instrumentr_tracer_has_callback_closure_call_exit, object)
}

## CREATE ######################################################################

#' @export
create_closure_call_exit_callback <- function(object) { # nolint
    UseMethod("create_closure_call_exit_callback")
}

#' @export
create_closure_call_exit_callback.function <- function(object) { # nolint
    stopifnot(is_closure(object) && has_parameters(object, 6))

    .Call(C_instrumentr_callback_closure_call_exit_create_from_r_function, object)
}

#' @export
create_closure_call_exit_callback.externalptr <- function(object) { # nolint
    .Call(C_instrumentr_callback_closure_call_exit_create_from_c_function, object)
}


################################################################################
## builtin_call_entry
################################################################################

## GET #########################################################################

#' @export
get_builtin_call_entry_callback <- function(tracer, ...) {
    UseMethod("get_builtin_call_entry_callback")
}

#' @export
get_builtin_call_entry_callback.instrumentr_tracer <- function(tracer, ...) { # nolint
    .Call(C_instrumentr_tracer_get_callback_builtin_call_entry, tracer)
}

## SET #########################################################################

#' @export
set_builtin_call_entry_callback <- function(tracer, callback, ...) {
    UseMethod("set_builtin_call_entry_callback")
}

#' @export
set_builtin_call_entry_callback.instrumentr_tracer <- function(tracer, callback, ...) { # nolint

    if (!is_instrumentr_builtin_call_entry_callback(callback)) {
        callback <- create_builtin_call_entry_callback(callback)
    }

    .Call(C_instrumentr_tracer_set_callback_builtin_call_entry, tracer, callback)

    invisible(NULL)
}

## HAS #########################################################################

#' @export
has_builtin_call_entry_callback <- function(object, ...) { # nolint
    UseMethod("has_builtin_call_entry_callback")
}

#' @export
has_builtin_call_entry_callback.instrumentr_tracer <- function(object, ...) { # nolint
    .Call(C_instrumentr_tracer_has_callback_builtin_call_entry, object)
}

## CREATE ######################################################################

#' @export
create_builtin_call_entry_callback <- function(object) { # nolint
    UseMethod("create_builtin_call_entry_callback")
}

#' @export
create_builtin_call_entry_callback.function <- function(object) { # nolint
    stopifnot(is_closure(object) && has_parameters(object, 6))

    .Call(C_instrumentr_callback_builtin_call_entry_create_from_r_function, object)
}

#' @export
create_builtin_call_entry_callback.externalptr <- function(object) { # nolint
    .Call(C_instrumentr_callback_builtin_call_entry_create_from_c_function, object)
}


################################################################################
## builtin_call_exit
################################################################################

## GET #########################################################################

#' @export
get_builtin_call_exit_callback <- function(tracer, ...) {
    UseMethod("get_builtin_call_exit_callback")
}

#' @export
get_builtin_call_exit_callback.instrumentr_tracer <- function(tracer, ...) { # nolint
    .Call(C_instrumentr_tracer_get_callback_builtin_call_exit, tracer)
}

## SET #########################################################################

#' @export
set_builtin_call_exit_callback <- function(tracer, callback, ...) {
    UseMethod("set_builtin_call_exit_callback")
}

#' @export
set_builtin_call_exit_callback.instrumentr_tracer <- function(tracer, callback, ...) { # nolint

    if (!is_instrumentr_builtin_call_exit_callback(callback)) {
        callback <- create_builtin_call_exit_callback(callback)
    }

    .Call(C_instrumentr_tracer_set_callback_builtin_call_exit, tracer, callback)

    invisible(NULL)
}

## HAS #########################################################################

#' @export
has_builtin_call_exit_callback <- function(object, ...) { # nolint
    UseMethod("has_builtin_call_exit_callback")
}

#' @export
has_builtin_call_exit_callback.instrumentr_tracer <- function(object, ...) { # nolint
    .Call(C_instrumentr_tracer_has_callback_builtin_call_exit, object)
}

## CREATE ######################################################################

#' @export
create_builtin_call_exit_callback <- function(object) { # nolint
    UseMethod("create_builtin_call_exit_callback")
}

#' @export
create_builtin_call_exit_callback.function <- function(object) { # nolint
    stopifnot(is_closure(object) && has_parameters(object, 6))

    .Call(C_instrumentr_callback_builtin_call_exit_create_from_r_function, object)
}

#' @export
create_builtin_call_exit_callback.externalptr <- function(object) { # nolint
    .Call(C_instrumentr_callback_builtin_call_exit_create_from_c_function, object)
}


################################################################################
## special_call_entry
################################################################################

## GET #########################################################################

#' @export
get_special_call_entry_callback <- function(tracer, ...) {
    UseMethod("get_special_call_entry_callback")
}

#' @export
get_special_call_entry_callback.instrumentr_tracer <- function(tracer, ...) { # nolint
    .Call(C_instrumentr_tracer_get_callback_special_call_entry, tracer)
}

## SET #########################################################################

#' @export
set_special_call_entry_callback <- function(tracer, callback, ...) {
    UseMethod("set_special_call_entry_callback")
}

#' @export
set_special_call_entry_callback.instrumentr_tracer <- function(tracer, callback, ...) { # nolint

    if (!is_instrumentr_special_call_entry_callback(callback)) {
        callback <- create_special_call_entry_callback(callback)
    }

    .Call(C_instrumentr_tracer_set_callback_special_call_entry, tracer, callback)

    invisible(NULL)
}

## HAS #########################################################################

#' @export
has_special_call_entry_callback <- function(object, ...) { # nolint
    UseMethod("has_special_call_entry_callback")
}

#' @export
has_special_call_entry_callback.instrumentr_tracer <- function(object, ...) { # nolint
    .Call(C_instrumentr_tracer_has_callback_special_call_entry, object)
}

## CREATE ######################################################################

#' @export
create_special_call_entry_callback <- function(object) { # nolint
    UseMethod("create_special_call_entry_callback")
}

#' @export
create_special_call_entry_callback.function <- function(object) { # nolint
    stopifnot(is_closure(object) && has_parameters(object, 6))

    .Call(C_instrumentr_callback_special_call_entry_create_from_r_function, object)
}

#' @export
create_special_call_entry_callback.externalptr <- function(object) { # nolint
    .Call(C_instrumentr_callback_special_call_entry_create_from_c_function, object)
}


################################################################################
## special_call_exit
################################################################################

## GET #########################################################################

#' @export
get_special_call_exit_callback <- function(tracer, ...) {
    UseMethod("get_special_call_exit_callback")
}

#' @export
get_special_call_exit_callback.instrumentr_tracer <- function(tracer, ...) { # nolint
    .Call(C_instrumentr_tracer_get_callback_special_call_exit, tracer)
}

## SET #########################################################################

#' @export
set_special_call_exit_callback <- function(tracer, callback, ...) {
    UseMethod("set_special_call_exit_callback")
}

#' @export
set_special_call_exit_callback.instrumentr_tracer <- function(tracer, callback, ...) { # nolint

    if (!is_instrumentr_special_call_exit_callback(callback)) {
        callback <- create_special_call_exit_callback(callback)
    }

    .Call(C_instrumentr_tracer_set_callback_special_call_exit, tracer, callback)

    invisible(NULL)
}

## HAS #########################################################################

#' @export
has_special_call_exit_callback <- function(object, ...) { # nolint
    UseMethod("has_special_call_exit_callback")
}

#' @export
has_special_call_exit_callback.instrumentr_tracer <- function(object, ...) { # nolint
    .Call(C_instrumentr_tracer_has_callback_special_call_exit, object)
}

## CREATE ######################################################################

#' @export
create_special_call_exit_callback <- function(object) { # nolint
    UseMethod("create_special_call_exit_callback")
}

#' @export
create_special_call_exit_callback.function <- function(object) { # nolint
    stopifnot(is_closure(object) && has_parameters(object, 6))

    .Call(C_instrumentr_callback_special_call_exit_create_from_r_function, object)
}

#' @export
create_special_call_exit_callback.externalptr <- function(object) { # nolint
    .Call(C_instrumentr_callback_special_call_exit_create_from_c_function, object)
}


################################################################################
## eval_entry
################################################################################

## GET #########################################################################

#' @export
get_eval_entry_callback <- function(tracer, ...) {
    UseMethod("get_eval_entry_callback")
}

#' @export
get_eval_entry_callback.instrumentr_tracer <- function(tracer, ...) { # nolint
    .Call(C_instrumentr_tracer_get_callback_eval_entry, tracer)
}

## SET #########################################################################

#' @export
set_eval_entry_callback <- function(tracer, callback, ...) {
    UseMethod("set_eval_entry_callback")
}

#' @export
set_eval_entry_callback.instrumentr_tracer <- function(tracer, callback, ...) { # nolint

    if (!is_instrumentr_eval_entry_callback(callback)) {
        callback <- create_eval_entry_callback(callback)
    }

    .Call(C_instrumentr_tracer_set_callback_eval_entry, tracer, callback)

    invisible(NULL)
}

## HAS #########################################################################

#' @export
has_eval_entry_callback <- function(object, ...) { # nolint
    UseMethod("has_eval_entry_callback")
}

#' @export
has_eval_entry_callback.instrumentr_tracer <- function(object, ...) { # nolint
    .Call(C_instrumentr_tracer_has_callback_eval_entry, object)
}

## CREATE ######################################################################

#' @export
create_eval_entry_callback <- function(object) { # nolint
    UseMethod("create_eval_entry_callback")
}

#' @export
create_eval_entry_callback.function <- function(object) { # nolint
    stopifnot(is_closure(object) && has_parameters(object, 2))

    .Call(C_instrumentr_callback_eval_entry_create_from_r_function, object)
}

#' @export
create_eval_entry_callback.externalptr <- function(object) { # nolint
    .Call(C_instrumentr_callback_eval_entry_create_from_c_function, object)
}


################################################################################
## eval_exit
################################################################################

## GET #########################################################################

#' @export
get_eval_exit_callback <- function(tracer, ...) {
    UseMethod("get_eval_exit_callback")
}

#' @export
get_eval_exit_callback.instrumentr_tracer <- function(tracer, ...) { # nolint
    .Call(C_instrumentr_tracer_get_callback_eval_exit, tracer)
}

## SET #########################################################################

#' @export
set_eval_exit_callback <- function(tracer, callback, ...) {
    UseMethod("set_eval_exit_callback")
}

#' @export
set_eval_exit_callback.instrumentr_tracer <- function(tracer, callback, ...) { # nolint

    if (!is_instrumentr_eval_exit_callback(callback)) {
        callback <- create_eval_exit_callback(callback)
    }

    .Call(C_instrumentr_tracer_set_callback_eval_exit, tracer, callback)

    invisible(NULL)
}

## HAS #########################################################################

#' @export
has_eval_exit_callback <- function(object, ...) { # nolint
    UseMethod("has_eval_exit_callback")
}

#' @export
has_eval_exit_callback.instrumentr_tracer <- function(object, ...) { # nolint
    .Call(C_instrumentr_tracer_has_callback_eval_exit, object)
}

## CREATE ######################################################################

#' @export
create_eval_exit_callback <- function(object) { # nolint
    UseMethod("create_eval_exit_callback")
}

#' @export
create_eval_exit_callback.function <- function(object) { # nolint
    stopifnot(is_closure(object) && has_parameters(object, 2))

    .Call(C_instrumentr_callback_eval_exit_create_from_r_function, object)
}

#' @export
create_eval_exit_callback.externalptr <- function(object) { # nolint
    .Call(C_instrumentr_callback_eval_exit_create_from_c_function, object)
}


################################################################################
## gc_allocation
################################################################################

## GET #########################################################################

#' @export
get_gc_allocation_callback <- function(tracer, ...) {
    UseMethod("get_gc_allocation_callback")
}

#' @export
get_gc_allocation_callback.instrumentr_tracer <- function(tracer, ...) { # nolint
    .Call(C_instrumentr_tracer_get_callback_gc_allocation, tracer)
}

## SET #########################################################################

#' @export
set_gc_allocation_callback <- function(tracer, callback, ...) {
    UseMethod("set_gc_allocation_callback")
}

#' @export
set_gc_allocation_callback.instrumentr_tracer <- function(tracer, callback, ...) { # nolint

    if (!is_instrumentr_gc_allocation_callback(callback)) {
        callback <- create_gc_allocation_callback(callback)
    }

    .Call(C_instrumentr_tracer_set_callback_gc_allocation, tracer, callback)

    invisible(NULL)
}

## HAS #########################################################################

#' @export
has_gc_allocation_callback <- function(object, ...) { # nolint
    UseMethod("has_gc_allocation_callback")
}

#' @export
has_gc_allocation_callback.instrumentr_tracer <- function(object, ...) { # nolint
    .Call(C_instrumentr_tracer_has_callback_gc_allocation, object)
}

## CREATE ######################################################################

#' @export
create_gc_allocation_callback <- function(object) { # nolint
    UseMethod("create_gc_allocation_callback")
}

#' @export
create_gc_allocation_callback.function <- function(object) { # nolint
    stopifnot(is_closure(object) && has_parameters(object, 1))

    .Call(C_instrumentr_callback_gc_allocation_create_from_r_function, object)
}

#' @export
create_gc_allocation_callback.externalptr <- function(object) { # nolint
    .Call(C_instrumentr_callback_gc_allocation_create_from_c_function, object)
}


################################################################################
## package_attach
################################################################################

## GET #########################################################################

#' @export
get_package_attach_callback <- function(tracer, ...) {
    UseMethod("get_package_attach_callback")
}

#' @export
get_package_attach_callback.instrumentr_tracer <- function(tracer, ...) { # nolint
    .Call(C_instrumentr_tracer_get_callback_package_attach, tracer)
}

## SET #########################################################################

#' @export
set_package_attach_callback <- function(tracer, callback, ...) {
    UseMethod("set_package_attach_callback")
}

#' @export
set_package_attach_callback.instrumentr_tracer <- function(tracer, callback, ...) { # nolint

    if (!is_instrumentr_package_attach_callback(callback)) {
        callback <- create_package_attach_callback(callback)
    }

    .Call(C_instrumentr_tracer_set_callback_package_attach, tracer, callback)

    invisible(NULL)
}

## HAS #########################################################################

#' @export
has_package_attach_callback <- function(object, ...) { # nolint
    UseMethod("has_package_attach_callback")
}

#' @export
has_package_attach_callback.instrumentr_tracer <- function(object, ...) { # nolint
    .Call(C_instrumentr_tracer_has_callback_package_attach, object)
}

## CREATE ######################################################################

#' @export
create_package_attach_callback <- function(object) { # nolint
    UseMethod("create_package_attach_callback")
}

#' @export
create_package_attach_callback.function <- function(object) { # nolint
    stopifnot(is_closure(object) && has_parameters(object, 3))

    .Call(C_instrumentr_callback_package_attach_create_from_r_function, object)
}

#' @export
create_package_attach_callback.externalptr <- function(object) { # nolint
    .Call(C_instrumentr_callback_package_attach_create_from_c_function, object)
}


################################################################################
## package_detach
################################################################################

## GET #########################################################################

#' @export
get_package_detach_callback <- function(tracer, ...) {
    UseMethod("get_package_detach_callback")
}

#' @export
get_package_detach_callback.instrumentr_tracer <- function(tracer, ...) { # nolint
    .Call(C_instrumentr_tracer_get_callback_package_detach, tracer)
}

## SET #########################################################################

#' @export
set_package_detach_callback <- function(tracer, callback, ...) {
    UseMethod("set_package_detach_callback")
}

#' @export
set_package_detach_callback.instrumentr_tracer <- function(tracer, callback, ...) { # nolint

    if (!is_instrumentr_package_detach_callback(callback)) {
        callback <- create_package_detach_callback(callback)
    }

    .Call(C_instrumentr_tracer_set_callback_package_detach, tracer, callback)

    invisible(NULL)
}

## HAS #########################################################################

#' @export
has_package_detach_callback <- function(object, ...) { # nolint
    UseMethod("has_package_detach_callback")
}

#' @export
has_package_detach_callback.instrumentr_tracer <- function(object, ...) { # nolint
    .Call(C_instrumentr_tracer_has_callback_package_detach, object)
}

## CREATE ######################################################################

#' @export
create_package_detach_callback <- function(object) { # nolint
    UseMethod("create_package_detach_callback")
}

#' @export
create_package_detach_callback.function <- function(object) { # nolint
    stopifnot(is_closure(object) && has_parameters(object, 3))

    .Call(C_instrumentr_callback_package_detach_create_from_r_function, object)
}

#' @export
create_package_detach_callback.externalptr <- function(object) { # nolint
    .Call(C_instrumentr_callback_package_detach_create_from_c_function, object)
}


################################################################################
## package_load
################################################################################

## GET #########################################################################

#' @export
get_package_load_callback <- function(tracer, ...) {
    UseMethod("get_package_load_callback")
}

#' @export
get_package_load_callback.instrumentr_tracer <- function(tracer, ...) { # nolint
    .Call(C_instrumentr_tracer_get_callback_package_load, tracer)
}

## SET #########################################################################

#' @export
set_package_load_callback <- function(tracer, callback, ...) {
    UseMethod("set_package_load_callback")
}

#' @export
set_package_load_callback.instrumentr_tracer <- function(tracer, callback, ...) { # nolint

    if (!is_instrumentr_package_load_callback(callback)) {
        callback <- create_package_load_callback(callback)
    }

    .Call(C_instrumentr_tracer_set_callback_package_load, tracer, callback)

    invisible(NULL)
}

## HAS #########################################################################

#' @export
has_package_load_callback <- function(object, ...) { # nolint
    UseMethod("has_package_load_callback")
}

#' @export
has_package_load_callback.instrumentr_tracer <- function(object, ...) { # nolint
    .Call(C_instrumentr_tracer_has_callback_package_load, object)
}

## CREATE ######################################################################

#' @export
create_package_load_callback <- function(object) { # nolint
    UseMethod("create_package_load_callback")
}

#' @export
create_package_load_callback.function <- function(object) { # nolint
    stopifnot(is_closure(object) && has_parameters(object, 3))

    .Call(C_instrumentr_callback_package_load_create_from_r_function, object)
}

#' @export
create_package_load_callback.externalptr <- function(object) { # nolint
    .Call(C_instrumentr_callback_package_load_create_from_c_function, object)
}


################################################################################
## package_unload
################################################################################

## GET #########################################################################

#' @export
get_package_unload_callback <- function(tracer, ...) {
    UseMethod("get_package_unload_callback")
}

#' @export
get_package_unload_callback.instrumentr_tracer <- function(tracer, ...) { # nolint
    .Call(C_instrumentr_tracer_get_callback_package_unload, tracer)
}

## SET #########################################################################

#' @export
set_package_unload_callback <- function(tracer, callback, ...) {
    UseMethod("set_package_unload_callback")
}

#' @export
set_package_unload_callback.instrumentr_tracer <- function(tracer, callback, ...) { # nolint

    if (!is_instrumentr_package_unload_callback(callback)) {
        callback <- create_package_unload_callback(callback)
    }

    .Call(C_instrumentr_tracer_set_callback_package_unload, tracer, callback)

    invisible(NULL)
}

## HAS #########################################################################

#' @export
has_package_unload_callback <- function(object, ...) { # nolint
    UseMethod("has_package_unload_callback")
}

#' @export
has_package_unload_callback.instrumentr_tracer <- function(object, ...) { # nolint
    .Call(C_instrumentr_tracer_has_callback_package_unload, object)
}

## CREATE ######################################################################

#' @export
create_package_unload_callback <- function(object) { # nolint
    UseMethod("create_package_unload_callback")
}

#' @export
create_package_unload_callback.function <- function(object) { # nolint
    stopifnot(is_closure(object) && has_parameters(object, 3))

    .Call(C_instrumentr_callback_package_unload_create_from_r_function, object)
}

#' @export
create_package_unload_callback.externalptr <- function(object) { # nolint
    .Call(C_instrumentr_callback_package_unload_create_from_c_function, object)
}


################################################################################
## variable_assignment
################################################################################

## GET #########################################################################

#' @export
get_variable_assignment_callback <- function(tracer, ...) {
    UseMethod("get_variable_assignment_callback")
}

#' @export
get_variable_assignment_callback.instrumentr_tracer <- function(tracer, ...) { # nolint
    .Call(C_instrumentr_tracer_get_callback_variable_assignment, tracer)
}

## SET #########################################################################

#' @export
set_variable_assignment_callback <- function(tracer, callback, ...) {
    UseMethod("set_variable_assignment_callback")
}

#' @export
set_variable_assignment_callback.instrumentr_tracer <- function(tracer, callback, ...) { # nolint

    if (!is_instrumentr_variable_assignment_callback(callback)) {
        callback <- create_variable_assignment_callback(callback)
    }

    .Call(C_instrumentr_tracer_set_callback_variable_assignment, tracer, callback)

    invisible(NULL)
}

## HAS #########################################################################

#' @export
has_variable_assignment_callback <- function(object, ...) { # nolint
    UseMethod("has_variable_assignment_callback")
}

#' @export
has_variable_assignment_callback.instrumentr_tracer <- function(object, ...) { # nolint
    .Call(C_instrumentr_tracer_has_callback_variable_assignment, object)
}

## CREATE ######################################################################

#' @export
create_variable_assignment_callback <- function(object) { # nolint
    UseMethod("create_variable_assignment_callback")
}

#' @export
create_variable_assignment_callback.function <- function(object) { # nolint
    stopifnot(is_closure(object) && has_parameters(object, 3))

    .Call(C_instrumentr_callback_variable_assignment_create_from_r_function, object)
}

#' @export
create_variable_assignment_callback.externalptr <- function(object) { # nolint
    .Call(C_instrumentr_callback_variable_assignment_create_from_c_function, object)
}


################################################################################
## variable_definition
################################################################################

## GET #########################################################################

#' @export
get_variable_definition_callback <- function(tracer, ...) {
    UseMethod("get_variable_definition_callback")
}

#' @export
get_variable_definition_callback.instrumentr_tracer <- function(tracer, ...) { # nolint
    .Call(C_instrumentr_tracer_get_callback_variable_definition, tracer)
}

## SET #########################################################################

#' @export
set_variable_definition_callback <- function(tracer, callback, ...) {
    UseMethod("set_variable_definition_callback")
}

#' @export
set_variable_definition_callback.instrumentr_tracer <- function(tracer, callback, ...) { # nolint

    if (!is_instrumentr_variable_definition_callback(callback)) {
        callback <- create_variable_definition_callback(callback)
    }

    .Call(C_instrumentr_tracer_set_callback_variable_definition, tracer, callback)

    invisible(NULL)
}

## HAS #########################################################################

#' @export
has_variable_definition_callback <- function(object, ...) { # nolint
    UseMethod("has_variable_definition_callback")
}

#' @export
has_variable_definition_callback.instrumentr_tracer <- function(object, ...) { # nolint
    .Call(C_instrumentr_tracer_has_callback_variable_definition, object)
}

## CREATE ######################################################################

#' @export
create_variable_definition_callback <- function(object) { # nolint
    UseMethod("create_variable_definition_callback")
}

#' @export
create_variable_definition_callback.function <- function(object) { # nolint
    stopifnot(is_closure(object) && has_parameters(object, 3))

    .Call(C_instrumentr_callback_variable_definition_create_from_r_function, object)
}

#' @export
create_variable_definition_callback.externalptr <- function(object) { # nolint
    .Call(C_instrumentr_callback_variable_definition_create_from_c_function, object)
}


################################################################################
## variable_lookup
################################################################################

## GET #########################################################################

#' @export
get_variable_lookup_callback <- function(tracer, ...) {
    UseMethod("get_variable_lookup_callback")
}

#' @export
get_variable_lookup_callback.instrumentr_tracer <- function(tracer, ...) { # nolint
    .Call(C_instrumentr_tracer_get_callback_variable_lookup, tracer)
}

## SET #########################################################################

#' @export
set_variable_lookup_callback <- function(tracer, callback, ...) {
    UseMethod("set_variable_lookup_callback")
}

#' @export
set_variable_lookup_callback.instrumentr_tracer <- function(tracer, callback, ...) { # nolint

    if (!is_instrumentr_variable_lookup_callback(callback)) {
        callback <- create_variable_lookup_callback(callback)
    }

    .Call(C_instrumentr_tracer_set_callback_variable_lookup, tracer, callback)

    invisible(NULL)
}

## HAS #########################################################################

#' @export
has_variable_lookup_callback <- function(object, ...) { # nolint
    UseMethod("has_variable_lookup_callback")
}

#' @export
has_variable_lookup_callback.instrumentr_tracer <- function(object, ...) { # nolint
    .Call(C_instrumentr_tracer_has_callback_variable_lookup, object)
}

## CREATE ######################################################################

#' @export
create_variable_lookup_callback <- function(object) { # nolint
    UseMethod("create_variable_lookup_callback")
}

#' @export
create_variable_lookup_callback.function <- function(object) { # nolint
    stopifnot(is_closure(object) && has_parameters(object, 3))

    .Call(C_instrumentr_callback_variable_lookup_create_from_r_function, object)
}

#' @export
create_variable_lookup_callback.externalptr <- function(object) { # nolint
    .Call(C_instrumentr_callback_variable_lookup_create_from_c_function, object)
}


################################################################################
## variable_removal
################################################################################

## GET #########################################################################

#' @export
get_variable_removal_callback <- function(tracer, ...) {
    UseMethod("get_variable_removal_callback")
}

#' @export
get_variable_removal_callback.instrumentr_tracer <- function(tracer, ...) { # nolint
    .Call(C_instrumentr_tracer_get_callback_variable_removal, tracer)
}

## SET #########################################################################

#' @export
set_variable_removal_callback <- function(tracer, callback, ...) {
    UseMethod("set_variable_removal_callback")
}

#' @export
set_variable_removal_callback.instrumentr_tracer <- function(tracer, callback, ...) { # nolint

    if (!is_instrumentr_variable_removal_callback(callback)) {
        callback <- create_variable_removal_callback(callback)
    }

    .Call(C_instrumentr_tracer_set_callback_variable_removal, tracer, callback)

    invisible(NULL)
}

## HAS #########################################################################

#' @export
has_variable_removal_callback <- function(object, ...) { # nolint
    UseMethod("has_variable_removal_callback")
}

#' @export
has_variable_removal_callback.instrumentr_tracer <- function(object, ...) { # nolint
    .Call(C_instrumentr_tracer_has_callback_variable_removal, object)
}

## CREATE ######################################################################

#' @export
create_variable_removal_callback <- function(object) { # nolint
    UseMethod("create_variable_removal_callback")
}

#' @export
create_variable_removal_callback.function <- function(object) { # nolint
    stopifnot(is_closure(object) && has_parameters(object, 2))

    .Call(C_instrumentr_callback_variable_removal_create_from_r_function, object)
}

#' @export
create_variable_removal_callback.externalptr <- function(object) { # nolint
    .Call(C_instrumentr_callback_variable_removal_create_from_c_function, object)
}
