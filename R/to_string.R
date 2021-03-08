
#' Convert instrumentr objects to a character representation for printing
#'
#' @param object Input instrument object.
#'
#' @return A character representation of the input object.
#' @examples
#'
#' to_string(create_tracer())
#'
#' @export
to_string <- function(object, ...) {
    UseMethod("to_string")
}

expr_to_string <- function(expr, n) {
    indentation <- paste(rep(" ", n - 1), sep="", collapse="")
    expr_strs <- deparse(expr)
    paste(expr_strs[1],
          paste(indentation, expr_strs[2:length(expr_strs)], collapse = "\n"),
          sep = "\n")
}

logical_to_string <- function(object) {
    c("FALSE", "TRUE")[object[1] + 1]
}

#' @export
to_string.default <- function(object, ...) {
    sprintf("<%s: %s>", typeof(object), get_object_details(object)$address)
}

#' @export
to_string.NULL <- function(object, ...) {
    "<null>"
}

#' @export
to_string.call <- function(object, ...) {
    paste(deparse(object), collapse = " ", sep = " ")
}

#' @export
to_string.instrumentr_tracer <- function(object, ...) {

    get_callback_representation <- function(event) {
        representation <- ""
        if(has_callback(object, event)) {
            callback <- get_callback(object, event)
            representation <- sprintf("%s=%s,\n", event, to_string(callback))
        }
        representation
    }

    representation <-
      sprintf("Context(%s%s%s%s%s%s%s%s%s%s%s",
              get_callback_representation(EVENT_TRACING_ENTRY),
              get_callback_representation(EVENT_TRACING_EXIT),
              get_callback_representation(EVENT_PACKAGE_LOAD),
              get_callback_representation(EVENT_PACKAGE_UNLOAD),
              get_callback_representation(EVENT_PACKAGE_ATTACH),
              get_callback_representation(EVENT_PACKAGE_DETACH),
              get_callback_representation(EVENT_GC_ALLOCATION),
              get_callback_representation(EVENT_VARIABLE_DEFINITION),
              get_callback_representation(EVENT_VARIABLE_ASSIGNMENT),
              get_callback_representation(EVENT_VARIABLE_REMOVAL),
              get_callback_representation(EVENT_VARIABLE_LOOKUP))

    representation
}

#' @export
to_string.instrumentr_result <- function(object, ...) {

    representation <-
        if (is_value(object)) {
            sprintf("Result(value=%s)", to_string(get_value(object)))
        }
        else {
            sprintf("Result(error=%s)", to_string(get_error(object)))
        }

    representation
}

#' @export
to_string.instrumentr_function <- function(object, ...) {
    template <- "Function(name='%s', parameter_count=%d, definition=%s, public=%s, s3_generic=%s, s3_method=%s)"
    representation <- sprintf(template,
                              get_name(object),
                              get_parameter_count(object),
                              to_string(get_definition(object)),
                              logical_to_string(is_public(object)),
                              logical_to_string(is_s3_generic(object)),
                              logical_to_string(is_s3_method(object)))

    representation
}

#' @export
to_string.instrumentr_argument <- function(object, ...) {
    name <- get_name(object)

    name_str <- ""
    if (name != "") {
        name_str <- sprintf("name='%s', ", name)
    }

    result <- get_result(object)
    result_str <- ""
    if (is_defined(result)) {
        result_str <- sprintf("result=%s, ", to_string(result))
    }

    representation <- sprintf("Argument(%sexpression=%s, %sevaluated=%s)",
                              name_str,
                              to_string(get_expression(object)),
                              result_str,
                              c("FALSE", "TRUE")[is_evaluated(object) + 1])

    representation
}

#' @export
to_string.instrumentr_call <- function(object, ...) {
    representation <- sprintf("Call(expression=%s, environment=%s)",
                              to_string(get_expression(object)),
                              to_string(get_environment(object)))

    representation
}

#' @export
to_string.instrumentr_application <- function(object, ...) { # nolint
    representation <- sprintf("Application(name='%s', directory='%s', environment=%s)",
                              get_name(object),
                              get_directory(object),
                              to_string(get_environment(object)))

    representation
}

#' @export
to_string.instrumentr_parameter <- function(object, ...) { # nolint
    representation <- sprintf("Parameter(name='%s', position=%d, missing=%s, vararg=%s)",
                              get_name(object),
                              get_position(object),
                              logical_to_string(is_missing(object)),
                              logical_to_string(is_vararg(object)))

    representation
}

#' @export
to_string.instrumentr_package <- function(object, ...) {
    representation <- sprintf("Package(name='%s', directory='%s', namespace=%s)",
                              get_name(object),
                              get_directory(object),
                              to_string(get_namespace(object)))

    representation
}

#' @export
to_string.instrumentr_error <- function(object, ...) {
    representation <- sprintf("Error(source='%s', message='%s', call=%s)",
                              get_source(object),
                              get_message(object),
                              to_string(get_call(object)))

    representation
}

#' @export
to_string.instrumentr_call_stack <- function(object, ...) { # nolint
    size <- get_size(object)

    representation <- sprintf("CallStack(%d frames)", size)

    if (size != 0) {
        frames <- character(0)

        for (index in 1:size) {
            call_object <- peek(object, index)
            frames <- c(frames, to_string(call_object))
        }

        ## NOTE: "├── " is written using unicode sequence
        ## to avoid R CMD check warnings
        prefix <- "\u251C\u2500\u2500 "

        ## NOTE: "█" is written using unicode sequence
        ## to avoid R CMD check warnings
        suffix <- "\u2588"

        frame_representation <- paste(paste(prefix, frames, collapse = "\n"), suffix, sep = "\n")
        representation <- paste(representation, frame_representation, sep = "\n")
    }

    representation
}

#' @export
to_string.instrumentr_undefined <- function(object, ...) { # nolint
    "<undefined>"
}
