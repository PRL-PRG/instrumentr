#include <instrumentr/string.h>
#include <instrumentr/log.h>
#include <string.h>
#include <stdlib.h>

char* instrumentr_duplicate_string(const char* original) {
    if (original == NULL) {
        return NULL;
    }
    int length = strlen(original);
    char* duplicate = (char*) malloc(length * sizeof(char));
    strcpy(duplicate, original);
    return duplicate;
}

SEXP instrumentr_c_string_wrap(const char* string) {
    if (string == NULL) {
        SEXP r_character = PROTECT(allocVector(STRSXP, 1));
        SET_STRING_ELT(r_character, 0, NA_STRING);
        UNPROTECT(1);
        return r_character;
    } else {
        return mkString(string);
    }
}

const char* instrumentr_c_string_unwrap(SEXP r_character, int index) {
    if (TYPEOF(r_character) == STRSXP) {
        int length = Rf_length(r_character);
        if (index < length) {
            SEXP r_string = STRING_ELT(r_character, index);
            return r_string == NA_STRING ? NULL : CHAR(r_string);
        } else {
            instrumentr_raise_error("cannot unwrap C string from position %d "
                                    "of a %d length R character vector",
                                    index,
                                    length);
        }
    } else {
        instrumentr_raise_error(
            "cannot unwrap C string from an R object of type %s",
            type2char(TYPEOF(r_character)));
    }
}
