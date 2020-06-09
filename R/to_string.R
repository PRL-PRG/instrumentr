
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

    application_load_callback <- get_application_load_callback(object)
    application_unload_callback <- get_application_unload_callback(object)
    application_attach_callback <- get_application_attach_callback(object)
    application_detach_callback <- get_application_detach_callback(object)
    package_load_callback <- get_package_load_callback(object)
    package_unload_callback <- get_package_unload_callback(object)
    package_attach_callback <- get_package_attach_callback(object)
    package_detach_callback <- get_package_detach_callback(object)
    function_attach_callback <- get_function_attach_callback(object)
    function_detach_callback <- get_function_detach_callback(object)
    call_entry_callback <- get_call_entry_callback(object)
    call_exit_callback <- get_call_exit_callback(object)
    packages <- get_traced_packages(object)
    functions <- character(0)
    for (package in packages) {
        functions <- c(functions, get_traced_functions(object, package))
    }

    representation <-
      paste("Context(application_load_callback = ", to_string(application_load_callback), ",\n",
            "        application_unload_callback = ", to_string(application_unload_callback), ",\n",
            "        application_attach_callback = ", to_string(application_attach_callback), ",\n",
            "        application_detach_callback = ", to_string(application_detach_callback), ",\n",
            "        package_load_callback = ", to_string(package_load_callback), ",\n",
            "        package_unload_callback = ", to_string(package_unload_callback), ",\n",
            "        package_attach_callback = ", to_string(package_attach_callback), ",\n",
            "        package_detach_callback = ", to_string(package_detach_callback), ",\n",
            "        function_attach_callback = ", to_string(function_attach_callback), ",\n",
            "        function_detach_callback = ", to_string(function_detach_callback), ",\n",
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
    representation <- sprintf("Parameter(name='%s', position=%d, missing=%s, vararg=%s)",
                              get_name(object),
                              get_position(object),
                              logical_to_string(is_missing(object)),
                              logical_to_string(is_vararg(object)))

    representation
}

#' @export
to_string.lightr_argument <- function(object, ...) {
    representation <- sprintf("Argument(name='%s', expression=%s, result=%s, evaluated=%s)",
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
