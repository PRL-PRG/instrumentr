get_commit_hash <- function() {
    .Call(C_instrumentr_get_commit_hash)
}

#' @importFrom utils packageVersion
get_version_information <- function() {

    r_version <- R.Version()
    r_version_string <- sprintf("%s.%s (%s-%s-%s)",
                                r_version$major,
                                r_version$minor,
                                r_version$year,
                                r_version$month,
                                r_version$day)

    instrumentr_version <- packageVersion("instrumentr")

    instrumentr_hash <- get_commit_hash()

    if (instrumentr_hash != "") {
        instrumentr_version_string <- sprintf("%s (%s)",
                                              instrumentr_version,
                                              instrumentr_hash)
    }
    else {
        instrumentr_version_string <- sprintf("%s", instrumentr_version)
    }

    structure(list(r_version = r_version_string,
                   instrumentr_version = instrumentr_version_string),
              class = "version_info")

}

get_environment_variable_info <- function(envvars = c("USER",
                                                      "HOME",

                                                      "R_HOME",
                                                      "R_INCLUDE_DIR",
                                                      "R_LIBS_USER",
                                                      "R_LIBS_SITE",
                                                      "R_SESSION_TMPDIR",

                                                      "R_KEEP_PKG_SOURCE",
                                                      "R_KEEP_PKG_PARSE_DATA",
                                                      "R_ENABLE_JIT",
                                                      "R_DISABLE_BYTECODE",
                                                      "OMP_NUM_THREADS"
                                                      )
                                          ) {

    values <- as.character(unname(Map(Sys.getenv, envvars)))
    name_sizes <-as.integer(unname(Map(nchar, envvars)))
    max_offset <- max(name_sizes) + 4
    offsets <- as.character(unname(Map(function(offset) paste(replicate(offset, " "), collapse = ""), max_offset - name_sizes)))
    paste(" - ", envvars, offsets, values, collapse = "\n")
}

show_setup_info <- function(version_info) {

    BULLET_CHARACTER <- "\u2022"
    LINE_CHARACTER <- "\u2500"

    LINE_SEPARATOR <- strrep(LINE_CHARACTER, 80)

    MESSAGE_TEMPLATE <- paste(
        "%s Setup %s",
        "%s R Version                    %s",
        "%s instrumentr Version          %s",
        "%s",
        "%s Working Directory            %s",
        "%s",
        "%s Environment Variables",
        "%s",
        "%s",
        "",
        sep = "\n"
    )


    message <- sprintf(MESSAGE_TEMPLATE,
                       strrep(LINE_CHARACTER, 2), strrep(LINE_CHARACTER, 71),
                       BULLET_CHARACTER, version_info$r_version,
                       BULLET_CHARACTER, version_info$instrumentr_version,
                       LINE_SEPARATOR,
                       BULLET_CHARACTER, getwd(),
                       LINE_SEPARATOR,
                       BULLET_CHARACTER,
                       get_environment_variable_info(),
                       LINE_SEPARATOR)

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
