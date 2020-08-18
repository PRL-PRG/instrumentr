#include "FunctionDetachCallback.hpp"
#include "Application.hpp"
#include "Context.hpp"
#include "Package.hpp"
#include "Function.hpp"

using instrumentr::Application;
using instrumentr::ApplicationSPtr;
using instrumentr::Context;
using instrumentr::ContextSPtr;
using instrumentr::Function;
using instrumentr::FunctionSPtr;
using instrumentr::Package;
using instrumentr::PackageSPtr;

namespace instrumentr {

typedef void (*callback_t)(ContextSPtr context,
                           ApplicationSPtr application,
                           PackageSPtr package,
                           FunctionSPtr function);

SEXP FunctionDetachCallback::class_ = nullptr;

void FunctionDetachCallback::initialize() {
    class_ = Callback::create_class("instrumentr_function_detach_callback");
    R_PreserveObject(class_);
}

void FunctionDetachCallback::finalize() {
    R_ReleaseObject(class_);
    class_ = NULL;
}

SEXP FunctionDetachCallback::get_class() {
    return class_;
}

void FunctionDetachCallback::invoke(SEXP r_context,
                                    SEXP r_application,
                                    SEXP r_package,
                                    SEXP r_function) {
    ContextSPtr context = from_sexp<Context>(r_context);

    if (is_c_callback()) {
        ApplicationSPtr application = from_sexp<Application>(r_application);
        PackageSPtr package = from_sexp<Package>(r_package);
        FunctionSPtr function = from_sexp<Function>(r_function);

        callback_t callback = get_function<callback_t>();
        callback(context, application, package, function);
    }
    /**/
    else {
        SEXP r_callback = get_function<SEXP>();
        SEXP r_environment = context->get_environment();

        Rf_eval(
            Rf_lang5(
                r_callback, r_context, r_application, r_package, r_function),
            r_environment);
    }
}

} // namespace instrumentr
