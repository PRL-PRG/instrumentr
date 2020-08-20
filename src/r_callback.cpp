#include "r_callback.h"
#include "Callback.hpp"

using instrumentr::Callback;
using instrumentr::CallbackSPtr;

SEXP r_callback_is_r_callback(SEXP r_callback) {
    CallbackSPtr callback = from_sexp<Callback>(r_callback);
    int result = callback->is_r_callback();
    return ScalarLogical(result);
}

SEXP r_callback_is_c_callback(SEXP r_callback) {
    CallbackSPtr callback = from_sexp<Callback>(r_callback);
    int result = callback->is_c_callback();
    return ScalarLogical(result);
}

/* TODO: add class and tag to external ptr */
SEXP r_callback_get_function(SEXP r_callback) {
    CallbackSPtr callback = from_sexp<Callback>(r_callback);

    if (callback->is_r_callback()) {
        return callback->get_function<SEXP>();
    } else {
        void* function = callback->get_function<void*>();
        return R_MakeExternalPtr(function, R_NilValue, R_NilValue);
    }
}

SEXP r_callback_activate(SEXP r_callback) {
    CallbackSPtr callback = from_sexp<Callback>(r_callback);
    callback->activate();
    return r_callback;
}

SEXP r_callback_deactivate(SEXP r_callback) {
    CallbackSPtr callback = from_sexp<Callback>(r_callback);
    callback->deactivate();
    return r_callback;
}

SEXP r_callback_reinstate(SEXP r_callback) {
    CallbackSPtr callback = from_sexp<Callback>(r_callback);
    callback->reinstate();
    return r_callback;
}

SEXP r_callback_is_active(SEXP r_callback) {
    CallbackSPtr callback = from_sexp<Callback>(r_callback);
    return ScalarLogical(callback->is_active());
}
