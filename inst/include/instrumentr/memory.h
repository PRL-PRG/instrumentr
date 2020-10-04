#ifndef INSTRUMENTR_MEMORY_H
#define INSTRUMENTR_MEMORY_H

#include <instrumentr/Rincludes.h>

void instrumentr_acquire_sexp(SEXP r_object);
void instrumentr_release_sexp(SEXP r_object);

#endif /* INSTRUMENTR_MEMORY_H */
