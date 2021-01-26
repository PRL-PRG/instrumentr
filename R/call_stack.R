
#' @export
print.instrumentr_call_stack <- function(x, ...) {
    cat(to_string(x), "\n")
}

#' @export
get_size.instrumentr_call_stack <- function(call_stack, ...) { # nolint
    .Call(C_instrumentr_call_stack_get_size, call_stack)
}

#' @export
peek.instrumentr_call_stack <- function(call_stack, position, ...) { # nolint
    .Call(C_instrumentr_call_stack_peek, call_stack, position)
}

#' @export
to_string.instrumentr_call_stack <- function(object, ...) { # nolint
    size <- get_size(object)

    representation <- sprintf("CallStack(%d frames)", size)

    if (size != 0) {
        frames <- character(0)

        for (index in 1:size) {
            call_object <- peek(object, index)
            frames <- c(frames, to_string(call_object))
        }

        ## NOTE: "├── " is written using unicode sequence
        ## to avoid R CMD check warnings
        prefix <- "\u251C\u2500\u2500 "

        ## NOTE: "█" is written using unicode sequence
        ## to avoid R CMD check warnings
        suffix <- "\u2588"

        frame_representation <- paste(paste(prefix, frames, collapse = "\n"), suffix, sep = "\n")
        representation <- paste(representation, frame_representation, sep = "\n")
    }

    representation
}


