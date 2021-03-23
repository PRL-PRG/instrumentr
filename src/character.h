#ifndef INSTRUMENTR_CHARACTER_H
#define INSTRUMENTR_CHARACTER_H

#include <instrumentr/api.h>
#include "value.h"

#ifdef __cplusplus
extern "C" {
#endif

/********************************************************************************
 * create
 *******************************************************************************/

instrumentr_character_t instrumentr_character_create(instrumentr_state_t state,
                                                     SEXP r_character);

/********************************************************************************
 * interop
 *******************************************************************************/

INSTRUMENTR_VALUE_DECLARE_API(INSTRUMENTR_VALUE_TYPE_CHARACTER,
                              character,
                              character)

INSTRUMENTR_CHARACTER_API_MAP(INSTRUMENTR_API_INCLUDER)

#ifdef __cplusplus
}
#endif

#endif /* INSTRUMENTR_CHARACTER_H */
