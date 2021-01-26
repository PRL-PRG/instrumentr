
create_package <- function(package_name,
                           package_directory = dirname(system.file(package=package_name)),
                           package_environment = getNamespace(package_name)) {
    stopifnot(is_scalar_character(package_name))
    stopifnot(is_scalar_character(package_directory))
    stopifnot(is_environment(package_environment))

    package_ptr <- .Call(C_instrumentr_package_create, package_name, package_directory, package_environment)

    function_table <- get_function_table(package_environment)

    for (function_name in ls(function_table)) {

        function_info <- get(function_name, envir=function_table)

        function_ptr <- create_function(function_name,
                                        length(formals(function_info$definition)),
                                        function_info$definition,
                                        function_info$public,
                                        function_info$s3_generic,
                                        function_info$s3_method)

        .Call(C_instrumentr_package_add_function, package_ptr, function_ptr)
    }

    message("Added ", length(get_functions(package_ptr)), " functions from ", package_name)

    package_ptr
}

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
        definition <- get(function_name, envir=package_env)

        if (!is_closure(definition)) next

        s3_method <- function_name %in% s3_method_function_names

        s3_generic <- function_name %in% s3_generic_function_names

        public <- function_name %in% public_function_names

        assign(function_name,
               list(definition = definition,
                    public = public,
                    s3_generic = s3_generic,
                    s3_method = s3_method),
               envir=function_table)

    }

    function_table
}
