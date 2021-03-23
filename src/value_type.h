#ifndef INSTRUMENTR_VALUE_TYPE_H
#define INSTRUMENTR_VALUE_TYPE_H

#ifdef __cplusplus
extern "C" {
#endif

#include <instrumentr/Rincludes.h>
#include <instrumentr/types.h>

/*******************************************************************************
 * initialize
 *******************************************************************************/

void instrumentr_value_type_initialize();

/*******************************************************************************
 * finalize
 *******************************************************************************/

void instrumentr_value_type_finalize();

/*******************************************************************************
 * name
 *******************************************************************************/

const char*
instrumentr_value_type_get_name(instrumentr_value_type_t value_type);

/*******************************************************************************
 * class
 *******************************************************************************/

SEXP instrumentr_value_type_get_class(instrumentr_value_type_t value_type);

#ifdef __cplusplus
}
#endif

#endif /* INSTRUMENTR_VALUE_TYPE_H */
