
#' @export
create_package <- function(package_name, package_directory, package_environment) {
    stopifnot(is_scalar_character(package_name))
    stopifnot(is_scalar_character(package_directory))
    stopifnot(is_environment(package_environment))

    .Call(C_package_create_package, package_name, package_directory, package_environment)
}
