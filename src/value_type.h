#ifndef INSTRUMENTR_VALUE_TYPE_H
#define INSTRUMENTR_VALUE_TYPE_H

#include <instrumentr/Rincludes.h>
#include <instrumentr/api.h>

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************************************
 * initialize
 *******************************************************************************/

void instrumentr_value_type_initialize();

/*******************************************************************************
 * finalize
 *******************************************************************************/

void instrumentr_value_type_finalize();

INSTRUMENTR_VALUE_TYPE_API_MAP(INSTRUMENTR_API_INCLUDER)

/*******************************************************************************
 * class
 *******************************************************************************/

SEXP instrumentr_value_type_get_class(instrumentr_value_type_t value_type);

#ifdef __cplusplus
}
#endif

#endif /* INSTRUMENTR_VALUE_TYPE_H */
