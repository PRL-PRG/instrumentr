
#' @export
create_application <- function(application_name, application_directory, application_environment) {
    stopifnot(is_scalar_character(application_name))
    stopifnot(is_scalar_character(application_directory))
    stopifnot(is_environment(application_environment))

    .Call(C_application_create, application_name, application_directory, application_environment)
}
