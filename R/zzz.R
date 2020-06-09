
.onLoad <- function(libname, pkgname) { # nolint

    .Call(C_lightr_initialize_lightr, parent.env(environment()), .state_env, undefined_object)

}
