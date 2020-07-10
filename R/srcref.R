
attach_base_srcref <- function() {
    version <- R.Version()

    version_string <- paste(version$major, version$minor, sep = ".")

    base_directory <- system.file("srcref", "base", version_string, package = "instrumentr")

    ## NOTE: if base source code is not present for the R version in question
    ##       then show an error message.
    if (base_directory == "") {
        stop(sprintf("base srcrefs not supported for %s", R.version.string))
    }

    filepaths <- list.files(base_directory, recursive = TRUE, full.names = TRUE)

    base <- baseenv()

    base_bindings <- ls(base)

    time <- system.time({
        counters <- vapply(filepaths, attach_base_function_srcref, 1, base, base_bindings)
    })

    structure(list(functions = sum(counters), time = unname(time["user.self"])),
              class = "srcref_info")
}


attach_base_function_srcref <- function(filepath, base, base_bindings) {
    env <- new.env(parent = base)

    sys.source(filepath,
               envir = env,
               keep.source = TRUE,
               keep.parse.data = TRUE)

    counter <- 0

    for (name in ls(env)) {
        dummy_value <- get(name, envir = env)

        if (is.function(dummy_value) && name %in% base_bindings) {
            srcref <- attr(dummy_value, "srcref")

            original_value <- get(name, envir = base)

            attr(original_value, "srcref") <- srcref

            ## NOTE: because of copy-on-write, attaching srcref to
            ##       original_value copies it locally. The corresponding
            ##       base package function is updated with this copy.
            locked <- bindingIsLocked(name, base)
            if(locked) unlockBinding(name, base)
            assign(name, original_value, envir = base)
            if(locked) lockBinding(name, base)

            counter <- counter + 1
        }
    }

    counter
}
