################################################################################
## application_attach
################################################################################

## GET #########################################################################

#' @export
get_application_attach_callback <- function(context, ...) {
    UseMethod("get_application_attach_callback")
}

#' @export
get_application_attach_callback.instrumentr_context <- function(context, ...) { # nolint
    .Call(C_context_get_application_attach_callback, context)
}

## SET #########################################################################

#' @export
set_application_attach_callback <- function(context, callback, ...) {
    UseMethod("set_application_attach_callback")
}

#' @export
set_application_attach_callback.instrumentr_context <- function(context, callback, ...) { # nolint

    if (!is_instrumentr_application_attach_callback(callback)) {
        callback <- create_application_attach_callback(callback)
    }

    .Call(C_context_set_application_attach_callback, context, callback)

    invisible(NULL)
}

## HAS #########################################################################

#' @export
has_application_attach_callback <- function(object, ...) { # nolint
    UseMethod("has_application_attach_callback")
}

#' @export
has_application_attach_callback.instrumentr_context <- function(object, ...) { # nolint
    .Call(C_context_has_application_attach_callback, object)
}

## CREATE ######################################################################

#' @export
create_application_attach_callback <- function(object) { # nolint
    UseMethod("create_application_attach_callback")
}

#' @export
create_application_attach_callback.function <- function(object) { # nolint
    stopifnot(is_closure(object) && has_parameters(object, 2))

    .Call(C_application_attach_callback_create_from_r_function, object)
}

#' @export
create_application_attach_callback.externalptr <- function(object) { # nolint
    .Call(C_application_attach_callback_create_from_c_function, object)
}

## IS ##########################################################################

is_instrumentr_application_attach_callback <- function(object) { # nolint
    inherits(object, 'instrumentr_application_attach_callback')
}


################################################################################
## application_detach
################################################################################

## GET #########################################################################

#' @export
get_application_detach_callback <- function(context, ...) {
    UseMethod("get_application_detach_callback")
}

#' @export
get_application_detach_callback.instrumentr_context <- function(context, ...) { # nolint
    .Call(C_context_get_application_detach_callback, context)
}

## SET #########################################################################

#' @export
set_application_detach_callback <- function(context, callback, ...) {
    UseMethod("set_application_detach_callback")
}

#' @export
set_application_detach_callback.instrumentr_context <- function(context, callback, ...) { # nolint

    if (!is_instrumentr_application_detach_callback(callback)) {
        callback <- create_application_detach_callback(callback)
    }

    .Call(C_context_set_application_detach_callback, context, callback)

    invisible(NULL)
}

## HAS #########################################################################

#' @export
has_application_detach_callback <- function(object, ...) { # nolint
    UseMethod("has_application_detach_callback")
}

#' @export
has_application_detach_callback.instrumentr_context <- function(object, ...) { # nolint
    .Call(C_context_has_application_detach_callback, object)
}

## CREATE ######################################################################

#' @export
create_application_detach_callback <- function(object) { # nolint
    UseMethod("create_application_detach_callback")
}

#' @export
create_application_detach_callback.function <- function(object) { # nolint
    stopifnot(is_closure(object) && has_parameters(object, 2))

    .Call(C_application_detach_callback_create_from_r_function, object)
}

#' @export
create_application_detach_callback.externalptr <- function(object) { # nolint
    .Call(C_application_detach_callback_create_from_c_function, object)
}

## IS ##########################################################################

is_instrumentr_application_detach_callback <- function(object) { # nolint
    inherits(object, 'instrumentr_application_detach_callback')
}


################################################################################
## application_load
################################################################################

## GET #########################################################################

#' @export
get_application_load_callback <- function(context, ...) {
    UseMethod("get_application_load_callback")
}

#' @export
get_application_load_callback.instrumentr_context <- function(context, ...) { # nolint
    .Call(C_context_get_application_load_callback, context)
}

## SET #########################################################################

#' @export
set_application_load_callback <- function(context, callback, ...) {
    UseMethod("set_application_load_callback")
}

#' @export
set_application_load_callback.instrumentr_context <- function(context, callback, ...) { # nolint

    if (!is_instrumentr_application_load_callback(callback)) {
        callback <- create_application_load_callback(callback)
    }

    .Call(C_context_set_application_load_callback, context, callback)

    invisible(NULL)
}

## HAS #########################################################################

#' @export
has_application_load_callback <- function(object, ...) { # nolint
    UseMethod("has_application_load_callback")
}

#' @export
has_application_load_callback.instrumentr_context <- function(object, ...) { # nolint
    .Call(C_context_has_application_load_callback, object)
}

## CREATE ######################################################################

#' @export
create_application_load_callback <- function(object) { # nolint
    UseMethod("create_application_load_callback")
}

#' @export
create_application_load_callback.function <- function(object) { # nolint
    stopifnot(is_closure(object) && has_parameters(object, 2))

    .Call(C_application_load_callback_create_from_r_function, object)
}

#' @export
create_application_load_callback.externalptr <- function(object) { # nolint
    .Call(C_application_load_callback_create_from_c_function, object)
}

## IS ##########################################################################

is_instrumentr_application_load_callback <- function(object) { # nolint
    inherits(object, 'instrumentr_application_load_callback')
}


################################################################################
## application_unload
################################################################################

## GET #########################################################################

#' @export
get_application_unload_callback <- function(context, ...) {
    UseMethod("get_application_unload_callback")
}

#' @export
get_application_unload_callback.instrumentr_context <- function(context, ...) { # nolint
    .Call(C_context_get_application_unload_callback, context)
}

## SET #########################################################################

#' @export
set_application_unload_callback <- function(context, callback, ...) {
    UseMethod("set_application_unload_callback")
}

#' @export
set_application_unload_callback.instrumentr_context <- function(context, callback, ...) { # nolint

    if (!is_instrumentr_application_unload_callback(callback)) {
        callback <- create_application_unload_callback(callback)
    }

    .Call(C_context_set_application_unload_callback, context, callback)

    invisible(NULL)
}

## HAS #########################################################################

#' @export
has_application_unload_callback <- function(object, ...) { # nolint
    UseMethod("has_application_unload_callback")
}

#' @export
has_application_unload_callback.instrumentr_context <- function(object, ...) { # nolint
    .Call(C_context_has_application_unload_callback, object)
}

## CREATE ######################################################################

#' @export
create_application_unload_callback <- function(object) { # nolint
    UseMethod("create_application_unload_callback")
}

#' @export
create_application_unload_callback.function <- function(object) { # nolint
    stopifnot(is_closure(object) && has_parameters(object, 2))

    .Call(C_application_unload_callback_create_from_r_function, object)
}

#' @export
create_application_unload_callback.externalptr <- function(object) { # nolint
    .Call(C_application_unload_callback_create_from_c_function, object)
}

## IS ##########################################################################

