
.state_env$sys_call_base_index <- 0

dot_callback_names <- c("C_lightr_trace_application_load",
                        "C_lightr_trace_application_unload",
                        "C_lightr_trace_application_attach",
                        "C_lightr_trace_application_detach",
                        "C_lightr_trace_package_load",
                        "C_lightr_trace_package_unload",
                        "C_lightr_trace_package_attach",
                        "C_lightr_trace_package_detach",
                        "C_lightr_trace_function_attach",
                        "C_lightr_trace_function_detach",
                        "C_lightr_trace_call_entry",
                        "C_lightr_trace_call_exit")

r_callback_names <- c("application_load_callback",
                      "application_unload_callback",
                      "application_attach_callback",
                      "application_detach_callback",
                      "package_load_callback",
                      "package_unload_callback",
                      "package_attach_callback",
                      "package_detach_callback",
                      "function_attach_callback",
                      "function_detach_callback",
                      "call_entry_callback",
                      "call_exit_callback")

set_sys_call_base_index <- function(index) {
    assign("sys_call_base_index", index, envir=.state_env)
}

get_sys_call_base_index <- function() {
    .state_env$sys_call_base_index
}

#' @export
get_sys_calls <- function() {

    execution_context <- peek_execution_context()
    calls <- sys.calls()
    base_index <- get_sys_call_base_index()
    maximum_index <- length(calls)
    lower_index <- base_index + 1
    higher_index <- lower_index

    r_callback_index <- 0
    dot_callback_index <- 0
    get_sys_calls_index <- 0

    for (index in lower_index:maximum_index) {
        call <- calls[[index]]
        function_name <- call[[1]]

        if (typeof(function_name) != "symbol") {
            next
        }

        function_name <- as.character(function_name)

        if (r_callback_index == 0) {
            if (function_name %in% r_callback_names) {
                r_callback_index <- index
                next
            }
        }

        if (dot_callback_index == 0) {
            if (function_name == ".Call") {
                argument_function_name <- call[[2]]
                if (typeof(argument_function_name) == "symbol") {
                    argument_function_name <- as.character(argument_function_name)
                    if (argument_function_name %in% dot_callback_names) {
                        dot_callback_index <- index
                        next
                    }
                }
            }
        }

        if (get_sys_calls_index == 0) {
            if (function_name == "get_sys_calls") {
                get_sys_calls_index <- index
                next
            }
        }
    }

    higher_index <- c(r_callback_index, dot_callback_index, get_sys_calls_index)
    higher_index <- min(higher_index[which(higher_index != 0)])

    if (length(higher_index) == 0) {
        higher_index <- maximum_index
    }
    else if (higher_index == lower_index) {
        higher_index <- lower_index - 1
    }
    else {
        higher_index <- higher_index - 1
    }

    print(lower_index)
    print(higher_index)
    print(base_index)

    if (higher_index < lower_index) {
        list()
    }
    else {
        calls[lower_index:higher_index]
    }
}
