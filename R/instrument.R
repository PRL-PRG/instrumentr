insert_instrumentation <- function(tracer_ptr, application_ptr) {
    .Call(C_instrumentr_tracer_disable, tracer_ptr)

    handle_current_packages(tracer_ptr, application_ptr)

    handle_future_packages(tracer_ptr, application_ptr)

    .Call(C_instrumentr_tracer_enable, tracer_ptr)
}

handle_current_packages <- function(tracer_ptr, application_ptr) {
    ## get all names which start with "package:"
    ## reverse the list to store it in correct order in the application model
    attached_packages <- Filter(function(x) startsWith(x, "package:"), rev(search()))

    ## remove "package:" prefix from names to get the actual package/namespace name
    attached_packages <- unlist(
        unname(
            Map(function(x) substring(x, nchar("package:") + 1), attached_packages)
        )
    )

    ## get names of packages that are loaded but not attached
    loaded_packages <- setdiff(loadedNamespaces(), attached_packages)


    ## NOTE: processing attached_packages in order is important since
    ##       function lookup happens in that order
    package_ptrs <- Map(create_package, c(attached_packages, loaded_packages))

    Map(function(package_ptr) add_package(application_ptr, package_ptr), package_ptrs)

    invisible(NULL)
}

handle_future_packages <- function(tracer_ptr, application_ptr) {

    handle_package_on_load <- function(package_name, ...) {
        .Call(C_instrumentr_tracer_disable, tracer_ptr)

        package_ptr <- create_package(package_name)
        .Call(C_instrumentr_trace_package_load, tracer_ptr, application_ptr, package_ptr)

        .Call(C_instrumentr_tracer_enable, tracer_ptr)
    }

    handle_package_attach <- function(package_name, ...) {
        .Call(C_instrumentr_tracer_disable, tracer_ptr)

        package_ptr <- get_package(application_ptr, package_name)
        .Call(C_instrumentr_trace_package_attach, tracer_ptr, application_ptr, package_ptr)

        .Call(C_instrumentr_tracer_enable, tracer_ptr)
    }

    handle_package_detach <- function(package_name, ...) {
        .Call(C_instrumentr_tracer_disable, tracer_ptr)

        package_ptr <- get_package(application_ptr, package_name)
        .Call(C_instrumentr_trace_package_detach, tracer_ptr, application_ptr, package_ptr)

        .Call(C_instrumentr_tracer_enable, tracer_ptr)
    }

    handle_package_on_unload <- function(package_name, ...) {
        .Call(C_instrumentr_tracer_disable, tracer_ptr)

        package_ptr <- get_package(application_ptr, package_name)
        .Call(C_instrumentr_trace_package_unload, tracer_ptr, application_ptr, package_ptr)
        .Call(C_instrumentr_application_remove_package, application_ptr, package_ptr)

        .Call(C_instrumentr_tracer_enable, tracer_ptr)
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
