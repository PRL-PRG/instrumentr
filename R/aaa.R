
#' @export
get_state <- function(object) {
    UseMethod("get_state")
}

#' @export
get_application <- function(object) { # nolint
    UseMethod("get_application")
}

#' @export
get_code <- function(object) {
    UseMethod("get_code")
}

#' @export
get_environment <- function(object) {
    UseMethod("get_environment")
}

#' @export
get_directory <- function(object) {
    UseMethod("get_directory")
}

#' @export
get_frame_position <- function(object) {
    UseMethod("get_frame_position")
}

#' @export
get_name <- function(object) {
    UseMethod("get_name")
}

#' @export
get_environment <- function(application, name_or_position) {
    UseMethod("get_environment")
}

#' @export
get_environments <- function(application) {
    UseMethod("get_environments")
}

#' @export
get_call_stack <- function(object) {
    UseMethod("get_call_stack")
}

create_result <- function(object, ...) {
    UseMethod("create_result")
}

#' @export
get_argument <- function(parameter, ...) {
    UseMethod("get_argument")
}

#' @export
get_arguments <- function(parameter, ...) {
    UseMethod("get_arguments")
}

#' @export
get_caller <- function(object, ...) {
    UseMethod("get_caller")
}

#' @export
get_call <- function(object, ...) {
    UseMethod("get_call")
}

#' @export
get_c_function <- function(object) {
    UseMethod("get_c_function")
}

#' @export
get_r_function <- function(object) {
    UseMethod("get_r_function")
}

#' @export
get_definition <- function(object, ...) {
    UseMethod("get_definition")
}

#' @export
get_expression <- function(object, ...) {
    UseMethod("get_expression")
}

#' @export
get_functions <- function(object) {
    UseMethod("get_functions")
}

#' @export
get_function_count <- function(object) {
    UseMethod("get_function_count")
}

#' @export
get_function <- function(object, name_or_position) {
    UseMethod("get_function")
}

#' @export
has_function <- function(object, name) {
    UseMethod("has_function")
}


#' @export
get_message <- function(object, ...) {
    UseMethod("get_message")
}

#' @export
get_namespace <- function(object) {
    UseMethod("get_namespace")
}

#' @export
get_parameter_count <- function(object, ...) {
    UseMethod("get_parameter_count")
}

#' @export
get_parameter <- function(call, ...) {
    UseMethod("get_parameter")
}

#' @export
get_parameters <- function(call, ...) {
    UseMethod("get_parameters")
}

#' @export
get_error <- function(object) {
    UseMethod("get_error")
}

#' @export
get_exec_stats <- function(object) {
    UseMethod("get_exec_stats")
}

#' @export
get_default_argument <- function(parameter, ...) {
    UseMethod("get_default_argument")
}

#' @export
get_position <- function(object, ...) {
    UseMethod("get_position")
}

#' @export
get_result <- function(object, ...) {
    UseMethod("get_result")
}

#' @export
get_size <- function(object) {
    UseMethod("get_size")
}

#' @export
get_source <- function(object, ...) {
    UseMethod("get_source")
}

#' @export
get_value <- function(object, ...) {
    UseMethod("get_value")
}

#' @export
has_c_function <- function(object) {
    UseMethod("has_c_function")
}

#' @export
has_r_function <- function(object) {
    UseMethod("has_r_function")
}

#' @export
has_result <- function(object, ...) {
    UseMethod("has_result")
}

#' @export
is_active <- function(object, ...) {
    UseMethod("is_active")
}

#' @export
is_error <- function(object) {
    UseMethod("is_error")
}

#' @export
is_evaluated <- function(argument, ...) {
    UseMethod("is_evaluated")
}

#' @export
is_missing <- function(object, ...) {
    UseMethod("is_missing")
}

#' @export
is_exported <- function(object, ...) {
    UseMethod("is_public")
}

#' @export
is_s3_method <- function(object, ...) {
    UseMethod("is_s3_method")
}

#' @export
is_s3_generic <- function(object, ...) {
    UseMethod("is_s3_generic")
}

#' @export
is_value <- function(object, ...) {
    UseMethod("is_value")
}

#' @export
is_vararg <- function(object, ...) {
    UseMethod("is_vararg")
}

#' @export
peek <- function(call_stack, position, ...) {
    UseMethod("peek")
}

#' @export
has_default_argument <- function(parameter, ...) {
    UseMethod("has_default_argument")
}

#' @export
is_attached <- function(object) {
    UseMethod("is_attached")
}

#' @export
get_sexp <- function(object) {
    UseMethod("get_sexp")
}

#' @export
get_sexp_address <- function(object) {
    UseMethod("get_sexp_address")
}

