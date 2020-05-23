
.onLoad <- function(libname, pkgname) {
    package_env <- .GlobalEnv #parent.env(environment())

    .Call(C_lightr_intercept_application_entry, package_env)

    reg.finalizer(package_env,
                  function(e) {
                      .Call(C_lightr_intercept_application_exit, e)
                  },
                  onexit = TRUE)
}


.onAttach <- function(libname, pkgname) {

    .Call(C_lightr_disable_interception)

    ##set_severity()
    ##set_autoinject()

    handle_package <- function(package_name, ...) {
        ##autoinject_packages <- get_autoinject()
        ##if (package_name %in% autoinject_packages || any(autoinject_packages
        ##== "all")) {

        tryCatch({

            package <- intercept_package(package_name)

        }, error = function(e) {

            packageStartupMessage(e$message)

        })

        ## TODO: add error
        .Call(C_lightr_intercept_package_exit, package)

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

    .Call(C_lightr_enable_interception)
}

.onDetach <- function(libpath) {
    ## TODO: remove interceptions
}
