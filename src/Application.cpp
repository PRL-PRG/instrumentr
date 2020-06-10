#include "../inst/include/Application.hpp"

namespace instrumentr {

SEXP Application::class_ = nullptr;

void Application::initialize() {
    class_ = Object::create_class("instrumentr_application");
    R_PreserveObject(class_);
}

SEXP Application::get_class() {
    return class_;
}

ApplicationSPtr Application::from_sexp(SEXP r_application) {
    void* application = R_ExternalPtrAddr(r_application);
    if (application == nullptr) {
        Rf_errorcall(R_NilValue, "Application::from_sexp: object is null");
    } else {
        return *static_cast<ApplicationSPtr*>(application);
    }
}

SEXP Application::to_sexp(ApplicationSPtr application) {
    SEXP r_application = PROTECT(R_MakeExternalPtr(
        new ApplicationSPtr(application), R_NilValue, R_NilValue));

    R_RegisterCFinalizerEx(r_application, Application::destroy_sexp, TRUE);

    setAttrib(r_application, R_ClassSymbol, Application::get_class());

    UNPROTECT(1);

    return r_application;
}

void Application::destroy_sexp(SEXP r_application) {
    delete static_cast<ApplicationSPtr*>(R_ExternalPtrAddr(r_application));
    R_SetExternalPtrAddr(r_application, nullptr);
}

} // namespace instrumentr
