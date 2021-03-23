#ifndef INSTRUMENTR_EXPRESSION_H
#define INSTRUMENTR_EXPRESSION_H

#include <instrumentr/api.h>
#include "value.h"

#ifdef __cplusplus
extern "C" {
#endif

/********************************************************************************
 * create
 *******************************************************************************/

instrumentr_expression_t
instrumentr_expression_create(instrumentr_state_t state, SEXP r_expression);

/********************************************************************************
 * interop
 *******************************************************************************/

INSTRUMENTR_VALUE_DECLARE_API(INSTRUMENTR_VALUE_TYPE_EXPRESSION,
                              expression,
                              expression)

INSTRUMENTR_EXPRESSION_API_MAP(INSTRUMENTR_API_INCLUDER)

#ifdef __cplusplus
}
#endif

#endif /* INSTRUMENTR_EXPRESSION_H */
