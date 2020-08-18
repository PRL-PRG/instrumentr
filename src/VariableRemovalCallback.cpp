#include "VariableRemovalCallback.hpp"
#include "Application.hpp"
#include "Context.hpp"

namespace instrumentr {

typedef void (*callback_t)(ContextSPtr context,
                           ApplicationSPtr application,
                           SEXP r_variable,
                           SEXP r_rho);

SEXP VariableRemovalCallback::class_ = nullptr;

void VariableRemovalCallback::initialize() {
    class_ = Callback::create_class("instrumentr_variable_removal_callback");
    R_PreserveObject(class_);
}

void VariableRemovalCallback::finalize() {
    R_ReleaseObject(class_);
    class_ = NULL;
}

SEXP VariableRemovalCallback::get_class() {
    return class_;
}

void VariableRemovalCallback::invoke(SEXP r_context,
                                     SEXP r_application,
                                     SEXP r_variable,
                                     SEXP r_rho) {
    ContextSPtr context = from_sexp<Context>(r_context);

    if (is_c_callback()) {
        ApplicationSPtr application = from_sexp<Application>(r_application);

        callback_t callback = get_function<callback_t>();
        callback(context, application, r_variable, r_rho);
    }
    /**/
    else {
        SEXP r_callback = get_function<SEXP>();
        SEXP r_environment = context->get_environment();

        Rf_eval(
            Rf_lang5(r_callback, r_context, r_application, r_variable, r_rho),
            r_environment);
    }
}

} // namespace instrumentr
