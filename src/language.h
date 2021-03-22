#ifndef INSTRUMENTR_LANGUAGE_H
#define INSTRUMENTR_LANGUAGE_H

#include <instrumentr/api.h>
#include "model.h"

#ifdef __cplusplus
extern "C" {
#endif

/********************************************************************************
 * create
 *******************************************************************************/

instrumentr_language_t instrumentr_language_create(instrumentr_state_t state,
                                                   SEXP r_language);

/********************************************************************************
 * interop
 *******************************************************************************/

INSTRUMENTR_MODEL_INTEROP_DECLARE_API(language, INSTRUMENTR_MODEL_TYPE_LANGUAGE)

INSTRUMENTR_LANGUAGE_API_MAP(INSTRUMENTR_API_INCLUDER)

#ifdef __cplusplus
}
#endif

#endif /* INSTRUMENTR_LANGUAGE_H */