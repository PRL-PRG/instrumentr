is_scalar_character <- function(vector) {
    is.character(vector) && (length(vector) == 1) && (nchar(vector) != 0)
}

is_vector_character <- function(vector) {
    is.character(vector)
}

is_scalar_logical <- function(vector) {
    is.logical(vector) && (length(vector) == 1)
}

is_environment <- function(env) {
    is.environment(env)
}

is_function <- function(fun) {
    is.function(fun)
}

is_closure <- function(obj) {
    typeof(obj) == "closure"
}

get_package_name <- function(fun) {
    name <- environmentName(environment(fun))
    if (name == "R_GlobalEnv") ".GlobalEnv"
    else name
}

remove_package_prefix <- function(package_names) {
    prefix <- "package:"
    ifelse(startsWith(package_names, prefix),
           substring(package_names, nchar(prefix) + 1),
           package_names)
}


add_package_prefix <- function(package_names) {
    prefix <- "package:"
    paste0(prefix, package_names)
}

