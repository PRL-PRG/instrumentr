#include "ObjectCoerceCallback.hpp"
#include "Application.hpp"
#include "Context.hpp"

namespace instrumentr {

typedef void (*callback_t)(ContextSPtr context,
                           ApplicationSPtr application,
                           SEXP r_input,
                           SEXP r_output);

SEXP ObjectCoerceCallback::class_ = nullptr;

void ObjectCoerceCallback::initialize() {
    class_ = Callback::create_class("instrumentr_object_coerce_callback");
    R_PreserveObject(class_);
}

void ObjectCoerceCallback::finalize() {
    R_ReleaseObject(class_);
    class_ = NULL;
}

SEXP ObjectCoerceCallback::get_class() {
    return class_;
}

void ObjectCoerceCallback::invoke(SEXP r_context,
                                  SEXP r_application,
                                  SEXP r_input,
                                  SEXP r_output) {
    ContextSPtr context = from_sexp<Context>(r_context);

    if (is_c_callback()) {
        ApplicationSPtr application = from_sexp<Application>(r_application);

        callback_t callback = get_function<callback_t>();
        callback(context, application, r_input, r_output);
    }
    /**/
    else {
        SEXP r_function_name = get_function_name();
        SEXP r_environment = context->get_environment();

        Rf_eval(
            Rf_lang5(
                r_function_name, r_context, r_application, r_input, r_output),
            r_environment);
    }
}

} // namespace instrumentr
