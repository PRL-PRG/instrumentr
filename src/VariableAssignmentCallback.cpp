#include "VariableAssignmentCallback.hpp"
#include "Application.hpp"
#include "Context.hpp"

namespace instrumentr {

typedef void (*callback_t)(ContextSPtr context,
                           ApplicationSPtr application,
                           SEXP r_variable,
                           SEXP r_value,
                           SEXP r_rho);

SEXP VariableAssignmentCallback::class_ = nullptr;

void VariableAssignmentCallback::initialize() {
    class_ = Callback::create_class("instrumentr_variable_assignment_callback");
    R_PreserveObject(class_);
}

void VariableAssignmentCallback::finalize() {
    R_ReleaseObject(class_);
    class_ = NULL;
}

SEXP VariableAssignmentCallback::get_class() {
    return class_;
}

void VariableAssignmentCallback::invoke(SEXP r_context,
                                        SEXP r_application,
                                        SEXP r_variable,
                                        SEXP r_value,
                                        SEXP r_rho) {
    ContextSPtr context = from_sexp<Context>(r_context);

    if (is_c_callback()) {
        ApplicationSPtr application = from_sexp<Application>(r_application);

        callback_t callback = get_function<callback_t>();
        callback(context, application, r_variable, r_value, r_rho);
    }
    /**/
    else {
        SEXP r_callback = get_function<SEXP>();
        SEXP r_environment = context->get_environment();

        Rf_eval(
            Rf_lang6(
                r_callback, r_context, r_application, r_variable, r_value, r_rho),
            r_environment);
    }
}

} // namespace instrumentr
