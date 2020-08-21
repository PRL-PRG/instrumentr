#include "ClosureCallEntryCallback.hpp"
#include "Application.hpp"
#include "Context.hpp"
#include "utilities.h"

namespace instrumentr {

typedef void (*callback_t)(ContextSPtr context,
                           ApplicationSPtr application,
                           SEXP r_call,
                           SEXP r_op,
                           SEXP r_args,
                           SEXP r_rho);

SEXP ClosureCallEntryCallback::class_ = nullptr;

void ClosureCallEntryCallback::initialize() {
    class_ = Callback::create_class("instrumentr_closure_call_entry_callback");
    R_PreserveObject(class_);
}

void ClosureCallEntryCallback::finalize() {
    R_ReleaseObject(class_);
    class_ = NULL;
}

SEXP ClosureCallEntryCallback::get_class() {
    return class_;
}

/* TODO: wrap call, op, args and rho  */
/* TODO: add dyntrace_dispatch_t */
void ClosureCallEntryCallback::invoke(SEXP r_context,
                                      SEXP r_application,
                                      SEXP r_call,
                                      SEXP r_op,
                                      SEXP r_args,
                                      SEXP r_rho) {
    ContextSPtr context = from_sexp<Context>(r_context);

    if (is_c_callback()) {
        ApplicationSPtr application = from_sexp<Application>(r_application);

        callback_t callback = get_function<callback_t>();
        callback(context, application, r_call, r_op, r_args, r_rho);
    }
    /**/
    else {
        SEXP r_function_name = get_function_name();
        SEXP r_environment = context->get_environment();

        Rf_eval(Rf_lang7(r_function_name,
                         r_context,
                         r_application,
                         r_call,
                         r_op,
                         r_args,
                         r_rho),
                r_environment);
    }
}

} // namespace instrumentr
