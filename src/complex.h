#ifndef INSTRUMENTR_COMPLEX_H
#define INSTRUMENTR_COMPLEX_H

#include <instrumentr/api.h>
#include "model.h"

#ifdef __cplusplus
extern "C" {
#endif

/********************************************************************************
 * create
 *******************************************************************************/

instrumentr_complex_t instrumentr_complex_create(instrumentr_state_t state,
                                                 SEXP r_complex);

/********************************************************************************
 * interop
 *******************************************************************************/

INSTRUMENTR_MODEL_INTEROP_DECLARE_API(complex, INSTRUMENTR_MODEL_TYPE_COMPLEX)

INSTRUMENTR_COMPLEX_API_MAP(INSTRUMENTR_API_INCLUDER)

#ifdef __cplusplus
}
#endif

#endif /* INSTRUMENTR_COMPLEX_H */