is_instrumentr_application_unload_callback <- function(object) { # nolint
    inherits(object, 'instrumentr_application_unload_callback')
}


################################################################################
## call_entry
################################################################################

## GET #########################################################################

#' @export
get_call_entry_callback <- function(context, ...) {
    UseMethod("get_call_entry_callback")
}

#' @export
get_call_entry_callback.instrumentr_context <- function(context, ...) { # nolint
    .Call(C_context_get_call_entry_callback, context)
}

## SET #########################################################################

#' @export
set_call_entry_callback <- function(context, callback, ...) {
    UseMethod("set_call_entry_callback")
}

#' @export
set_call_entry_callback.instrumentr_context <- function(context, callback, ...) { # nolint

    if (!is_instrumentr_call_entry_callback(callback)) {
        callback <- create_call_entry_callback(callback)
    }

    .Call(C_context_set_call_entry_callback, context, callback)

    invisible(NULL)
}

## HAS #########################################################################

#' @export
has_call_entry_callback <- function(object, ...) { # nolint
    UseMethod("has_call_entry_callback")
}

#' @export
has_call_entry_callback.instrumentr_context <- function(object, ...) { # nolint
    .Call(C_context_has_call_entry_callback, object)
}

## CREATE ######################################################################

#' @export
create_call_entry_callback <- function(object) { # nolint
    UseMethod("create_call_entry_callback")
}

#' @export
create_call_entry_callback.function <- function(object) { # nolint
    stopifnot(is_closure(object) && has_parameters(object, 5))

    .Call(C_call_entry_callback_create_from_r_function, object)
}

#' @export
create_call_entry_callback.externalptr <- function(object) { # nolint
    .Call(C_call_entry_callback_create_from_c_function, object)
}

## IS ##########################################################################

is_instrumentr_call_entry_callback <- function(object) { # nolint
    inherits(object, 'instrumentr_call_entry_callback')
}


################################################################################
## call_exit
################################################################################

## GET #########################################################################

#' @export
get_call_exit_callback <- function(context, ...) {
    UseMethod("get_call_exit_callback")
}

#' @export
get_call_exit_callback.instrumentr_context <- function(context, ...) { # nolint
    .Call(C_context_get_call_exit_callback, context)
}

## SET #########################################################################

#' @export
set_call_exit_callback <- function(context, callback, ...) {
    UseMethod("set_call_exit_callback")
}

#' @export
set_call_exit_callback.instrumentr_context <- function(context, callback, ...) { # nolint

    if (!is_instrumentr_call_exit_callback(callback)) {
        callback <- create_call_exit_callback(callback)
    }

    .Call(C_context_set_call_exit_callback, context, callback)

    invisible(NULL)
}

## HAS #########################################################################

#' @export
has_call_exit_callback <- function(object, ...) { # nolint
    UseMethod("has_call_exit_callback")
}

#' @export
has_call_exit_callback.instrumentr_context <- function(object, ...) { # nolint
    .Call(C_context_has_call_exit_callback, object)
}

## CREATE ######################################################################

#' @export
create_call_exit_callback <- function(object) { # nolint
    UseMethod("create_call_exit_callback")
}

#' @export
create_call_exit_callback.function <- function(object) { # nolint
    stopifnot(is_closure(object) && has_parameters(object, 5))

    .Call(C_call_exit_callback_create_from_r_function, object)
}

#' @export
create_call_exit_callback.externalptr <- function(object) { # nolint
    .Call(C_call_exit_callback_create_from_c_function, object)
}

## IS ##########################################################################

is_instrumentr_call_exit_callback <- function(object) { # nolint
    inherits(object, 'instrumentr_call_exit_callback')
}


################################################################################
## object_coerce
################################################################################

## GET #########################################################################

#' @export
get_object_coerce_callback <- function(context, ...) {
    UseMethod("get_object_coerce_callback")
}

#' @export
get_object_coerce_callback.instrumentr_context <- function(context, ...) { # nolint
    .Call(C_context_get_object_coerce_callback, context)
}

## SET #########################################################################

#' @export
set_object_coerce_callback <- function(context, callback, ...) {
    UseMethod("set_object_coerce_callback")
}

#' @export
set_object_coerce_callback.instrumentr_context <- function(context, callback, ...) { # nolint

    if (!is_instrumentr_object_coerce_callback(callback)) {
        callback <- create_object_coerce_callback(callback)
    }

    .Call(C_context_set_object_coerce_callback, context, callback)

    invisible(NULL)
}

## HAS #########################################################################

#' @export
has_object_coerce_callback <- function(object, ...) { # nolint
    UseMethod("has_object_coerce_callback")
}

#' @export
has_object_coerce_callback.instrumentr_context <- function(object, ...) { # nolint
    .Call(C_context_has_object_coerce_callback, object)
}

## CREATE ######################################################################

#' @export
create_object_coerce_callback <- function(object) { # nolint
    UseMethod("create_object_coerce_callback")
}

#' @export
create_object_coerce_callback.function <- function(object) { # nolint
    stopifnot(is_closure(object) && has_parameters(object, 4))

    .Call(C_object_coerce_callback_create_from_r_function, object)
}

#' @export
create_object_coerce_callback.externalptr <- function(object) { # nolint
    .Call(C_object_coerce_callback_create_from_c_function, object)
}

## IS ##########################################################################

is_instrumentr_object_coerce_callback <- function(object) { # nolint
    inherits(object, 'instrumentr_object_coerce_callback')
}


################################################################################
## object_duplicate
################################################################################

## GET #########################################################################

#' @export
get_object_duplicate_callback <- function(context, ...) {
    UseMethod("get_object_duplicate_callback")
}

#' @export
get_object_duplicate_callback.instrumentr_context <- function(context, ...) { # nolint
    .Call(C_context_get_object_duplicate_callback, context)
}

## SET #########################################################################

#' @export
set_object_duplicate_callback <- function(context, callback, ...) {
    UseMethod("set_object_duplicate_callback")
}

#' @export
set_object_duplicate_callback.instrumentr_context <- function(context, callback, ...) { # nolint

    if (!is_instrumentr_object_duplicate_callback(callback)) {
        callback <- create_object_duplicate_callback(callback)
    }

    .Call(C_context_set_object_duplicate_callback, context, callback)

    invisible(NULL)
}

## HAS #########################################################################

#' @export
has_object_duplicate_callback <- function(object, ...) { # nolint
    UseMethod("has_object_duplicate_callback")
}

#' @export
has_object_duplicate_callback.instrumentr_context <- function(object, ...) { # nolint
    .Call(C_context_has_object_duplicate_callback, object)
}

## CREATE ######################################################################

#' @export
create_object_duplicate_callback <- function(object) { # nolint
    UseMethod("create_object_duplicate_callback")
}

