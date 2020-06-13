get_function_table <- function(package_env) {

    package_ns <- package_env$.__NAMESPACE__.

    s3_method_function_names <- character(0)

    s3_generic_function_names <- character(0)

    public_function_names <- character(0)

    if (!is.null(package_ns)) {

        s3_table <- package_ns$S3methods

        exports_env <- package_ns$exports

        if (!is.null(s3_table)) {
            s3_generic_function_names <- s3_table[, 1]
            s3_method_function_names <- paste(s3_table[, 1], s3_table[, 2], sep = ".")
        }

        if (!is.null(exports_env)) {
            public_function_names <- ls(exports_env, all.names = TRUE)
        }
    }

    function_names <- ls(package_env, all.names = TRUE)

    function_table <- new.env(parent = emptyenv())

    for (function_name in function_names) {
        function_obj <- get(function_name, envir=package_env)

        if (!is_closure(function_obj)) next

        s3_method <- function_name %in% s3_method_function_names

        s3_generic <- function_name %in% s3_generic_function_names

        public <- function_name %in% public_function_names

        assign(function_name,
               list(function_obj = function_obj,
                    public = public,
                    s3_generic = s3_generic,
                    s3_method = s3_method),
               envir=function_table)

    }

    function_table
}
