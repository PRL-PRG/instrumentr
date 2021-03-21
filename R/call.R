#' API for call objects
#'
#' @description
#' TODO
#'
#' @name call
NULL


#' @export
#' @rdname call
get_environment.instrumentr_call <- function(object, ...) { # nolint
    .Call(r_instrumentr_call_get_environment, object)
}

#' @export
#' @rdname call
get_frame_position.instrumentr_call <- function(object, ...) { # nolint
    .Call(r_instrumentr_call_get_frame_position, object)
}

#' @export
#' @rdname call
get_caller.instrumentr_call <- function(object, ...) { # nolint
    frame_position <- get_frame_position(object)

    frames <- sys.frames()
    calls <- sys.calls()
    parents <- sys.parents()

    caller_frame_position <- parents[frame_position]


    if (caller_frame_position == 0) {
        stop("caller of topmost call is undefined")
    }

    caller_env <- frames[[caller_frame_position]]
    caller_expr <- calls[[caller_frame_position]]


    function_name <- ""

    if (is_language(caller_expr)) {
        function_name <- caller_expr[[1]]
    }

    if (is_symbol(function_name)) {
        function_name <- as.character(function_name)
    }
    else {
        function_name <- ""
    }

    definition <- sys.function(caller_frame_position)
    definition_depth <- 0

    if (!is_closure(definition)) {
        package_name <- "base"
    }
    else {
        env <- environment(definition)

        package_name <- environmentName(env)

        while (package_name == "") {
            definition_depth <- definition_depth + 1
            if (is.null(env)) {
                print(definition)
                print(function_name)
                stop("problem here")
            }

            env <- parent.env(env)
            package_name <- environmentName(env)
        }
    }

    list(package_name = package_name,
         function_name = function_name,
         definition = definition,
         environment = caller_env,
         definition_depth = definition_depth,
         frame_position = caller_frame_position,
         call_expression = caller_expr)
}

#' @export
#' @rdname call
get_expression.instrumentr_call <- function(object, ...) { # nolint
    .Call(r_instrumentr_call_get_expression, object)
}

#' @export
#' @rdname call
get_function.instrumentr_call <- function(object, ...) {
    .Call(r_instrumentr_call_get_function, object)
}


#' @export
#' @rdname call
get_parameter_count.instrumentr_call <- function(object) { # nolint
    .Call(r_instrumentr_call_get_parameter_count, object)
}

#' @export
#' @rdname call
get_parameter.instrumentr_call <- function(object, position_or_name) { # nolint
    if (is_scalar_character(position_or_name)) {
        .Call(r_instrumentr_call_get_parameter_by_name, object, position_or_name)
    }

    else if (is_scalar_integer(position_or_name)) {
        .Call(r_instrumentr_call_get_parameter_by_position, object, position_or_name)
    }

    else if (is_scalar_real(position_or_name)) {
        position_or_name <- as.integer(position_or_name)
        .Call(r_instrumentr_call_get_parameter_by_position, object, position_or_name)
    }

    else {
        message <- sprintf("unexpected 'position_or_name' value of type '%s'", typeof(position_or_name))
        stop(message)
    }
}

#' @export
#' @rdname call
get_parameters.instrumentr_call <- function(call, ...) { # nolint
    .Call(r_instrumentr_call_get_parameters, call)
}

#' @export
#' @rdname call
get_result.instrumentr_call <- function(object, ...) {
    .Call(r_instrumentr_call_get_result, object)
}

#' @export
#' @rdname call
has_result.instrumentr_call <- function(object, ...) {
    .Call(r_instrumentr_call_has_result, object)
}

#' @export
#' @rdname call
is_active.instrumentr_call <- function(object) {
    .Call(r_instrumentr_call_is_active, object)
}
