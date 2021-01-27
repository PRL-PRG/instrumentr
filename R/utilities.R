is_scalar_character <- function(vector) {
    is.character(vector) && (length(vector) == 1) && (nchar(vector) != 0)
}

is_vector_character <- function(vector) {
    is.character(vector)
}

is_scalar_logical <- function(vector) {
    is.logical(vector) && (length(vector) == 1)
}

is_scalar_integer <- function(vector) {
    is.integer(vector) && (length(vector) == 1)
}

is_scalar_real <- function(object) {
    is.double(object) && (length(object) == 1)
}

is_environment <- function(env) {
    is.environment(env)
}

is_function <- function(fun) {
    is.function(fun)
}

is_null <- function(obj) {
    is.null(obj)
}

is_symbol <- function(obj) {
    is.symbol(obj)
}

is_closure <- function(obj) {
    typeof(obj) == "closure"
}

is_language <- function(obj) {
    is.language(obj)
}

is_instrumentr_tracer <- function(obj) {
    "instrumentr_tracer" %in% class(obj)
}

is_instrumentr_application <- function(obj) {
    "instrumentr_application" %in% class(obj)
}

is_instrumentr_package <- function(obj) {
    "instrumentr_package" %in% class(obj)
}

is_instrumentr_function <- function(obj) {
    "instrumentr_function" %in% class(obj)
}

has_parameters <- function(fun, n, include_varargs = TRUE) {
    parameters <- formals(fun)
    cond <- length(parameters) >= n
    if (!cond && include_varargs) {
        cond <- "..." %in% names(parameters)
    }
    cond
}

get_package_name <- function(fun) {
    name <- environmentName(environment(fun))
    if (name == "R_GlobalEnv") ".GlobalEnv"
    else name
}

#' @export
get_installed_package_names <- function(exclude = "instrumentr") {

    stopifnot(is_vector_character(exclude))

    ## NOTE: directly calling installed.packages results in
    ## warnings on R CMD check
    local_installed_packages <- get("installed.packages", envir=getNamespace("utils"))
    installed_package_names <- local_installed_packages()[, 1]
    setdiff(installed_package_names, exclude)
}

#' @export
get_package_function_names <- function(package_name,
                                       public = TRUE,
                                       private = TRUE) {

    stopifnot(is_scalar_character(package_name))
    stopifnot(is_scalar_logical(public))
    stopifnot(is_scalar_logical(private))

    function_names <- character(0)

    if (public) {

        function_names <- get_public_package_function_names(package_name)

        function_names <- paste(package_name, "::", function_names, sep = "")

    }

    if (private) {

        private_function_names <- get_private_package_function_names(package_name)

        private_function_names <- paste(package_name, ":::", private_function_names, sep= "")

        function_names <- c(function_names, private_function_names)

    }

    function_names
}

filter_closure_bindings <- function(binding_names, environment) {

    closure_bindings <- c()

    for (binding_name in binding_names) {

        binding_value <- get(binding_name, envir=environment)

        if (is_closure(binding_value)) {
            closure_bindings <- c(closure_bindings, binding_name)
        }
    }

    closure_bindings
}

with_package_namespace <- function(package_name, fun) {
    package_names <- loadedNamespaces()
    package_is_loaded <- package_name %in% package_names

    ns <- NULL

    if (!package_is_loaded) {
        ns <- loadNamespace(package_name)
    }
    else {
        ns <- getNamespace(package_name)
    }

    result <- fun(ns)

    if (!package_is_loaded) {
        unloadNamespace(package_name)
    }

    result
}

get_all_package_function_names <- function(package_name) {
    with_package_namespace(
        package_name,
        function(package_namespace) {

            all_function_names <- ls(package_namespace, all.names = TRUE)

            all_function_names <- filter_closure_bindings(all_function_names, package_namespace)

            all_function_names
        }
    )
}

get_public_package_function_names <- function(package_name) {  # nolint
    with_package_namespace(
        package_name,
        function(package_namespace) {

            all_function_names <- get_all_package_function_names(package_name)

            inner_package_namespace <- package_namespace$.__NAMESPACE__.

            public_function_names <- ls(inner_package_namespace$exports, all.names = TRUE)

            public_function_names <- filter_closure_bindings(public_function_names,
                                                             package_namespace)

            public_function_names <- intersect(public_function_names, all_function_names)

            public_function_names
        }
    )
}

get_private_package_function_names <- function(package_name) { # nolint
    with_package_namespace(
        package_name,
        function(package_namespace) {
            all_function_names <- get_all_package_function_names(package_name)

            public_function_names <- get_public_package_function_names(package_name)

            private_function_names <- setdiff(all_function_names, public_function_names)

            private_function_names
        }
    )
}

#' @export
get_object_details <- function(value, variable, envir = parent.frame(), peek = TRUE) {

    if(missing(variable)) {
        variable <- NULL
    }
    else {
        stopifnot(is_scalar_character(variable))

        if(!exists(variable, envir)) {
            stop(cat("variable", variable, "does not exist in environment"))
        }
    }

    if(missing(value)) {
        value <- NULL
    }

    .Call(C_instrumentr_get_object_details, value, variable, envir, peek)
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
