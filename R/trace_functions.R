
#' @export
trace_functions <- function(object, qualified_function_names, ...) {
    UseMethod("trace_functions")
}

#' @export
trace_functions.instrumentr_context <- function(object, qualified_function_names, ...) { # nolint
    is_vector_character(qualified_function_names)

    for (qualified_function_name in qualified_function_names) {
        name_components <- split_name_components(qualified_function_name)
        package_name <- name_components[1]
        function_name <- name_components[2]
        .Call(C_context_trace_function, object, package_name, function_name)
    }
}

split_name_components <- function(qualified_function_name) {

    components <- unlist(strsplit(qualified_function_name, ":::", fixed=TRUE))

    if (length(components) == 1) {
        components <- unlist(strsplit(qualified_function_name, "::", fixed=TRUE))

        if (length(components) == 1) {
            ##TODO: better error message for names without ::
            stopifnot(sprintf("Expected fully qualified function name, not '%s'", qualified_function_name))
        }
    }

    if (length(components) > 2) {
        first <- components[1]
        rest <- components[2:length(components)]
        components <- c(first, paste(rest, sep = "", collapse = ""))
    }

    components
}
