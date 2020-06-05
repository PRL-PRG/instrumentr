
expr_to_string <- function(expr, n) {
    indentation <- paste(rep(" ", n - 1), sep="", collapse="")
    expr_strs <- deparse(expr)
    paste(expr_strs[1],
          paste(indentation, expr_strs[2:length(expr_strs)], collapse = "\n"),
          sep = "\n")
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
to_string.lightr_invalid_value <- function(object, ...) {
    "<invalid value>"
}

#' @export
to_string.call <- function(object, ...) {
    paste(deparse(object), collapse = " ", sep = " ")
}

#' @export
to_string.lightr_context <- function(object, ...) {

    application_entry_callback <- get_application_entry_callback(object)
    application_exit_callback <- get_application_exit_callback(object)
    package_entry_callback <- get_package_entry_callback(object)
    package_exit_callback <- get_package_exit_callback(object)
    function_entry_callback <- get_function_entry_callback(object)
    function_exit_callback <- get_function_exit_callback(object)
    call_entry_callback <- get_call_entry_callback(object)
    call_exit_callback <- get_call_exit_callback(object)
    packages <- get_traced_packages(object)
    functions <- character(0)
    for(package in packages) {
        functions <- c(functions, get_traced_functions(object, package))
    }

    representation <-
      paste("Context(application_entry_callback = ", to_string(application_entry_callback), ",\n",
            "        application_exit_callback = ", to_string(application_exit_callback), ",\n",
            "        package_entry_callback = ", to_string(package_entry_callback), ",\n",
            "        package_exit_callback = ", to_string(package_exit_callback), ",\n",
            "        function_entry_callback = ", to_string(function_entry_callback), ",\n",
            "        function_exit_callback = ", to_string(function_exit_callback), ",\n",
            "        call_entry_callback = ", to_string(call_entry_callback), ",\n",
            "        call_exit_callback = ", to_string(call_exit_callback), ",\n",
            "        packages = ", to_string(packages), ",\n",
            "        functions = ", to_string(functions), ")",
            ")",
            sep = "",
            collapse = "")

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
    representation <- sprintf("Parameter(name='%s', position=%d)",
                              get_name(object),
                              get_position(object))

    representation
}

#' @export
to_string.lightr_argument <- function(object, ...) {
    representation <- sprintf("Argument(name='%s', expression=%s, result=%s, is_evaluated=%s)",
                              get_name(object),
                              to_string(get_expression(object)),
                              to_string(get_result(object)),
                              c("FALSE", "TRUE")[is_evaluated(object) + 1])

    representation
}

#' @export
to_string.lightr_call_stack <- function(object, ...) {
    size <- get_size(object)

    representation <- sprintf("CallStack(%d frames)", size)

    if(size != 0) {
        frames <- character(0)

        for(index in size:1) {
            call_object <- get_frame(object, index)
            frames <- c(frames, to_string(call_object))
        }

        frame_representation <- paste(paste("├── ", frames, collapse = "\n"), "█", sep = "\n")
        representation <- paste(representation, frame_representation, sep = "\n")
    }

    representation
}
