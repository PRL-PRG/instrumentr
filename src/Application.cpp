#include "../inst/include/Application.hpp"

namespace lightr {

ApplicationSPtr Application::from_sexp(SEXP r_application) {
    void* application = R_ExternalPtrAddr(r_application);
    if (application == nullptr) {
        // TODO error;
    }
    return *static_cast<ApplicationSPtr*>(application);
}

SEXP Application::to_sexp(ApplicationSPtr application) {
    SEXP r_application = PROTECT(R_MakeExternalPtr(
        new ApplicationSPtr(application), R_NilValue, R_NilValue));

    R_RegisterCFinalizerEx(r_application, Application::destroy_sexp, TRUE);

    /*TODO: global constant */
    setAttrib(r_application, R_ClassSymbol, mkString("lightr_application"));

    UNPROTECT(1);

    return r_application;
}

void Application::destroy_sexp(SEXP r_application) {
    delete static_cast<ApplicationSPtr*>(R_ExternalPtrAddr(r_application));
    R_SetExternalPtrAddr(r_application, nullptr);
}

} // namespace lightr

