
.onLoad <- function(libname, pkgname) { # nolint

    .Call(C_instrumentr_initialize_instrumentr, parent.env(environment()), .state_env, undefined_object)

}
