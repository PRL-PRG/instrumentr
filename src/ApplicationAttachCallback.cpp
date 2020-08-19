#include "ApplicationAttachCallback.hpp"
#include "Application.hpp"
#include "Context.hpp"

using instrumentr::Application;
using instrumentr::ApplicationSPtr;
using instrumentr::Context;
using instrumentr::ContextSPtr;

namespace instrumentr {

typedef void (*callback_t)(ContextSPtr context, ApplicationSPtr application);

SEXP ApplicationAttachCallback::class_ = nullptr;

void ApplicationAttachCallback::initialize() {
    class_ = Callback::create_class("instrumentr_application_attach_callback");
    R_PreserveObject(class_);
}

void ApplicationAttachCallback::finalize() {
    R_ReleaseObject(class_);
    class_ = NULL;
}

SEXP ApplicationAttachCallback::get_class() {
    return class_;
}

void ApplicationAttachCallback::invoke(SEXP r_context, SEXP r_application) {
    ContextSPtr context = from_sexp<Context>(r_context);

    if (is_c_callback()) {
        ApplicationSPtr application = from_sexp<Application>(r_application);

        callback_t callback = get_function<callback_t>();
        callback(context, application);
    }
    /**/
    else {
        SEXP r_function_name = get_function_name();
        SEXP r_environment = context->get_environment();

        Rf_eval(Rf_lang3(r_function_name, r_context, r_application),
                r_environment);
    }
}

} // namespace instrumentr
