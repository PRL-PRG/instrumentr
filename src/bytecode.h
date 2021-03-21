#ifndef INSTRUMENTR_BYTECODE_H
#define INSTRUMENTR_BYTECODE_H

#include <instrumentr/api.h>
#include "model.h"

#ifdef __cplusplus
extern "C" {
#endif

/********************************************************************************
 * create
 *******************************************************************************/

instrumentr_bytecode_t instrumentr_bytecode_create(instrumentr_state_t state,
                                                   SEXP r_bytecode);

/********************************************************************************
 * interop
 *******************************************************************************/

INSTRUMENTR_MODEL_INTEROP_DECLARE_API(bytecode, INSTRUMENTR_MODEL_TYPE_BYTECODE)

INSTRUMENTR_BYTECODE_API_MAP(INSTRUMENTR_API_INCLUDER)

#ifdef __cplusplus
}
#endif

#endif /* INSTRUMENTR_BYTECODE_H */
