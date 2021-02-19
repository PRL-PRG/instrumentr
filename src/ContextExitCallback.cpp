#include "ContextExitCallback.hpp"
#include "Context.hpp"

namespace instrumentr {

typedef void (*callback_t)(ContextSPtr context,
                           ApplicationSPtr application,
                           void* call_context);

SEXP ContextExitCallback::class_ = nullptr;

void ContextExitCallback::initialize() {
    class_ = Callback::create_class("instrumentr_context_exit_callback");
    R_PreserveObject(class_);
}

void ContextExitCallback::finalize() {
    R_ReleaseObject(class_);
    class_ = NULL;
}

SEXP ContextExitCallback::get_class() {
    return class_;
}

void ContextExitCallback::invoke(ContextSPtr context,
                                 ApplicationSPtr application,
                                 void* call_context) {

    if (is_c_callback()) {
        callback_t callback = get_function<callback_t>();
        callback(context, application, call_context);
    }
    /**/
    else {
        SEXP r_function_name = get_function_name();
        SEXP r_environment = context->get_environment();

        SEXP r_context = to_sexp(context);
        SEXP r_application = to_sexp(application);
        SEXP r_call_context =
            PROTECT(R_MakeExternalPtr(call_context, R_NilValue, R_NilValue));

        Rf_eval(
            Rf_lang4(r_function_name, r_context, r_application, r_call_context),
            r_environment);

        UNPROTECT(1);
    }
}

} // namespace instrumentr
