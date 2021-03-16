#ifndef INSTRUMENTR_OBJECT_TYPE_H
#define INSTRUMENTR_OBJECT_TYPE_H

#ifdef __cplusplus
extern "C" {
#endif

#include <instrumentr/Rincludes.h>
#include <instrumentr/types.h>

/*******************************************************************************
 * initialize
 *******************************************************************************/

void instrumentr_object_type_initialize();

/*******************************************************************************
 * finalize
 *******************************************************************************/

void instrumentr_object_type_finalize();

/*******************************************************************************
 * name
 *******************************************************************************/

const char*
instrumentr_object_type_get_name(instrumentr_object_type_t object_type);

/*******************************************************************************
 * class
 *******************************************************************************/

SEXP instrumentr_object_type_get_class(instrumentr_object_type_t object_type);

#ifdef __cplusplus
}
#endif

#endif /* INSTRUMENTR_OBJECT_TYPE_H */
