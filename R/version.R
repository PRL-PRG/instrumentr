
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

    injectr_version <- packageVersion("injectr")

    structure(list(r_version = r_version_string,
                   instrumentr_version = instrumentr_version_string,
                   injectr_version = injectr_version),
              class = "version_info")
}