#' @export
create_object_duplicate_callback.function <- function(object) { # nolint
    stopifnot(is_closure(object) && has_parameters(object, 5))

    .Call(C_object_duplicate_callback_create_from_r_function, object)
}

#' @export
create_object_duplicate_callback.externalptr <- function(object) { # nolint
    .Call(C_object_duplicate_callback_create_from_c_function, object)
}

## IS ##########################################################################

is_instrumentr_object_duplicate_callback <- function(object) { # nolint
    inherits(object, 'instrumentr_object_duplicate_callback')
}


################################################################################
## vector_copy
################################################################################

## GET #########################################################################

#' @export
get_vector_copy_callback <- function(context, ...) {
    UseMethod("get_vector_copy_callback")
}

#' @export
get_vector_copy_callback.instrumentr_context <- function(context, ...) { # nolint
    .Call(C_context_get_vector_copy_callback, context)
}

## SET #########################################################################

#' @export
set_vector_copy_callback <- function(context, callback, ...) {
    UseMethod("set_vector_copy_callback")
}

#' @export
set_vector_copy_callback.instrumentr_context <- function(context, callback, ...) { # nolint

    if (!is_instrumentr_vector_copy_callback(callback)) {
        callback <- create_vector_copy_callback(callback)
    }

    .Call(C_context_set_vector_copy_callback, context, callback)

    invisible(NULL)
}

## HAS #########################################################################

#' @export
has_vector_copy_callback <- function(object, ...) { # nolint
    UseMethod("has_vector_copy_callback")
}

#' @export
has_vector_copy_callback.instrumentr_context <- function(object, ...) { # nolint
    .Call(C_context_has_vector_copy_callback, object)
}

## CREATE ######################################################################

#' @export
create_vector_copy_callback <- function(object) { # nolint
    UseMethod("create_vector_copy_callback")
}

#' @export
create_vector_copy_callback.function <- function(object) { # nolint
    stopifnot(is_closure(object) && has_parameters(object, 4))

    .Call(C_vector_copy_callback_create_from_r_function, object)
}

#' @export
create_vector_copy_callback.externalptr <- function(object) { # nolint
    .Call(C_vector_copy_callback_create_from_c_function, object)
}

## IS ##########################################################################

is_instrumentr_vector_copy_callback <- function(object) { # nolint
    inherits(object, 'instrumentr_vector_copy_callback')
}


################################################################################
## matrix_copy
################################################################################

## GET #########################################################################

#' @export
get_matrix_copy_callback <- function(context, ...) {
    UseMethod("get_matrix_copy_callback")
}

#' @export
get_matrix_copy_callback.instrumentr_context <- function(context, ...) { # nolint
    .Call(C_context_get_matrix_copy_callback, context)
}

## SET #########################################################################

#' @export
set_matrix_copy_callback <- function(context, callback, ...) {
    UseMethod("set_matrix_copy_callback")
}

#' @export
set_matrix_copy_callback.instrumentr_context <- function(context, callback, ...) { # nolint

    if (!is_instrumentr_matrix_copy_callback(callback)) {
        callback <- create_matrix_copy_callback(callback)
    }

    .Call(C_context_set_matrix_copy_callback, context, callback)

    invisible(NULL)
}

## HAS #########################################################################

#' @export
has_matrix_copy_callback <- function(object, ...) { # nolint
    UseMethod("has_matrix_copy_callback")
}

#' @export
has_matrix_copy_callback.instrumentr_context <- function(object, ...) { # nolint
    .Call(C_context_has_matrix_copy_callback, object)
}

## CREATE ######################################################################

#' @export
create_matrix_copy_callback <- function(object) { # nolint
    UseMethod("create_matrix_copy_callback")
}

#' @export
create_matrix_copy_callback.function <- function(object) { # nolint
    stopifnot(is_closure(object) && has_parameters(object, 4))

    .Call(C_matrix_copy_callback_create_from_r_function, object)
}

#' @export
create_matrix_copy_callback.externalptr <- function(object) { # nolint
    .Call(C_matrix_copy_callback_create_from_c_function, object)
}

## IS ##########################################################################

is_instrumentr_matrix_copy_callback <- function(object) { # nolint
    inherits(object, 'instrumentr_matrix_copy_callback')
}


################################################################################
## closure_call_entry
################################################################################

## GET #########################################################################

#' @export
get_closure_call_entry_callback <- function(context, ...) {
    UseMethod("get_closure_call_entry_callback")
}

#' @export
get_closure_call_entry_callback.instrumentr_context <- function(context, ...) { # nolint
    .Call(C_context_get_closure_call_entry_callback, context)
}

## SET #########################################################################

#' @export
set_closure_call_entry_callback <- function(context, callback, ...) {
    UseMethod("set_closure_call_entry_callback")
}

#' @export
set_closure_call_entry_callback.instrumentr_context <- function(context, callback, ...) { # nolint

    if (!is_instrumentr_closure_call_entry_callback(callback)) {
        callback <- create_closure_call_entry_callback(callback)
    }

    .Call(C_context_set_closure_call_entry_callback, context, callback)

    invisible(NULL)
}

## HAS #########################################################################

#' @export
has_closure_call_entry_callback <- function(object, ...) { # nolint
    UseMethod("has_closure_call_entry_callback")
}

#' @export
has_closure_call_entry_callback.instrumentr_context <- function(object, ...) { # nolint
    .Call(C_context_has_closure_call_entry_callback, object)
}

## CREATE ######################################################################

#' @export
create_closure_call_entry_callback <- function(object) { # nolint
    UseMethod("create_closure_call_entry_callback")
}

#' @export
create_closure_call_entry_callback.function <- function(object) { # nolint
    stopifnot(is_closure(object) && has_parameters(object, 6))

    .Call(C_closure_call_entry_callback_create_from_r_function, object)
}

#' @export
create_closure_call_entry_callback.externalptr <- function(object) { # nolint
    .Call(C_closure_call_entry_callback_create_from_c_function, object)
}

## IS ##########################################################################

is_instrumentr_closure_call_entry_callback <- function(object) { # nolint
    inherits(object, 'instrumentr_closure_call_entry_callback')
}


################################################################################
## closure_call_exit
################################################################################

## GET #########################################################################

#' @export
get_closure_call_exit_callback <- function(context, ...) {
    UseMethod("get_closure_call_exit_callback")
}

#' @export
get_closure_call_exit_callback.instrumentr_context <- function(context, ...) { # nolint
    .Call(C_context_get_closure_call_exit_callback, context)
}

## SET #########################################################################

#' @export
set_closure_call_exit_callback <- function(context, callback, ...) {
    UseMethod("set_closure_call_exit_callback")
}

#' @export
set_closure_call_exit_callback.instrumentr_context <- function(context, callback, ...) { # nolint

    if (!is_instrumentr_closure_call_exit_callback(callback)) {
        callback <- create_closure_call_exit_callback(callback)
    }

    .Call(C_context_set_closure_call_exit_callback, context, callback)

    invisible(NULL)
}

