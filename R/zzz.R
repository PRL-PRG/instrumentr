
.onLoad <- function(libname, pkgname) { # nolint

    .Call(C_instrumentr_initialize_instrumentr, parent.env(environment()), .state_env, undefined_object)

    version_info <- get_version_information()

    show_setup_info(version_info)

}

.onUnload <- function(libpath) { # nolint

    .Call(C_instrumentr_finalize_instrumentr)

}

.onDetach <- function(libpath) { # nolint

    .Call(C_instrumentr_finalize_instrumentr)

}
