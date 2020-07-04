#ifndef INSTRUMENTR_INSTRUMENTR_HPP
#define INSTRUMENTR_INSTRUMENTR_HPP

#include <Rinternals.h>
#include <memory>

namespace instrumentr {

class Context;

using ContextSPtr = std::shared_ptr<Context>;

const char* get_commit_hash();

SEXP get_undefined_object();

bool is_undefined_object(SEXP object);

bool is_defined_object(SEXP object);

void add_context(ContextSPtr context);

void remove_context(ContextSPtr context);

bool is_tracing_enabled();

void set_tracing_status(bool tracing_status);

void enable_tracing();

void disable_tracing();

void reinstate_tracing();

void initialize_instrumentr(SEXP r_package_environment,
                            SEXP r_state_environment,
                            SEXP r_undefined_object);

void finalize_instrumentr();

} // namespace instrumentr

#endif /* INSTRUMENTR_INSTRUMENTR_HPP */
