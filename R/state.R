.intercepted_functions <- new.env(parent=emptyenv())

.no_retval_marker <- new.env(parent=emptyenv())

.state_env <- new.env(parent=emptyenv())

add_intercepted_function <- function(key, value) {
    assign(key, value, envir=.intercepted_functions)
}

get_intercepted_function <- function(key) {
    get(key, envir=.intercepted_functions)
}

has_intercepted_function <- function(key) {
    exists(key, envir=.intercepted_functions)
}

get_intercepted_function_ids <- function() {
    ls(envir=.intercepted_functions, all.names = TRUE)
}

remove_intercepted_function <- function(key) {
    rm(list = key, envir=.intercepted_functions)
}

get_intercepted_function_count <- function() {
    length(get_intercepted_function_ids())
}
