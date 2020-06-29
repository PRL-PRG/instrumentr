
#' @export
create_application <- function(name, directory, code, environment, frame_position) {
    stopifnot(is_scalar_character(name))
    stopifnot(is_scalar_character(directory))
    stopifnot(is_language(code))
    stopifnot(is_environment(environment))
    stopifnot(is_scalar_integer(frame_position))

    .Call(C_application_create_application, name, directory, code, environment, frame_position)
}