## HAS #########################################################################

#' @export
has_closure_call_exit_callback <- function(object, ...) { # nolint
    UseMethod("has_closure_call_exit_callback")
}

#' @export
has_closure_call_exit_callback.instrumentr_context <- function(object, ...) { # nolint
    .Call(C_context_has_closure_call_exit_callback, object)
}

## CREATE ######################################################################

#' @export
create_closure_call_exit_callback <- function(object) { # nolint
    UseMethod("create_closure_call_exit_callback")
}

#' @export
create_closure_call_exit_callback.function <- function(object) { # nolint
    stopifnot(is_closure(object) && has_parameters(object, 7))

    .Call(C_closure_call_exit_callback_create_from_r_function, object)
}

#' @export
create_closure_call_exit_callback.externalptr <- function(object) { # nolint
    .Call(C_closure_call_exit_callback_create_from_c_function, object)
}

## IS ##########################################################################

is_instrumentr_closure_call_exit_callback <- function(object) { # nolint
    inherits(object, 'instrumentr_closure_call_exit_callback')
}


################################################################################
## builtin_call_entry
################################################################################

## GET #########################################################################

#' @export
get_builtin_call_entry_callback <- function(context, ...) {
    UseMethod("get_builtin_call_entry_callback")
}

#' @export
get_builtin_call_entry_callback.instrumentr_context <- function(context, ...) { # nolint
    .Call(C_context_get_builtin_call_entry_callback, context)
}

## SET #########################################################################

#' @export
set_builtin_call_entry_callback <- function(context, callback, ...) {
    UseMethod("set_builtin_call_entry_callback")
}

#' @export
set_builtin_call_entry_callback.instrumentr_context <- function(context, callback, ...) { # nolint

    if (!is_instrumentr_builtin_call_entry_callback(callback)) {
        callback <- create_builtin_call_entry_callback(callback)
    }

    .Call(C_context_set_builtin_call_entry_callback, context, callback)

    invisible(NULL)
}

## HAS #########################################################################

#' @export
has_builtin_call_entry_callback <- function(object, ...) { # nolint
    UseMethod("has_builtin_call_entry_callback")
}

#' @export
has_builtin_call_entry_callback.instrumentr_context <- function(object, ...) { # nolint
    .Call(C_context_has_builtin_call_entry_callback, object)
}

## CREATE ######################################################################

#' @export
create_builtin_call_entry_callback <- function(object) { # nolint
    UseMethod("create_builtin_call_entry_callback")
}

#' @export
create_builtin_call_entry_callback.function <- function(object) { # nolint
    stopifnot(is_closure(object) && has_parameters(object, 6))

    .Call(C_builtin_call_entry_callback_create_from_r_function, object)
}

#' @export
create_builtin_call_entry_callback.externalptr <- function(object) { # nolint
    .Call(C_builtin_call_entry_callback_create_from_c_function, object)
}

## IS ##########################################################################

is_instrumentr_builtin_call_entry_callback <- function(object) { # nolint
    inherits(object, 'instrumentr_builtin_call_entry_callback')
}


################################################################################
## builtin_call_exit
################################################################################

## GET #########################################################################

#' @export
get_builtin_call_exit_callback <- function(context, ...) {
    UseMethod("get_builtin_call_exit_callback")
}

#' @export
get_builtin_call_exit_callback.instrumentr_context <- function(context, ...) { # nolint
    .Call(C_context_get_builtin_call_exit_callback, context)
}

## SET #########################################################################

#' @export
set_builtin_call_exit_callback <- function(context, callback, ...) {
    UseMethod("set_builtin_call_exit_callback")
}

#' @export
set_builtin_call_exit_callback.instrumentr_context <- function(context, callback, ...) { # nolint

    if (!is_instrumentr_builtin_call_exit_callback(callback)) {
        callback <- create_builtin_call_exit_callback(callback)
    }

    .Call(C_context_set_builtin_call_exit_callback, context, callback)

    invisible(NULL)
}

## HAS #########################################################################

#' @export
has_builtin_call_exit_callback <- function(object, ...) { # nolint
    UseMethod("has_builtin_call_exit_callback")
}

#' @export
has_builtin_call_exit_callback.instrumentr_context <- function(object, ...) { # nolint
    .Call(C_context_has_builtin_call_exit_callback, object)
}

## CREATE ######################################################################

#' @export
create_builtin_call_exit_callback <- function(object) { # nolint
    UseMethod("create_builtin_call_exit_callback")
}

#' @export
create_builtin_call_exit_callback.function <- function(object) { # nolint
    stopifnot(is_closure(object) && has_parameters(object, 7))

    .Call(C_builtin_call_exit_callback_create_from_r_function, object)
}

#' @export
create_builtin_call_exit_callback.externalptr <- function(object) { # nolint
    .Call(C_builtin_call_exit_callback_create_from_c_function, object)
}

## IS ##########################################################################

is_instrumentr_builtin_call_exit_callback <- function(object) { # nolint
    inherits(object, 'instrumentr_builtin_call_exit_callback')
}


################################################################################
## special_call_entry
################################################################################

## GET #########################################################################

#' @export
get_special_call_entry_callback <- function(context, ...) {
    UseMethod("get_special_call_entry_callback")
}

#' @export
get_special_call_entry_callback.instrumentr_context <- function(context, ...) { # nolint
    .Call(C_context_get_special_call_entry_callback, context)
}

## SET #########################################################################

#' @export
set_special_call_entry_callback <- function(context, callback, ...) {
    UseMethod("set_special_call_entry_callback")
}

#' @export
set_special_call_entry_callback.instrumentr_context <- function(context, callback, ...) { # nolint

    if (!is_instrumentr_special_call_entry_callback(callback)) {
        callback <- create_special_call_entry_callback(callback)
    }

    .Call(C_context_set_special_call_entry_callback, context, callback)

    invisible(NULL)
}

## HAS #########################################################################

#' @export
has_special_call_entry_callback <- function(object, ...) { # nolint
    UseMethod("has_special_call_entry_callback")
}

#' @export
has_special_call_entry_callback.instrumentr_context <- function(object, ...) { # nolint
    .Call(C_context_has_special_call_entry_callback, object)
}

## CREATE ######################################################################

#' @export
create_special_call_entry_callback <- function(object) { # nolint
    UseMethod("create_special_call_entry_callback")
}

#' @export
create_special_call_entry_callback.function <- function(object) { # nolint
    stopifnot(is_closure(object) && has_parameters(object, 6))

    .Call(C_special_call_entry_callback_create_from_r_function, object)
}

#' @export
create_special_call_entry_callback.externalptr <- function(object) { # nolint
    .Call(C_special_call_entry_callback_create_from_c_function, object)
}

