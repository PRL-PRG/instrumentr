#ifndef INSTRUMENTR_FUNTAB_H
#define INSTRUMENTR_FUNTAB_H

#include <instrumentr/types.h>

#include <instrumentr/Rincludes.h>

int instrumentr_funtab_get_index(SEXP op);
int instrumentr_funtab_get_size();
instrumentr_function_t instrumentr_funtab_create_function(int index);

#endif /* INSTRUMENTR_FUNTAB_H */
