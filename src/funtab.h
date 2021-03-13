#ifndef INSTRUMENTR_FUNTAB_H
#define INSTRUMENTR_FUNTAB_H

#ifdef __cplusplus
extern "C" {
#endif

#include <instrumentr/types.h>
#include <instrumentr/Rincludes.h>

int instrumentr_funtab_get_index(SEXP op);
int instrumentr_funtab_get_size();
instrumentr_function_t instrumentr_funtab_create_function(instrumentr_state_t state,
                                                          int index);

#ifdef __cplusplus
}
#endif

#endif /* INSTRUMENTR_FUNTAB_H */
