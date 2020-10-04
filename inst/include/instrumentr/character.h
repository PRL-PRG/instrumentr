#ifndef INSTRUMENTR_STRING_H
#define INSTRUMENTR_STRING_H

#include <instrumentr/Rincludes.h>

char* instrumentr_character_duplicate(const char* original);

SEXP instrumentr_character_wrap(const char* string);

const char* instrumentr_character_unwrap(SEXP r_character, int index);

#endif /* INSTRUMENTR_STRING_H */
