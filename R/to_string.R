
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
to_string <- function(object, ...) {
    UseMethod("to_string")
}

#' @export
#' @importFrom injectr sexp_address
to_string.default <- function(object, ...) {
    sprintf("<%s: %s>", typeof(object), sexp_address(object))
}

#' @export
to_string.NULL <- function(object, ...) {
    "<null>"
}

#' @export
to_string.lightr_undefined <- function(object, ...) {
    "<undefined>"
}

#' @export
to_string.call <- function(object, ...) {
    paste(deparse(object), collapse = " ", sep = " ")
}

#' @export
to_string.lightr_context <- function(object, ...) {

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
        representation <- ""
        if (check(value)) {
            representation <- sprintf("%s=%s%s", name, to_string(value), sep)
        }
        representation
    }

    representation <-
      sprintf("Context(%s%s%s%s%s%s%s%s%s%s%s%s%s%s",
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
              get_field_representation("call_exit_callback", get_call_exit_callback),
              get_value_representation("packages", packages, function(v) length(v) > 0, ", "),
              get_value_representation("functions", functions, function(v) length(v) > 0, ")"))

    representation
}

#' @export
to_string.lightr_application <- function(object, ...) {
    representation <- sprintf("Application(name='%s', directory='%s', environment=%s)",
                              get_name(object),
                              get_directory(object),
                              to_string(get_environment(object)))

    representation
}

#' @export
to_string.lightr_package <- function(object, ...) {
    representation <- sprintf("Package(name='%s', directory='%s', environment=%s)",
                              get_name(object),
                              get_directory(object),
                              to_string(get_environment(object)))

    representation
}

#' @export
to_string.lightr_function <- function(object, ...) {
    representation <- sprintf("Function(name='%s', parameter_count=%d, definition=%s)",
                              get_name(object),
                              get_parameter_count(object),
                              to_string(get_definition(object)))

    representation
}

#' @export
to_string.lightr_call <- function(object, ...) {
    representation <- sprintf("Call(expression=%s, environment=%s)",
                              to_string(get_expression(object)),
                              to_string(get_environment(object)))

    representation
}

#' @export
to_string.lightr_parameter <- function(object, ...) {
    representation <- sprintf("Parameter(name='%s', position=%d, missing=%s, vararg=%s)",
                              get_name(object),
                              get_position(object),
                              logical_to_string(is_missing(object)),
                              logical_to_string(is_vararg(object)))

    representation
}

#' @export
to_string.lightr_argument <- function(object, ...) {
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
to_string.lightr_call_stack <- function(object, ...) {
    size <- get_size(object)

    representation <- sprintf("CallStack(%d frames)", size)

    if (size != 0) {
        frames <- character(0)

        for (index in 1:size) {
            call_object <- peek_frame(object, index)
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
to_string.lightr_result <- function(object, ...) {

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
to_string.lightr_error <- function(object, ...) {
    representation <- sprintf("Error(source='%s', message='%s', call=%s)",
                              get_source(object),
                              get_message(object),
                              to_string(get_call(object)))

    representation
}
