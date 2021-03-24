#ifndef INSTRUMENTR_MODEL_TYPE_H
#define INSTRUMENTR_MODEL_TYPE_H

#include <instrumentr/Rincludes.h>
#include <instrumentr/api.h>

#ifdef __cplusplus
extern "C" {
#endif

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

INSTRUMENTR_MODEL_TYPE_API_MAP(INSTRUMENTR_API_INCLUDER)

/*******************************************************************************
 * class
 *******************************************************************************/

SEXP instrumentr_model_type_get_class(instrumentr_model_type_t model_type);

#ifdef __cplusplus
}
#endif

#endif /* INSTRUMENTR_MODEL_TYPE_H */
