#ifndef LIGHTR_LIGHTR_HPP
#define LIGHTR_LIGHTR_HPP

#include <Rinternals.h>

namespace lightr {

bool is_tracing_enabled();

void enable_tracing();

void disable_tracing();

void reinstate_tracing();

SEXP get_invalid_value();

bool is_invalid_value(SEXP value);

bool is_valid_value(SEXP value);

void initialize_lightr(SEXP r_package_environment,
                       SEXP r_state_environment,
                       SEXP r_invalid_value);

} // namespace lightr

#endif /* LIGHTR_LIGHTR_HPP */
