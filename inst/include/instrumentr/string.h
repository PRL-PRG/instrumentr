#ifndef INSTRUMENTR_STRING_H
#define INSTRUMENTR_STRING_H

#include <instrumentr/Rincludes.h>

char* instrumentr_duplicate_string(const char* string);

SEXP instrumentr_c_string_wrap(const char* string);

const char* instrumentr_c_string_unwrap(SEXP r_character, int index);

#endif /* INSTRUMENTR_STRING_H */
