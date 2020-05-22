
.onAttach <- function(libname, pkgname) {
    assign(".lightr_disable_interception", lightr:::C_lightr_disable_interception, envir = baseenv())
    assign(".lightr_enable_interception", lightr:::C_lightr_enable_interception, envir = baseenv())
    assign(".lightr_interception_is_enabled", lightr:::C_lightr_interception_is_enabled, envir = baseenv())
    assign(".lightr_intercept_call_entry", lightr:::C_lightr_intercept_call_entry, envir = baseenv())
    assign(".lightr_intercept_call_exit", lightr:::C_lightr_intercept_call_exit, envir = baseenv())
    assign(".lightr_no_retval_marker", lightr:::.no_retval_marker, envir = baseenv())

    .Call(.lightr_disable_interception)


    ##set_severity()
    ##set_autoinject()

    handle_package <- function(package_name, ...) {
        ##autoinject_packages <- get_autoinject()
        ##if (package_name %in% autoinject_packages || any(autoinject_packages
        ##== "all")) {

        intercepted_funs <- 0

        tryCatch({

            package <- intercept_package(package_name)

            application <- get_application()

            add_package(application, package)

            intercepted_funs <- length(get_functions(package))

        }, error = function(e) {

            packageStartupMessage(e$message)

        })

        packageStartupMessage("Intercepting ", intercepted_funs, " functions from ", package_name)

        ##}
    }

    remove_packages <- c(".GlobalEnv", "Autoloads", "tools:callr", "tools:rstudio", "lightr")
    loaded_packages <- setdiff(remove_package_prefix(search()), remove_packages)

    ## TODO: find alternative to installed.packages
    installed_packages <- get("installed.packages", envir=getNamespace("utils"))()[, 1]
    remaining_packages <- setdiff(installed_packages, c(loaded_packages, remove_packages))

    for (package in loaded_packages) {
        handle_package(package)
    }

    for (package in remaining_packages) {
        setHook(packageEvent(package, "attach"), handle_package)
    }

    .Call(.lightr_enable_interception)
}

.onDetach <- function(libpath) {
    ## TODO: remove interceptions
}
