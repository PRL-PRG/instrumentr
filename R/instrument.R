insert_package_hooks <- function(tracer_ptr, application_ptr, state_ptr) {
    .Call(C_instrumentr_tracer_disable, tracer_ptr)

    handle_current_packages(tracer_ptr, state_ptr)

    handle_future_packages(tracer_ptr, application_ptr, state_ptr)

    .Call(C_instrumentr_tracer_enable, tracer_ptr)
}

handle_current_packages <- function(tracer_ptr, state_ptr) {
    ## get all names which start with "package:"
    ## reverse the list to store it in correct order in the state model
    attached_packages <- Filter(function(x) startsWith(x, "package:"), rev(search()))

    ## remove "package:" prefix from names to get the actual package/namespace name
    attached_packages <- unlist(
        unname(
            Map(function(x) substring(x, nchar("package:") + 1), attached_packages)
        )
    )

    create_and_add <- function(package_name, attached) {
        package_ptr <- create_package(state_ptr,
                                      package_name,
                                      attached = attached)
        add_package(state_ptr, package_ptr)
    }

    Map(create_and_add, attached_packages, TRUE)

    ## get names of packages that are loaded but not attached
    loaded_packages <- setdiff(loadedNamespaces(), attached_packages)

    Map(create_and_add, loaded_packages, FALSE)

    invisible(NULL)
}

#' @importFrom utils installed.packages
handle_future_packages <- function(tracer_ptr, application_ptr, state_ptr) {

    handle_package_on_load <- function(package_name, ...) {
        #.Call(C_instrumentr_tracer_disable, tracer_ptr)

        package_ptr <- create_package(state_ptr,
                                      package_name,
                                      attached = FALSE)
        .Call(C_instrumentr_trace_package_load, tracer_ptr, application_ptr, package_ptr)

        #.Call(C_instrumentr_tracer_enable, tracer_ptr)
    }

    handle_package_attach <- function(package_name, ...) {
        #.Call(C_instrumentr_tracer_disable, tracer_ptr)

        package_ptr <- get_package(state_ptr, package_name)
        .Call(C_instrumentr_trace_package_attach, tracer_ptr, application_ptr, package_ptr)

        #.Call(C_instrumentr_tracer_enable, tracer_ptr)
    }

    handle_package_detach <- function(package_name, ...) {
        #.Call(C_instrumentr_tracer_disable, tracer_ptr)

        package_ptr <- get_package(state_ptr, package_name)
        .Call(C_instrumentr_trace_package_detach, tracer_ptr, application_ptr, package_ptr)

        #.Call(C_instrumentr_tracer_enable, tracer_ptr)
    }

    handle_package_on_unload <- function(package_name, ...) {
        #.Call(C_instrumentr_tracer_disable, tracer_ptr)

        package_ptr <- get_package(state_ptr, package_name)
        .Call(C_instrumentr_trace_package_unload, tracer_ptr, application_ptr, package_ptr)

        #.Call(C_instrumentr_tracer_enable, tracer_ptr)
    }

    future_packages <- setdiff(unname(installed.packages()[,1]), loadedNamespaces())

    ## attach event hooks for packages not loaded
    for (package in future_packages) {
        setHook(packageEvent(package, "onLoad"), handle_package_on_load, action = "prepend")
        setHook(packageEvent(package, "attach"), handle_package_attach, action = "prepend")
        setHook(packageEvent(package, "detach"), handle_package_detach, action = "prepend")
        setHook(packageEvent(package, "onUnload"), handle_package_on_unload, action = "prepend")
    }

    invisible(NULL)
}

#' @importFrom utils installed.packages
remove_package_hooks <- function(tracer_ptr, state_ptr) {
    packages <- unname(installed.packages()[,1])

    ## attach event hooks for packages not loaded
    for (package in packages) {
        setHook(packageEvent(package, "onLoad"), NULL, action = "replace")
        setHook(packageEvent(package, "attach"), NULL, action = "replace")
        setHook(packageEvent(package, "detach"), NULL, action = "replace")
        setHook(packageEvent(package, "onUnload"), NULL, action = "replace")
    }
}
