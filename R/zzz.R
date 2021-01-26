
show_setup_info <- function(version_info) {

    bullet <- "\u2022"
    line <- "\u2500"

    message_template <- paste(
        "%s Setup %s",
        "%s R           %s",
        "%s instrumentr %s",
        "%s injectr     %s",
        "%s",
        "",
        sep = "\n"
    )

    message <- sprintf(message_template,
                       strrep(line, 2), strrep(line, 71),
                       bullet, version_info$r_version,
                       bullet, version_info$instrumentr_version,
                       bullet, version_info$injectr_version,
                       strrep(line, 80))

    packageStartupMessage(message)
}

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
