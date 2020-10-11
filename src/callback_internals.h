#ifndef INSTRUMENTR_CALLBACK_INTERNALS_H
#define INSTRUMENTR_CALLBACK_INTERNALS_H

#include <instrumentr/callback.h>

/********************************************************************************
 * active
 *******************************************************************************/

/* mutator  */
int instrumentr_callback_activate(instrumentr_callback_t callback);

/* mutator  */
int instrumentr_callback_deactivate(instrumentr_callback_t callback);

#endif /* INSTRUMENTR_CALLBACK_INTERNALS_H */
