
.onLoad <- function(libname, pkgname) {

    .Call(C_lightr_initialize_lightr, parent.env(environment()), .state_env)

}
