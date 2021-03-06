
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
