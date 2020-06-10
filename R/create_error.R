
create_error <- function(source, message, call) {
    structure(list(source = source, message = message, call = call),
              class = c("instrumentr_error", "simpleError", "error", "condition"))
}
