
#' @export
get_installed_package_names <- function(exclude = "lightr") {

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

    if(public) {

        function_names <- get_public_package_function_names(package_name)

        function_names <- paste(package_name, "::", function_names)

    }

    if(private) {

        private_function_names <- get_private_package_function_names(package_name)

        private_function_names <- paste(package_name, ":::", private_function_names)

        function_names <- c(function_names, private_function_names)

    }

    function_names
}

filter_closure_bindings <- function(binding_names, environment) {

    closure_bindings <- c()

    for(binding_name in binding_names) {

        binding_value <- get(binding_name, envir=environment)

        if(is_closure(binding_value)) {
            closure_bindings <- c(closure_bindings, binding_name)
        }
    }

    closure_bindings
}

with_package_namespace <- function(package_name, fun) {
    package_names <- loadedNamespaces()
    package_is_loaded <- package_name %in% package_names

    ns <- NULL

    if(!package_is_loaded) {
        ns <- loadNamespace(package_name)
    }
    else {
        ns <- getNamespace(package_name)
    }

    result <- fun(ns)

    if(!package_is_loaded) {
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

get_public_package_function_names <- function(package_name) {
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

get_private_package_function_names <- function(package_name) {
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
