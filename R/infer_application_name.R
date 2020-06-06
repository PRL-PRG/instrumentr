
infer_application_name <- function() {
    application_name <- NULL

    ## this works if script is sourced inside R session
    tryCatch({
        application_name <- sys.frame(1)$ofile
        if (!is.null(application_name)) {
            application_name <- substr(application_name,
                                       1,
                                       ## ignore `.R` suffix
                                       nchar(application_name) - 2)
        }
    }, error = function(e) {
    })

    if (is.null(application_name)) {
        ## this works if script is started from command line as
        ## R --file=<application_name>.R
        args <- commandArgs(trailingOnly = FALSE)
        for (arg in args) {
            if (startsWith(arg, "--file=")) {
                application_name <- substr(arg,
                                           ## ignore `--file=` prefix
                                           nchar("--file=") + 1,
                                           ## ignore `.R` suffix
                                           nchar(arg) - 2)
                break
            }
        }
    }

    if (is.null(application_name)) {
        application_name <- "<unknown>"
    }

    application_name
}
