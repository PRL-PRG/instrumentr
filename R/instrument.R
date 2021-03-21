#' @importFrom utils installed.packages
insert_package_hooks <- function(tracer_ptr) {

    handle_package_on_load <- function(package_name, ...) {
        .Call(r_instrumentr_trace_package_load, tracer_ptr, package_name)
    }

    handle_package_attach <- function(package_name, ...) {
        .Call(r_instrumentr_trace_package_attach, tracer_ptr, package_name)
    }

    handle_package_detach <- function(package_name, ...) {
        .Call(r_instrumentr_trace_package_detach, tracer_ptr, package_name)
    }

    handle_package_on_unload <- function(package_name, ...) {
        .Call(r_instrumentr_trace_package_unload, tracer_ptr, package_name)
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
remove_package_hooks <- function() {
    packages <- unname(installed.packages()[,1])

    ## attach event hooks for packages not loaded
    for (package in packages) {
        setHook(packageEvent(package, "onLoad"), NULL, action = "replace")
        setHook(packageEvent(package, "attach"), NULL, action = "replace")
        setHook(packageEvent(package, "detach"), NULL, action = "replace")
        setHook(packageEvent(package, "onUnload"), NULL, action = "replace")
    }
}