## IS ##########################################################################

is_instrumentr_special_call_entry_callback <- function(object) { # nolint
    inherits(object, 'instrumentr_special_call_entry_callback')
}


################################################################################
## special_call_exit
################################################################################

## GET #########################################################################

#' @export
get_special_call_exit_callback <- function(context, ...) {
    UseMethod("get_special_call_exit_callback")
}

#' @export
get_special_call_exit_callback.instrumentr_context <- function(context, ...) { # nolint
    .Call(C_context_get_special_call_exit_callback, context)
}

## SET #########################################################################

#' @export
set_special_call_exit_callback <- function(context, callback, ...) {
    UseMethod("set_special_call_exit_callback")
}

#' @export
set_special_call_exit_callback.instrumentr_context <- function(context, callback, ...) { # nolint

    if (!is_instrumentr_special_call_exit_callback(callback)) {
        callback <- create_special_call_exit_callback(callback)
    }

    .Call(C_context_set_special_call_exit_callback, context, callback)

    invisible(NULL)
}

## HAS #########################################################################

#' @export
has_special_call_exit_callback <- function(object, ...) { # nolint
    UseMethod("has_special_call_exit_callback")
}

#' @export
has_special_call_exit_callback.instrumentr_context <- function(object, ...) { # nolint
    .Call(C_context_has_special_call_exit_callback, object)
}

## CREATE ######################################################################

#' @export
create_special_call_exit_callback <- function(object) { # nolint
    UseMethod("create_special_call_exit_callback")
}

#' @export
create_special_call_exit_callback.function <- function(object) { # nolint
    stopifnot(is_closure(object) && has_parameters(object, 7))

    .Call(C_special_call_exit_callback_create_from_r_function, object)
}

#' @export
create_special_call_exit_callback.externalptr <- function(object) { # nolint
    .Call(C_special_call_exit_callback_create_from_c_function, object)
}

## IS ##########################################################################

is_instrumentr_special_call_exit_callback <- function(object) { # nolint
    inherits(object, 'instrumentr_special_call_exit_callback')
}


################################################################################
## eval_entry
################################################################################

## GET #########################################################################

#' @export
get_eval_entry_callback <- function(context, ...) {
    UseMethod("get_eval_entry_callback")
}

#' @export
get_eval_entry_callback.instrumentr_context <- function(context, ...) { # nolint
    .Call(C_context_get_eval_entry_callback, context)
}

## SET #########################################################################

#' @export
set_eval_entry_callback <- function(context, callback, ...) {
    UseMethod("set_eval_entry_callback")
}

#' @export
set_eval_entry_callback.instrumentr_context <- function(context, callback, ...) { # nolint

    if (!is_instrumentr_eval_entry_callback(callback)) {
        callback <- create_eval_entry_callback(callback)
    }

    .Call(C_context_set_eval_entry_callback, context, callback)

    invisible(NULL)
}

## HAS #########################################################################

#' @export
has_eval_entry_callback <- function(object, ...) { # nolint
    UseMethod("has_eval_entry_callback")
}

#' @export
has_eval_entry_callback.instrumentr_context <- function(object, ...) { # nolint
    .Call(C_context_has_eval_entry_callback, object)
}

## CREATE ######################################################################

#' @export
create_eval_entry_callback <- function(object) { # nolint
    UseMethod("create_eval_entry_callback")
}

#' @export
create_eval_entry_callback.function <- function(object) { # nolint
    stopifnot(is_closure(object) && has_parameters(object, 4))

    .Call(C_eval_entry_callback_create_from_r_function, object)
}

#' @export
create_eval_entry_callback.externalptr <- function(object) { # nolint
    .Call(C_eval_entry_callback_create_from_c_function, object)
}

## IS ##########################################################################

is_instrumentr_eval_entry_callback <- function(object) { # nolint
    inherits(object, 'instrumentr_eval_entry_callback')
}


################################################################################
## eval_exit
################################################################################

## GET #########################################################################

#' @export
get_eval_exit_callback <- function(context, ...) {
    UseMethod("get_eval_exit_callback")
}

#' @export
get_eval_exit_callback.instrumentr_context <- function(context, ...) { # nolint
    .Call(C_context_get_eval_exit_callback, context)
}

## SET #########################################################################

#' @export
set_eval_exit_callback <- function(context, callback, ...) {
    UseMethod("set_eval_exit_callback")
}

#' @export
set_eval_exit_callback.instrumentr_context <- function(context, callback, ...) { # nolint

    if (!is_instrumentr_eval_exit_callback(callback)) {
        callback <- create_eval_exit_callback(callback)
    }

    .Call(C_context_set_eval_exit_callback, context, callback)

    invisible(NULL)
}

## HAS #########################################################################

#' @export
has_eval_exit_callback <- function(object, ...) { # nolint
    UseMethod("has_eval_exit_callback")
}

#' @export
has_eval_exit_callback.instrumentr_context <- function(object, ...) { # nolint
    .Call(C_context_has_eval_exit_callback, object)
}

## CREATE ######################################################################

#' @export
create_eval_exit_callback <- function(object) { # nolint
    UseMethod("create_eval_exit_callback")
}

#' @export
create_eval_exit_callback.function <- function(object) { # nolint
    stopifnot(is_closure(object) && has_parameters(object, 5))

    .Call(C_eval_exit_callback_create_from_r_function, object)
}

#' @export
create_eval_exit_callback.externalptr <- function(object) { # nolint
    .Call(C_eval_exit_callback_create_from_c_function, object)
}

## IS ##########################################################################

is_instrumentr_eval_exit_callback <- function(object) { # nolint
    inherits(object, 'instrumentr_eval_exit_callback')
}


################################################################################
## gc_allocation
################################################################################

## GET #########################################################################

#' @export
get_gc_allocation_callback <- function(context, ...) {
    UseMethod("get_gc_allocation_callback")
}

#' @export
get_gc_allocation_callback.instrumentr_context <- function(context, ...) { # nolint
    .Call(C_context_get_gc_allocation_callback, context)
}

## SET #########################################################################

#' @export
set_gc_allocation_callback <- function(context, callback, ...) {
    UseMethod("set_gc_allocation_callback")
}

#' @export
set_gc_allocation_callback.instrumentr_context <- function(context, callback, ...) { # nolint

    if (!is_instrumentr_gc_allocation_callback(callback)) {
        callback <- create_gc_allocation_callback(callback)
    }

    .Call(C_context_set_gc_allocation_callback, context, callback)

    invisible(NULL)
}

## HAS #########################################################################

#' @export
has_gc_allocation_callback <- function(object, ...) { # nolint
    UseMethod("has_gc_allocation_callback")
}

#' @export
has_gc_allocation_callback.instrumentr_context <- function(object, ...) { # nolint
    .Call(C_context_has_gc_allocation_callback, object)
}

