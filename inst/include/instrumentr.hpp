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

template <typename T>
std::shared_ptr<T> from_sexp(SEXP r_object) {
    void* object_sptr_ptr = R_ExternalPtrAddr(r_object);
    if (object_sptr_ptr == NULL) {
        Rf_error("from_sexp: object is null");
    } else {
        return *static_cast<std::shared_ptr<T>*>(object_sptr_ptr);
    }
}

template <typename T>
void destroy_sexp(SEXP r_object) {
    delete static_cast<std::shared_ptr<T>*>(R_ExternalPtrAddr(r_object));
    R_SetExternalPtrAddr(r_object, NULL);
}

template <typename T>
SEXP to_sexp(std::shared_ptr<T> object_sptr) {
    SEXP r_object = R_MakeExternalPtr(
        new std::shared_ptr<T>(object_sptr), R_NilValue, R_NilValue);

    PROTECT(r_object);

    R_RegisterCFinalizerEx(r_object, destroy_sexp<T>, TRUE);

    setAttrib(r_object, R_ClassSymbol, T::get_class());

    UNPROTECT(1);

    return r_object;
}

} // namespace instrumentr

#endif /* INSTRUMENTR_INSTRUMENTR_HPP */
