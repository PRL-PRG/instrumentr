
get_commit_hash <- function() {
    .Call(C_instrumentr_get_commit_hash)
}

#' @importFrom utils packageVersion
show_version_information <- function() {

    bullet <- "\u2022"
    line <- "\u2500"
    r_major_version <- R.Version()$major
    r_minor_version <- R.Version()$minor
    instrumentr_version <- packageVersion("instrumentr")
    instrumentr_hash <- get_commit_hash()
    injectr_version <- packageVersion("injectr")

    message_template <- paste(
        "%s Environment %s",
        "%s R           %s.%s",
        "%s instrumentr %s (%s)",
        "%s injectr     %s",
        "%s",
        "\n",
        sep = "\n"
    )

    message <- sprintf(message_template,
                       strrep(line, 2), strrep(line, 65),
                       bullet, r_major_version, r_minor_version,
                       bullet, instrumentr_version, instrumentr_hash,
                       bullet, injectr_version,
                       strrep(line, 80))

    packageStartupMessage(message)
}
