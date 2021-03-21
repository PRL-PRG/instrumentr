#ifndef INSTRUMENTR_EVENT_H
#define INSTRUMENTR_EVENT_H

#include <instrumentr/types.h>

#ifdef __cplusplus
extern "C" {
#endif

int instrumentr_event_get_parameter_count(instrumentr_event_t event);

const char* instrumentr_event_to_string(instrumentr_event_t event);

SEXP instrumentr_event_wrap(instrumentr_event_t event);
instrumentr_event_t instrumentr_event_unwrap(SEXP r_event);

#ifdef __cplusplus
}
#endif

#endif /* INSTRUMENTR_EVENT_H */