#' @export
get_sexp_type <- function(object) {
    UseMethod("get_sexp_type")
}

#' @export
is_forced <- function(object) {
    UseMethod("is_forced")
}

#' @export
get_expression <- function(object) {
    UseMethod("get_expression")
}

#' @export
get_value <- function(object) {
    UseMethod("get_value")
}

#' @export
has_name <- function(object) {
    UseMethod("has_name")
}

#' @export
is_value <- function(object) {
    UseMethod("is_value")
}

#' @export
as_value <- function(object) {
    UseMethod("as_value")
}

#' @export
is_promise <- function(object) {
    UseMethod("is_promise")
}

#' @export
as_promise <- function(object) {
    UseMethod("as_promise")
}

#' @export
create_callback <- function(object, event) {
    UseMethod("create_callback")
}

#' @export
get_callback <- function(tracer, event) {
    UseMethod("get_callback")
}

#' @export
set_callback <- function(tracer, callback) {
    UseMethod("set_callback")
}

#' @export
has_callback <- function(tracer, event) { # nolint
    UseMethod("has_callback")
}

#' @export
remove_callback <- function(tracer, event) { # nolint
    UseMethod("remove_callback")
}

#' @export
get_minimum_time <- function(exec_stats) {
    UseMethod("get_minimum_time")
}

#' @export
get_maximum_time <- function(exec_stats) {
    UseMethod("get_maximum_time")
}

#' @export
get_average_time <- function(exec_stats) {
    UseMethod("get_average_time")
}

#' @export
get_total_time <- function(exec_stats) {
    UseMethod("get_total_time")
}

#' @export
get_execution_count <- function(exec_stats) {
    UseMethod("get_execution_count")
}

#' @export
get_event <- function(object) {
    UseMethod("get_event")
}

#' @export
handles_event <- function(object, event) {
    UseMethod("handles_event")
}

#' @export
get_event_exec_stats <- function(tracer, event) {
    UseMethod("get_event_exec_stats")
}

#' @export
get_tracing_exec_stats <- function(tracer) {
    UseMethod("get_tracing_exec_stats")
}

#' @export
is_active <- function(object) {
    UseMethod("is_active")
}

#' @export
is_enabled <- function(tracer) {
    UseMethod("is_enabled")
}

#' @export
enable <- function(tracer) {
    UseMethod("enable")
}

#' @export
disable <- function(tracer) {
    UseMethod("disable")
}

#' @export
reinstate <- function(tracer) {
    UseMethod("reinstate")
}

#' @export
get_active_callback <- function(tracer) {
    UseMethod("get_active_callback")
}

#' @export
get_parameter_count <- function(object) { # nolint
    UseMethod("get_parameter_count")
}

#' @export
get_argument_count <- function(object) { # nolint
    UseMethod("get_argument_count")
}

#' @export
has_name <- function(object) { # nolint
    UseMethod("has_name")
}

#' @export
is_inner <- function(object) { # nolint
    UseMethod("is_inner")
}

#' @export
get_parent <- function(object) { # nolint
    UseMethod("get_parent")
}

#' @export
is_empty <- function(state) {
    UseMethod("is_empty")
}

#' @export
clear <- function(state) {
    UseMethod("clear")
}

#' @export
has_key <- function(state) {
    UseMethod("has_key")
}

#' @export
as_list <- function(state) {
    UseMethod("as_list")
}

#' @export
lookup <- function(state, key, alternative) {
    UseMethod("lookup")
}

#' @export
insert <- function(state, key, value, overwrite) {
    UseMethod("insert")
}

#' @export
erase <- function(state, key) {
    UseMethod("erase")
}

get_type <- function(object) {
    UseMethod("get_type")
}

is_argument <- function(object) {
    UseMethod("is_argument")
}

is_delayed_assign <- function(object) {
    UseMethod("is_delayed_assign")
}

is_lazy_load <- function(object) {
    UseMethod("is_lazy_load")
}

is_unknown <- function(object) {
    UseMethod("is_unknown")
}

get_argument <- function(object, position_or_name) {
    UseMethod("get_argument")
}

get_parameter <- function(object, position_or_name) {
    UseMethod("get_parameter")
}

get_call <- function(object) {
    UseMethod("get_call")
}

compute_hash <- function(input, collapse = FALSE, algorithm = 'sha512') {
    UseMethod("compute_hash")
}

encode <- function(input, collapse = FALSE, algorithm = 'base64') {
    UseMethod("encode")
}

decode <- function(input, collapse = FALSE, algorithm = 'base64') {
    UseMethod("decode")
}

#' @export
get_packages <- function(object, ...) {
    UseMethod("get_packages")
}

#' @export
get_namespaces <- function(object, ...) {
    UseMethod("get_namespaces")
}
