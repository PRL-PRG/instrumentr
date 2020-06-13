instrumented_packages <- new.env(parent=emptyenv())

.state_env <- new.env(parent=emptyenv())

add_instrumented_function <- function(package_name, function_name, value) {
    instrumented_functions <- get0(package_name, envir=instrumented_packages, ifnotfound=NULL)
    if (is.null(instrumented_functions)) {
        instrumented_functions <- new.env(parent=emptyenv())
        assign(package_name, instrumented_functions, envir=instrumented_packages)
    }
    assign(function_name, value, envir=instrumented_functions)
}

get_instrumented_function <- function(package_name, function_name) {
    instrumented_functions <- get0(package_name, envir=instrumented_packages, ifnotfound=NULL)

    if (is.null(instrumented_functions)) {
        NULL
    } else {
        get(function_name, envir=instrumented_functions)
    }
}

has_instrumented_function <- function(package_name, function_name) {
    instrumented_functions <- get0(package_name, envir=instrumented_packages, ifnotfound=NULL)

    if (is.null(instrumented_functions)) {
        FALSE
    } else {
        exists(function_name, envir=instrumented_functions)
    }
}

get_instrumented_package_names <- function() {
    ls(envir=instrumented_packages, all.names = TRUE)
}

get_instrumented_function_names <- function(package_name) {
    instrumented_functions <- get(package_name, envir=instrumented_packages)
    ls(envir=instrumented_functions, all.names = TRUE)
}

remove_instrumented_package <- function(package_name) {
    rm(list=package_name, envir=instrumented_packages)
}

remove_instrumented_function <- function(package_name, function_name) {
    instrumented_functions <- get(package_name, envir=instrumented_packages)
    rm(list = function_name, envir=instrumented_functions)
}
