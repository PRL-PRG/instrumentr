
#' @export
create_package <- function(package_name, package_directory, package_environment) {
    is_scalar_character(package_name)
    is_scalar_character(package_directory)
    is_environment(package_environment)
    .Call(C_package_create, package_name, package_directory, package_environment)
}
