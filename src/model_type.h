#ifndef INSTRUMENTR_MODEL_TYPE_H
#define INSTRUMENTR_MODEL_TYPE_H

#ifdef __cplusplus
extern "C" {
#endif

#include <instrumentr/Rincludes.h>
#include <instrumentr/types.h>

/*******************************************************************************
 * initialize
 *******************************************************************************/

void instrumentr_model_type_initialize();

/*******************************************************************************
 * finalize
 *******************************************************************************/

void instrumentr_model_type_finalize();

/*******************************************************************************
 * name
 *******************************************************************************/

const char*
instrumentr_model_type_get_name(instrumentr_model_type_t model_type);

/*******************************************************************************
 * class
 *******************************************************************************/

SEXP instrumentr_model_type_get_class(instrumentr_model_type_t model_type);

#ifdef __cplusplus
}
#endif

#endif /* INSTRUMENTR_MODEL_TYPE_H */
