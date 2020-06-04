
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

to_string.default <- function(object, ...) {
}

to_string.NULL <- function(object, ...) {
    "<null>"
}

to_string.function <- function(object, ...) {
    sprintf("<function: %s>", sexp_address(object))
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

    representation <-
      paste("Context(application_entry_callback = ", to_string(application_entry_callback), ",\n",
            "        application_exit_callback = ", to_string(application_exit_callback), ",\n",
            "        package_entry_callback = ", to_string(package_entry_callback), ",\n",
            "        package_exit_callback = ", to_string(package_exit_callback), ",\n",
            "        function_entry_callback = ", to_string(function_entry_callback), ",\n",
            "        function_exit_callback = ", to_string(function_exit_callback), ",\n",
            "        call_entry_callback = ", to_string(call_entry_callback), ",\n",
            "        call_exit_callback = ", to_string(call_exit_callback), ")",
            sep = "",
            collapse = "")

    representation
}

#' @export
#' @importFrom injectr sexp_address
to_string.lightr_application <- function(object, ...) {
    representation <- sprintf("Application(name='%s', directory='%s', environment=<environment: %s>)",
                              get_name(object), get_directory(object), sexp_address(get_environment(object)))

    representation
}

#' @export
to_string.lightr_package <- function(object, ...) {
    representation <- sprintf("Package(name='%s', directory='%s', environment=<environment: %s>)",
                              get_name(object), get_directory(object), sexp_address(get_environment(object)))

    representation
}

#' @export
to_string.lightr_call_stack <- function(object, ...) {
    "call_stack"
}

#' @export
to_string.lightr_function <- function(object, ...) {
    representation <- sprintf("Function(name='%s', parameter_count=%d, object=<closure: %s>)",
                              get_name(object),
                              get_parameter_count(object),
                              sexp_address(get_object(object)))

    representation
}

#' @export
to_string.lightr_call <- function(object, ...) {
    "call"
}

#' @export
to_string.lightr_parameter <- function(object, ...) {
    "parameter"
}

#' @export
to_string.lightr_argument <- function(object, ...) {
    "argument"
}
