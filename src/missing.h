#ifndef INSTRUMENTR_MISSING_H
#define INSTRUMENTR_MISSING_H

#include <instrumentr/api.h>
#include "model.h"

#ifdef __cplusplus
extern "C" {
#endif

    /********************************************************************************
     * create
     *******************************************************************************/

    instrumentr_missing_t instrumentr_missing_create(instrumentr_state_t state,
                                               SEXP r_missing);

    /********************************************************************************
     * interop
     *******************************************************************************/

    INSTRUMENTR_MODEL_INTEROP_DECLARE_API(missing, INSTRUMENTR_MODEL_TYPE_MISSING)

    INSTRUMENTR_MISSING_API_MAP(INSTRUMENTR_API_INCLUDER)

#ifdef __cplusplus
}
#endif

#endif /* INSTRUMENTR_MISSING_H */
