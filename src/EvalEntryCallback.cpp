#include "EvalEntryCallback.hpp"
#include "Application.hpp"
#include "Context.hpp"
#include "utilities.h"

namespace instrumentr {

typedef void (*callback_t)(ContextSPtr context,
                           ApplicationSPtr application,
                           SEXP r_expression,
                           SEXP r_rho);

SEXP EvalEntryCallback::class_ = nullptr;

void EvalEntryCallback::initialize() {
    class_ = Callback::create_class("instrumentr_eval_entry_callback");
    R_PreserveObject(class_);
}

void EvalEntryCallback::finalize() {
    R_ReleaseObject(class_);
    class_ = NULL;
}

SEXP EvalEntryCallback::get_class() {
    return class_;
}

/* TODO: wrap expression */
void EvalEntryCallback::invoke(SEXP r_context,
                               SEXP r_application,
                               SEXP r_expression,
                               SEXP r_rho) {
    ContextSPtr context = from_sexp<Context>(r_context);

    if (is_c_callback()) {
        ApplicationSPtr application = from_sexp<Application>(r_application);

        callback_t callback = get_function<callback_t>();
        callback(context, application, r_expression, r_rho);
    }
    /**/
    else {
        SEXP r_function_name = get_function_name();
        SEXP r_environment = context->get_environment();

        Rf_eval(
            Rf_lang5(
                r_function_name, r_context, r_application, r_expression, r_rho),
            r_environment);
    }
}

} // namespace instrumentr
