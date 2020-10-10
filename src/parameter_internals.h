#ifndef INSTRUMENTR_PARAMETER_INTERNALS_H
#define INSTRUMENTR_PARAMETER_INTERNALS_H

#include <instrumentr/parameter.h>

/********************************************************************************
 * create
 *******************************************************************************/

instrumentr_parameter_t instrumentr_parameter_create(const char* name,
                                                     int position,
                                                     SEXP r_default_expression);

/********************************************************************************
 * arguments
 *******************************************************************************/

void instrumentr_parameter_append_argument(instrumentr_parameter_t parameter,
                                           instrumentr_argument_t argument);

#endif /* INSTRUMENTR_PARAMETER_INTERNALS_H */
