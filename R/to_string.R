
#' @export
to_string <- function(object, ...) {
    UseMethod("to_string")
}

#' @export
to_string.lightr_context <- function(object, ...) {

    expr_to_string <- function(expr, n) {
        indentation <- paste(rep(" ", n - 1), sep="", collapse="")
        expr_strs <- deparse(expr)
        paste(expr_strs[1],
              paste(indentation, expr_strs[2:length(expr_strs)], collapse = "\n"),
              sep = "\n")
    }

    initializer <- expr_to_string(get_initializer(object),
                                  nchar("context(initializer = "))

    finalizer <- expr_to_string(get_finalizer(object),
                                nchar("        finalizer = "))

    package_entry_callback <- expr_to_string(get_package_entry_callback(object),
                                             nchar("        package_entry_callback = "))

    package_exit_callback <- expr_to_string(get_package_exit_callback(object),
                                            nchar("        package_exit_callback = "))

    call_entry_callback <- expr_to_string(get_call_entry_callback(object),
                                          nchar("        call_entry_callback = "))

    call_exit_callback <- expr_to_string(get_call_exit_callback(object),
                                         nchar("        call_exit_callback = "))

    representation <-
      paste("context(initializer = ", initializer, ",\n",
            "        finalizer = ", finalizer, ",\n",
            "        package_entry_callback = ", package_entry_callback, ",\n",
            "        package_exit_callback = ", package_exit_callback, ",\n",
            "        call_entry_callback = ", call_entry_callback, ",\n",
            "        call_exit_callback = ", call_entry_callback, ")",
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
