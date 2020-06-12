#ifndef INSTRUMENTR_INSTRUMENTR_HPP
#define INSTRUMENTR_INSTRUMENTR_HPP

#include <Rinternals.h>
#include "ExecutionContext.hpp"

namespace instrumentr {

bool is_tracing_enabled();

void set_tracing_status(bool tracing_status);

void enable_tracing();

void disable_tracing();

void reinstate_tracing();

void clear_tracing();

ExecutionContext peek_execution_context();

void push_execution_context(ExecutionContext execution_context);

ExecutionContext pop_execution_context();

void clear_execution_context();

SEXP get_undefined_object();

bool is_undefined_object(SEXP object);

bool is_defined_object(SEXP object);

void initialize_instrumentr(SEXP r_package_environment,
                       SEXP r_state_environment,
                       SEXP r_undefined_object);

void finalize_instrumentr();

} // namespace instrumentr

#endif /* INSTRUMENTR_INSTRUMENTR_HPP */
