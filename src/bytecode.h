#ifndef INSTRUMENTR_BYTECODE_H
#define INSTRUMENTR_BYTECODE_H

#include <instrumentr/api.h>
#include "value.h"

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

INSTRUMENTR_VALUE_DECLARE_API(INSTRUMENTR_VALUE_TYPE_BYTECODE,
                              bytecode,
                              bytecode)

INSTRUMENTR_BYTECODE_API_MAP(INSTRUMENTR_API_INCLUDER)

#ifdef __cplusplus
}
#endif

#endif /* INSTRUMENTR_BYTECODE_H */
