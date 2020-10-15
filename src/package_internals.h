#ifndef INSTRUMENTR_PACKAGE_INTERNALS_H
#define INSTRUMENTR_PACKAGE_INTERNALS_H

#include <instrumentr/package.h>

/********************************************************************************
 * functions
 *******************************************************************************/

/*  mutator  */
void instrumentr_package_append_function(instrumentr_package_t package,
                                         instrumentr_function_t function);

/*  mutator  */
void instrumentr_package_remove_function(instrumentr_package_t package,
                                         instrumentr_function_t function);

#endif /* INSTRUMENTR_PACKAGE_INTERNALS_H */
