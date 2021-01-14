#ifndef INSTRUMENTR_FUNTAB_H
#define INSTRUMENTR_FUNTAB_H

#include <instrumentr/types.h>

#include <instrumentr/Rincludes.h>


void funtab_initialize();
void funtab_finalize();
instrumentr_function_t instrumentr_funtab_get_function(SEXP op);

#endif /* INSTRUMENTR_FUNTAB_H */
