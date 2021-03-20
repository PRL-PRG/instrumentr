
compute_hash.character <- function(input, collapse = FALSE, algorithm = 'sha512') {
    stopifnot(is_scalar_logical(collapse))
    .Call(C_instrumentr_hash_string, input, collapse)
}

encode.character <- function(input, collapse = FALSE, algorithm = 'base64') {
    .Call(C_instrumentr_encode_string, input, collapse)
}

decode.character <- function(input, collapse = FALSE, algorithm = 'base64') {
    .Call(C_instrumentr_decode_string, input, collapse)
}