## CREATE ######################################################################

#' @export
create_gc_allocation_callback <- function(object) { # nolint
    UseMethod("create_gc_allocation_callback")
}

#' @export
create_gc_allocation_callback.function <- function(object) { # nolint
    stopifnot(is_closure(object) && has_parameters(object, 3))

    .Call(C_gc_allocation_callback_create_from_r_function, object)
}

#' @export
create_gc_allocation_callback.externalptr <- function(object) { # nolint
    .Call(C_gc_allocation_callback_create_from_c_function, object)
}

## IS ##########################################################################

is_instrumentr_gc_allocation_callback <- function(object) { # nolint
    inherits(object, 'instrumentr_gc_allocation_callback')
}


################################################################################
## gc_unmark
################################################################################

## GET #########################################################################

#' @export
get_gc_unmark_callback <- function(context, ...) {
    UseMethod("get_gc_unmark_callback")
}

#' @export
get_gc_unmark_callback.instrumentr_context <- function(context, ...) { # nolint
    .Call(C_context_get_gc_unmark_callback, context)
}

## SET #########################################################################

#' @export
set_gc_unmark_callback <- function(context, callback, ...) {
    UseMethod("set_gc_unmark_callback")
}

#' @export
set_gc_unmark_callback.instrumentr_context <- function(context, callback, ...) { # nolint

    if (!is_instrumentr_gc_unmark_callback(callback)) {
        callback <- create_gc_unmark_callback(callback)
    }

    .Call(C_context_set_gc_unmark_callback, context, callback)

    invisible(NULL)
}

## HAS #########################################################################

#' @export
has_gc_unmark_callback <- function(object, ...) { # nolint
    UseMethod("has_gc_unmark_callback")
}

#' @export
has_gc_unmark_callback.instrumentr_context <- function(object, ...) { # nolint
    .Call(C_context_has_gc_unmark_callback, object)
}

## CREATE ######################################################################

#' @export
create_gc_unmark_callback <- function(object) { # nolint
    UseMethod("create_gc_unmark_callback")
}

#' @export
create_gc_unmark_callback.function <- function(object) { # nolint
    stopifnot(is_closure(object) && has_parameters(object, 3))

    .Call(C_gc_unmark_callback_create_from_r_function, object)
}

#' @export
create_gc_unmark_callback.externalptr <- function(object) { # nolint
    .Call(C_gc_unmark_callback_create_from_c_function, object)
}

## IS ##########################################################################

is_instrumentr_gc_unmark_callback <- function(object) { # nolint
    inherits(object, 'instrumentr_gc_unmark_callback')
}


################################################################################
## function_attach
################################################################################

## GET #########################################################################

#' @export
get_function_attach_callback <- function(context, ...) {
    UseMethod("get_function_attach_callback")
}

#' @export
get_function_attach_callback.instrumentr_context <- function(context, ...) { # nolint
    .Call(C_context_get_function_attach_callback, context)
}

## SET #########################################################################

#' @export
set_function_attach_callback <- function(context, callback, ...) {
    UseMethod("set_function_attach_callback")
}

#' @export
set_function_attach_callback.instrumentr_context <- function(context, callback, ...) { # nolint

    if (!is_instrumentr_function_attach_callback(callback)) {
        callback <- create_function_attach_callback(callback)
    }

    .Call(C_context_set_function_attach_callback, context, callback)

    invisible(NULL)
}

## HAS #########################################################################

#' @export
has_function_attach_callback <- function(object, ...) { # nolint
    UseMethod("has_function_attach_callback")
}

#' @export
has_function_attach_callback.instrumentr_context <- function(object, ...) { # nolint
    .Call(C_context_has_function_attach_callback, object)
}

## CREATE ######################################################################

#' @export
create_function_attach_callback <- function(object) { # nolint
    UseMethod("create_function_attach_callback")
}

#' @export
create_function_attach_callback.function <- function(object) { # nolint
    stopifnot(is_closure(object) && has_parameters(object, 4))

    .Call(C_function_attach_callback_create_from_r_function, object)
}

#' @export
create_function_attach_callback.externalptr <- function(object) { # nolint
    .Call(C_function_attach_callback_create_from_c_function, object)
}

## IS ##########################################################################

is_instrumentr_function_attach_callback <- function(object) { # nolint
    inherits(object, 'instrumentr_function_attach_callback')
}


################################################################################
## function_detach
################################################################################

## GET #########################################################################

#' @export
get_function_detach_callback <- function(context, ...) {
    UseMethod("get_function_detach_callback")
}

#' @export
get_function_detach_callback.instrumentr_context <- function(context, ...) { # nolint
    .Call(C_context_get_function_detach_callback, context)
}

## SET #########################################################################

#' @export
set_function_detach_callback <- function(context, callback, ...) {
    UseMethod("set_function_detach_callback")
}

#' @export
set_function_detach_callback.instrumentr_context <- function(context, callback, ...) { # nolint

    if (!is_instrumentr_function_detach_callback(callback)) {
        callback <- create_function_detach_callback(callback)
    }

    .Call(C_context_set_function_detach_callback, context, callback)

    invisible(NULL)
}

## HAS #########################################################################

#' @export
has_function_detach_callback <- function(object, ...) { # nolint
    UseMethod("has_function_detach_callback")
}

#' @export
has_function_detach_callback.instrumentr_context <- function(object, ...) { # nolint
    .Call(C_context_has_function_detach_callback, object)
}

## CREATE ######################################################################

#' @export
create_function_detach_callback <- function(object) { # nolint
    UseMethod("create_function_detach_callback")
}

#' @export
create_function_detach_callback.function <- function(object) { # nolint
    stopifnot(is_closure(object) && has_parameters(object, 4))

    .Call(C_function_detach_callback_create_from_r_function, object)
}

#' @export
create_function_detach_callback.externalptr <- function(object) { # nolint
    .Call(C_function_detach_callback_create_from_c_function, object)
}

## IS ##########################################################################

is_instrumentr_function_detach_callback <- function(object) { # nolint
    inherits(object, 'instrumentr_function_detach_callback')
}


################################################################################
## package_attach
################################################################################

## GET #########################################################################

#' @export
get_package_attach_callback <- function(context, ...) {
    UseMethod("get_package_attach_callback")
}

#' @export
get_package_attach_callback.instrumentr_context <- function(context, ...) { # nolint
    .Call(C_context_get_package_attach_callback, context)
}

## SET #########################################################################

#' @export
set_package_attach_callback <- function(context, callback, ...) {
    UseMethod("set_package_attach_callback")
}

#' @export
set_package_attach_callback.instrumentr_context <- function(context, callback, ...) { # nolint

    if (!is_instrumentr_package_attach_callback(callback)) {
        callback <- create_package_attach_callback(callback)
    }

    .Call(C_context_set_package_attach_callback, context, callback)

    invisible(NULL)
}

