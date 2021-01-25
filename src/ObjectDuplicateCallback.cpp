#include "ObjectDuplicateCallback.hpp"
#include "Application.hpp"
#include "Context.hpp"

namespace instrumentr {

typedef void (*callback_t)(ContextSPtr context,
                           ApplicationSPtr application,
                           SEXP r_input,
                           SEXP r_output,
                           SEXP r_deep);

SEXP ObjectDuplicateCallback::class_ = nullptr;

void ObjectDuplicateCallback::initialize() {
    class_ = Callback::create_class("instrumentr_object_duplicate_callback");
    R_PreserveObject(class_);
}

void ObjectDuplicateCallback::finalize() {
    R_ReleaseObject(class_);
    class_ = NULL;
}

SEXP ObjectDuplicateCallback::get_class() {
    return class_;
}

void ObjectDuplicateCallback::invoke(SEXP r_context,
                                     SEXP r_application,
                                     SEXP r_input,
                                     SEXP r_output,
                                     SEXP r_deep) {
    ContextSPtr context = from_sexp<Context>(r_context);

    if (is_c_callback()) {
        ApplicationSPtr application = from_sexp<Application>(r_application);

        callback_t callback = get_function<callback_t>();
        callback(context, application, r_input, r_output, r_deep);
    }
    /**/
    else {
        SEXP r_function_name = get_function_name();
        SEXP r_environment = context->get_environment();

        Rf_eval(Rf_lang6(r_function_name,
                         r_context,
                         r_application,
                         r_input,
                         r_output,
                         r_deep),
                r_environment);
    }
}

} // namespace instrumentr
