
## TODO: remove state_env
.state_env <- new.env(parent=emptyenv())

.onLoad <- function(libname, pkgname) { # nolint

    .Call(C_instrumentr_initialize, parent.env(environment()), .state_env)

    version_info <- get_version_information()

    show_setup_info(version_info)

}

.onUnload <- function(libpath) { # nolint

    .Call(C_instrumentr_finalize)

}
