
create_error <- function(source, message, call) {
    structure(list(source = source, message = message, call = call),
              class = c("lightr_error", "simpleError", "error", "condition"))
}
