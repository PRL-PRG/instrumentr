#ifndef INSTRUMENTR_ARGUMENT_INTERNALS_H
#define INSTRUMENTR_ARGUMENT_INTERNALS_H

#include <instrumentr/argument.h>

/********************************************************************************
 * create
 *******************************************************************************/

instrumentr_argument_t instrumentr_argument_create(const char* name,
                                                   SEXP r_promise);

#endif /* INSTRUMENTR_ARGUMENT_INTERNALS_H */
