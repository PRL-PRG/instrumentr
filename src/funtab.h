#ifndef INSTRUMENTR_FUNTAB_H
#define INSTRUMENTR_FUNTAB_H

#include <instrumentr/Rincludes.h>
#include <instrumentr/types.h>

#ifdef __cplusplus
extern "C" {
#endif

int instrumentr_funtab_get_index(SEXP op);
CCODE instrumentr_funtab_get_definition(int index);
const char* instrumentr_funtab_get_name(int index);
int instrumentr_funtab_get_parameter_count(int index);
int instrumentr_funtab_is_internal(int index);
int instrumentr_funtab_is_primitive(int index);
int instrumentr_funtab_is_builtin(int index);
int instrumentr_funtab_is_special(int index);

#ifdef __cplusplus
}
#endif

#endif /* INSTRUMENTR_FUNTAB_H */
