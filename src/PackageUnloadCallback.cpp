#include "PackageUnloadCallback.hpp"
#include "Application.hpp"
#include "Context.hpp"
#include "Package.hpp"

using instrumentr::Application;
using instrumentr::ApplicationSPtr;
using instrumentr::Context;
using instrumentr::ContextSPtr;
using instrumentr::Package;
using instrumentr::PackageSPtr;

namespace instrumentr {

typedef void (*callback_t)(ContextSPtr context,
                           ApplicationSPtr application,
                           PackageSPtr package);

SEXP PackageUnloadCallback::class_ = nullptr;

void PackageUnloadCallback::initialize() {
    class_ = Callback::create_class("instrumentr_package_unload_callback");
    R_PreserveObject(class_);
}

void PackageUnloadCallback::finalize() {
    R_ReleaseObject(class_);
    class_ = NULL;
}

SEXP PackageUnloadCallback::get_class() {
    return class_;
}

void PackageUnloadCallback::invoke(SEXP r_context,
                                   SEXP r_application,
                                   SEXP r_package) {
    ContextSPtr context = from_sexp<Context>(r_context);

    initialize_pre_invocation_(context);

    if (is_c_callback()) {
        ApplicationSPtr application = from_sexp<Application>(r_application);
        PackageSPtr package = from_sexp<Package>(r_package);

        callback_t callback = get_function<callback_t>();
        callback(context, application, package);
    }
    /**/
    else {
        SEXP r_callback = get_function<SEXP>();
        SEXP r_environment = context->get_environment();

        Rf_eval(Rf_lang4(r_callback, r_context, r_application, r_package),
                r_environment);
    }

    finalize_post_invocation_(context);
}

} // namespace instrumentr