## HAS #########################################################################

#' @export
has_package_attach_callback <- function(object, ...) { # nolint
    UseMethod("has_package_attach_callback")
}

#' @export
has_package_attach_callback.instrumentr_context <- function(object, ...) { # nolint
    .Call(C_context_has_package_attach_callback, object)
}

## CREATE ######################################################################

#' @export
create_package_attach_callback <- function(object) { # nolint
    UseMethod("create_package_attach_callback")
}

#' @export
create_package_attach_callback.function <- function(object) { # nolint
    stopifnot(is_closure(object) && has_parameters(object, 3))

    .Call(C_package_attach_callback_create_from_r_function, object)
}

#' @export
create_package_attach_callback.externalptr <- function(object) { # nolint
    .Call(C_package_attach_callback_create_from_c_function, object)
}

## IS ##########################################################################

is_instrumentr_package_attach_callback <- function(object) { # nolint
    inherits(object, 'instrumentr_package_attach_callback')
}


################################################################################
## package_detach
################################################################################

## GET #########################################################################

#' @export
get_package_detach_callback <- function(context, ...) {
    UseMethod("get_package_detach_callback")
}

#' @export
get_package_detach_callback.instrumentr_context <- function(context, ...) { # nolint
    .Call(C_context_get_package_detach_callback, context)
}

## SET #########################################################################

#' @export
set_package_detach_callback <- function(context, callback, ...) {
    UseMethod("set_package_detach_callback")
}

#' @export
set_package_detach_callback.instrumentr_context <- function(context, callback, ...) { # nolint

    if (!is_instrumentr_package_detach_callback(callback)) {
        callback <- create_package_detach_callback(callback)
    }

    .Call(C_context_set_package_detach_callback, context, callback)

    invisible(NULL)
}

## HAS #########################################################################

#' @export
has_package_detach_callback <- function(object, ...) { # nolint
    UseMethod("has_package_detach_callback")
}

#' @export
has_package_detach_callback.instrumentr_context <- function(object, ...) { # nolint
    .Call(C_context_has_package_detach_callback, object)
}

## CREATE ######################################################################

#' @export
create_package_detach_callback <- function(object) { # nolint
    UseMethod("create_package_detach_callback")
}

#' @export
create_package_detach_callback.function <- function(object) { # nolint
    stopifnot(is_closure(object) && has_parameters(object, 3))

    .Call(C_package_detach_callback_create_from_r_function, object)
}

#' @export
create_package_detach_callback.externalptr <- function(object) { # nolint
    .Call(C_package_detach_callback_create_from_c_function, object)
}

## IS ##########################################################################

is_instrumentr_package_detach_callback <- function(object) { # nolint
    inherits(object, 'instrumentr_package_detach_callback')
}


################################################################################
## package_load
################################################################################

## GET #########################################################################

#' @export
get_package_load_callback <- function(context, ...) {
    UseMethod("get_package_load_callback")
}

#' @export
get_package_load_callback.instrumentr_context <- function(context, ...) { # nolint
    .Call(C_context_get_package_load_callback, context)
}

## SET #########################################################################

#' @export
set_package_load_callback <- function(context, callback, ...) {
    UseMethod("set_package_load_callback")
}

#' @export
set_package_load_callback.instrumentr_context <- function(context, callback, ...) { # nolint

    if (!is_instrumentr_package_load_callback(callback)) {
        callback <- create_package_load_callback(callback)
    }

    .Call(C_context_set_package_load_callback, context, callback)

    invisible(NULL)
}

## HAS #########################################################################

#' @export
has_package_load_callback <- function(object, ...) { # nolint
    UseMethod("has_package_load_callback")
}

#' @export
has_package_load_callback.instrumentr_context <- function(object, ...) { # nolint
    .Call(C_context_has_package_load_callback, object)
}

## CREATE ######################################################################

#' @export
create_package_load_callback <- function(object) { # nolint
    UseMethod("create_package_load_callback")
}

#' @export
create_package_load_callback.function <- function(object) { # nolint
    stopifnot(is_closure(object) && has_parameters(object, 3))

    .Call(C_package_load_callback_create_from_r_function, object)
}

#' @export
create_package_load_callback.externalptr <- function(object) { # nolint
    .Call(C_package_load_callback_create_from_c_function, object)
}

## IS ##########################################################################

is_instrumentr_package_load_callback <- function(object) { # nolint
    inherits(object, 'instrumentr_package_load_callback')
}


################################################################################
## package_unload
################################################################################

## GET #########################################################################

#' @export
get_package_unload_callback <- function(context, ...) {
    UseMethod("get_package_unload_callback")
}

#' @export
get_package_unload_callback.instrumentr_context <- function(context, ...) { # nolint
    .Call(C_context_get_package_unload_callback, context)
}

## SET #########################################################################

#' @export
set_package_unload_callback <- function(context, callback, ...) {
    UseMethod("set_package_unload_callback")
}

#' @export
set_package_unload_callback.instrumentr_context <- function(context, callback, ...) { # nolint

    if (!is_instrumentr_package_unload_callback(callback)) {
        callback <- create_package_unload_callback(callback)
    }

    .Call(C_context_set_package_unload_callback, context, callback)

    invisible(NULL)
}

## HAS #########################################################################

#' @export
has_package_unload_callback <- function(object, ...) { # nolint
    UseMethod("has_package_unload_callback")
}

#' @export
has_package_unload_callback.instrumentr_context <- function(object, ...) { # nolint
    .Call(C_context_has_package_unload_callback, object)
}

## CREATE ######################################################################

#' @export
create_package_unload_callback <- function(object) { # nolint
    UseMethod("create_package_unload_callback")
}

#' @export
create_package_unload_callback.function <- function(object) { # nolint
    stopifnot(is_closure(object) && has_parameters(object, 3))

    .Call(C_package_unload_callback_create_from_r_function, object)
}

#' @export
create_package_unload_callback.externalptr <- function(object) { # nolint
    .Call(C_package_unload_callback_create_from_c_function, object)
}

## IS ##########################################################################

is_instrumentr_package_unload_callback <- function(object) { # nolint
    inherits(object, 'instrumentr_package_unload_callback')
}


################################################################################
## variable_assignment
################################################################################

## GET #########################################################################

#' @export
get_variable_assignment_callback <- function(context, ...) {
    UseMethod("get_variable_assignment_callback")
}

#' @export
get_variable_assignment_callback.instrumentr_context <- function(context, ...) { # nolint
    .Call(C_context_get_variable_assignment_callback, context)
}

## SET #########################################################################

#' @export
set_variable_assignment_callback <- function(context, callback, ...) {
    UseMethod("set_variable_assignment_callback")
}

