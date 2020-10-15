
#' @export
create_application <- function(name, directory, code, environment, frame_position) {
    stopifnot(is_scalar_character(name))
    stopifnot(is_scalar_character(directory))
    stopifnot(is_language(code))
    stopifnot(is_environment(environment))
    stopifnot(is_scalar_integer(frame_position))

    .Call(C_instrumentr_application_create, name, directory, code, environment, frame_position)
}
