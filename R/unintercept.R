
unintercept <- function() {
    print("unintercepting being")
    for(function_id in get_intercepted_function_ids()) {
        unintercept_function(get_intercepted_function(function_id))
        remove_intercepted_function(function_id)
    }
    print("unintercepting end")
}

unintercept_function <- function(intercepted_function) {
    package_name = intercepted_function$package_name
    package_env = intercepted_function$package_env
    function_name = intercepted_function$function_name
    new_function_obj = intercepted_function$function_obj
    old_function_obj = intercepted_function$old_function_obj

    unlockBinding(function_name, package_env)
    assign(function_name, old_function_obj, envir = package_env)
    lockBinding(function_name, package_env)
}