#' @export
set_variable_assignment_callback.instrumentr_context <- function(context, callback, ...) { # nolint

    if (!is_instrumentr_variable_assignment_callback(callback)) {
        callback <- create_variable_assignment_callback(callback)
    }

    .Call(C_context_set_variable_assignment_callback, context, callback)

    invisible(NULL)
}

## HAS #########################################################################

#' @export
has_variable_assignment_callback <- function(object, ...) { # nolint
    UseMethod("has_variable_assignment_callback")
}

#' @export
has_variable_assignment_callback.instrumentr_context <- function(object, ...) { # nolint
    .Call(C_context_has_variable_assignment_callback, object)
}

## CREATE ######################################################################

#' @export
create_variable_assignment_callback <- function(object) { # nolint
    UseMethod("create_variable_assignment_callback")
}

#' @export
create_variable_assignment_callback.function <- function(object) { # nolint
    stopifnot(is_closure(object) && has_parameters(object, 5))

    .Call(C_variable_assignment_callback_create_from_r_function, object)
}

#' @export
create_variable_assignment_callback.externalptr <- function(object) { # nolint
    .Call(C_variable_assignment_callback_create_from_c_function, object)
}

## IS ##########################################################################

is_instrumentr_variable_assignment_callback <- function(object) { # nolint
    inherits(object, 'instrumentr_variable_assignment_callback')
}


################################################################################
## variable_definition
################################################################################

## GET #########################################################################

#' @export
get_variable_definition_callback <- function(context, ...) {
    UseMethod("get_variable_definition_callback")
}

#' @export
get_variable_definition_callback.instrumentr_context <- function(context, ...) { # nolint
    .Call(C_context_get_variable_definition_callback, context)
}

## SET #########################################################################

#' @export
set_variable_definition_callback <- function(context, callback, ...) {
    UseMethod("set_variable_definition_callback")
}

#' @export
set_variable_definition_callback.instrumentr_context <- function(context, callback, ...) { # nolint

    if (!is_instrumentr_variable_definition_callback(callback)) {
        callback <- create_variable_definition_callback(callback)
    }

    .Call(C_context_set_variable_definition_callback, context, callback)

    invisible(NULL)
}

## HAS #########################################################################

#' @export
has_variable_definition_callback <- function(object, ...) { # nolint
    UseMethod("has_variable_definition_callback")
}

#' @export
has_variable_definition_callback.instrumentr_context <- function(object, ...) { # nolint
    .Call(C_context_has_variable_definition_callback, object)
}

## CREATE ######################################################################

#' @export
create_variable_definition_callback <- function(object) { # nolint
    UseMethod("create_variable_definition_callback")
}

#' @export
create_variable_definition_callback.function <- function(object) { # nolint
    stopifnot(is_closure(object) && has_parameters(object, 5))

    .Call(C_variable_definition_callback_create_from_r_function, object)
}

#' @export
create_variable_definition_callback.externalptr <- function(object) { # nolint
    .Call(C_variable_definition_callback_create_from_c_function, object)
}

## IS ##########################################################################

is_instrumentr_variable_definition_callback <- function(object) { # nolint
    inherits(object, 'instrumentr_variable_definition_callback')
}


################################################################################
## variable_lookup
################################################################################

## GET #########################################################################

#' @export
get_variable_lookup_callback <- function(context, ...) {
    UseMethod("get_variable_lookup_callback")
}

#' @export
get_variable_lookup_callback.instrumentr_context <- function(context, ...) { # nolint
    .Call(C_context_get_variable_lookup_callback, context)
}

## SET #########################################################################

#' @export
set_variable_lookup_callback <- function(context, callback, ...) {
    UseMethod("set_variable_lookup_callback")
}

#' @export
set_variable_lookup_callback.instrumentr_context <- function(context, callback, ...) { # nolint

    if (!is_instrumentr_variable_lookup_callback(callback)) {
        callback <- create_variable_lookup_callback(callback)
    }

    .Call(C_context_set_variable_lookup_callback, context, callback)

    invisible(NULL)
}

## HAS #########################################################################

#' @export
has_variable_lookup_callback <- function(object, ...) { # nolint
    UseMethod("has_variable_lookup_callback")
}

#' @export
has_variable_lookup_callback.instrumentr_context <- function(object, ...) { # nolint
    .Call(C_context_has_variable_lookup_callback, object)
}

## CREATE ######################################################################

#' @export
create_variable_lookup_callback <- function(object) { # nolint
    UseMethod("create_variable_lookup_callback")
}

#' @export
create_variable_lookup_callback.function <- function(object) { # nolint
    stopifnot(is_closure(object) && has_parameters(object, 5))

    .Call(C_variable_lookup_callback_create_from_r_function, object)
}

#' @export
create_variable_lookup_callback.externalptr <- function(object) { # nolint
    .Call(C_variable_lookup_callback_create_from_c_function, object)
}

## IS ##########################################################################

is_instrumentr_variable_lookup_callback <- function(object) { # nolint
    inherits(object, 'instrumentr_variable_lookup_callback')
}


################################################################################
## variable_removal
################################################################################

## GET #########################################################################

#' @export
get_variable_removal_callback <- function(context, ...) {
    UseMethod("get_variable_removal_callback")
}

#' @export
get_variable_removal_callback.instrumentr_context <- function(context, ...) { # nolint
    .Call(C_context_get_variable_removal_callback, context)
}

## SET #########################################################################

#' @export
set_variable_removal_callback <- function(context, callback, ...) {
    UseMethod("set_variable_removal_callback")
}

#' @export
set_variable_removal_callback.instrumentr_context <- function(context, callback, ...) { # nolint

    if (!is_instrumentr_variable_removal_callback(callback)) {
        callback <- create_variable_removal_callback(callback)
    }

    .Call(C_context_set_variable_removal_callback, context, callback)

    invisible(NULL)
}

## HAS #########################################################################

#' @export
has_variable_removal_callback <- function(object, ...) { # nolint
    UseMethod("has_variable_removal_callback")
}

#' @export
has_variable_removal_callback.instrumentr_context <- function(object, ...) { # nolint
    .Call(C_context_has_variable_removal_callback, object)
}

## CREATE ######################################################################

#' @export
create_variable_removal_callback <- function(object) { # nolint
    UseMethod("create_variable_removal_callback")
}

#' @export
create_variable_removal_callback.function <- function(object) { # nolint
    stopifnot(is_closure(object) && has_parameters(object, 4))

    .Call(C_variable_removal_callback_create_from_r_function, object)
}

#' @export
create_variable_removal_callback.externalptr <- function(object) { # nolint
    .Call(C_variable_removal_callback_create_from_c_function, object)
}

## IS ##########################################################################

is_instrumentr_variable_removal_callback <- function(object) { # nolint
    inherits(object, 'instrumentr_variable_removal_callback')
}
