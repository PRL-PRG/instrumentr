#include "VariableDefinitionCallback.hpp"
#include "Application.hpp"
#include "Context.hpp"

namespace instrumentr {

typedef void (*callback_t)(ContextSPtr context,
                           ApplicationSPtr application,
                           SEXP r_variable,
                           SEXP r_value,
                           SEXP r_rho);

SEXP VariableDefinitionCallback::class_ = nullptr;

void VariableDefinitionCallback::initialize() {
    class_ = Callback::create_class("instrumentr_variable_definition_callback");
    R_PreserveObject(class_);
}

void VariableDefinitionCallback::finalize() {
    R_ReleaseObject(class_);
    class_ = NULL;
}

SEXP VariableDefinitionCallback::get_class() {
    return class_;
}

void VariableDefinitionCallback::invoke(ContextSPtr context,
                                        ApplicationSPtr application,
                                        SEXP r_variable,
                                        SEXP r_value,
                                        SEXP r_rho) {
    if (is_c_callback()) {
        callback_t callback = get_function<callback_t>();
        callback(context, application, r_variable, r_value, r_rho);
    }
    /**/
    else {
        SEXP r_function_name = get_function_name();
        SEXP r_environment = context->get_environment();

        SEXP r_context = to_sexp(context);
        SEXP r_application = to_sexp(application);

        Rf_eval(Rf_lang6(r_function_name,
                         r_context,
                         r_application,
                         r_variable,
                         r_value,
                         r_rho),
                r_environment);
    }
}

} // namespace instrumentr
