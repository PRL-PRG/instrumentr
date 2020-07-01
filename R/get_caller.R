
#' @export
get_caller <- function(object, ...) {
    UseMethod("get_caller")
}

#' @export
get_caller.instrumentr_call <- function(object, ...) { # nolint
    frame_position <- get_frame_position(object)

    frames <- sys.frames()
    calls <- sys.calls()
    parents <- sys.parents()

    caller_frame_position <- parents[frame_position]


    if (caller_frame_position == 0) {
        stop("caller of topmost call is undefined")
    }

    caller_env <- frames[[caller_frame_position]]
    caller_expr <- calls[[caller_frame_position]]


    function_name <- ""

    if (is_language(caller_expr)) {
        function_name <- caller_expr[[1]]
    }

    if (is_symbol(function_name)) {
        function_name <- as.character(function_name)
    }
    else {
        function_name <- ""
    }

    definition <- sys.function(caller_frame_position)
    definition_depth <- 0

    if (!is_closure(definition)) {
        package_name <- "base"
    }
    else {
        env <- environment(definition)

        package_name <- environmentName(env)

        while (package_name == "") {
            definition_depth <- definition_depth + 1
            if (is.null(env)) {
                print(definition)
                print(function_name)
                stop("problem here")
            }

            env <- parent.env(env)
            package_name <- environmentName(env)
        }
    }

    list(package_name = package_name,
         function_name = function_name,
         definition = definition,
         environment = caller_env,
         definition_depth = definition_depth,
         frame_position = caller_frame_position,
         call_expression = caller_expr)
}
