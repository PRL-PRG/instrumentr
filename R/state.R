intercepted_packages <- new.env(parent=emptyenv())

invalid_value <- structure(new.env(parent=emptyenv()), class = c("lightr_invalid_value"))

.state_env <- new.env(parent=emptyenv())

add_intercepted_function <- function(package_name, function_name, value) {
    intercepted_functions <- get0(package_name, envir=intercepted_packages, ifnotfound=NULL)
    if (is.null(intercepted_functions)) {
        intercepted_functions <- new.env(parent=emptyenv())
        assign(package_name, intercepted_functions, envir=intercepted_packages)
    }
    assign(function_name, value, envir=intercepted_functions)
}

get_intercepted_function <- function(package_name, function_name) {
    intercepted_functions <- get0(package_name, envir=intercepted_packages, ifnotfound=NULL)

    if (is.null(intercepted_functions)) {
        NULL
    } else {
        get(function_name, envir=intercepted_functions)
    }
}

has_intercepted_function <- function(package_name, function_name) {
    intercepted_functions <- get0(package_name, envir=intercepted_packages, ifnotfound=NULL)

    if (is.null(intercepted_functions)) {
        FALSE
    } else {
        exists(function_name, envir=intercepted_functions)
    }
}

get_intercepted_package_names <- function() {
    ls(envir=intercepted_packages, all.names = TRUE)
}

get_intercepted_function_names <- function(package_name) {
    intercepted_functions <- get(package_name, envir=intercepted_packages)
    ls(envir=intercepted_functions, all.names = TRUE)
}

remove_intercepted_package <- function(package_name) {
    rm(list=package_name, envir=intercepted_packages)
}

remove_intercepted_function <- function(package_name, function_name) {
    intercepted_functions <- get(package_name, envir=intercepted_packages)
    rm(list = function_name, envir=intercepted_functions)
}
