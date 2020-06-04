#ifndef LIGHTR_LIGHTR_HPP
#define LIGHTR_LIGHTR_HPP

#include "Application.hpp"
#include "Context.hpp"

namespace lightr {

bool is_tracing_enabled();

void enable_tracing();

void disable_tracing();

void reinstate_tracing();

void initialize_lightr(SEXP r_package_environment, SEXP r_state_environment);

} // namespace lightr

#endif /* LIGHTR_LIGHTR_HPP */
