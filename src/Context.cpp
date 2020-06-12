#include "Context.hpp"

namespace instrumentr {

SEXP ApplicationLoadCallbackSymbol = NULL;
SEXP ApplicationUnloadCallbackSymbol = NULL;
SEXP ApplicationAttachCallbackSymbol = NULL;
SEXP ApplicationDetachCallbackSymbol = NULL;
SEXP PackageLoadCallbackSymbol = NULL;
SEXP PackageUnloadCallbackSymbol = NULL;
SEXP PackageAttachCallbackSymbol = NULL;
SEXP PackageDetachCallbackSymbol = NULL;
SEXP FunctionAttachCallbackSymbol = NULL;
SEXP FunctionDetachCallbackSymbol = NULL;
SEXP CallEntryCallbackSymbol = NULL;
SEXP CallExitCallbackSymbol = NULL;

SEXP Context::class_ = nullptr;

void Context::initialize() {
    class_ = Object::create_class("instrumentr_context");
    R_PreserveObject(class_);
    ApplicationLoadCallbackSymbol = Rf_install("application_load_callback");
    ApplicationUnloadCallbackSymbol = Rf_install("application_unload_callback");
    ApplicationAttachCallbackSymbol = Rf_install("application_attach_callback");
    ApplicationDetachCallbackSymbol = Rf_install("application_detach_callback");
    PackageLoadCallbackSymbol = Rf_install("package_load_callback");
    PackageUnloadCallbackSymbol = Rf_install("package_unload_callback");
    PackageAttachCallbackSymbol = Rf_install("package_attach_callback");
    PackageDetachCallbackSymbol = Rf_install("package_detach_callback");
    FunctionAttachCallbackSymbol = Rf_install("function_attach_callback");
    FunctionDetachCallbackSymbol = Rf_install("function_detach_callback");
    CallEntryCallbackSymbol = Rf_install("call_entry_callback");
    CallExitCallbackSymbol = Rf_install("call_exit_callback");
}

SEXP Context::get_class() {
    return class_;
}

ContextSPtr Context::from_sexp(SEXP r_context) {
    void* context = R_ExternalPtrAddr(r_context);
    if (context == nullptr) {
        Rf_errorcall(R_NilValue, "Context::from_sexp: object is null");
    } else {
        return *static_cast<ContextSPtr*>(context);
    }
}

SEXP Context::to_sexp(ContextSPtr context) {
    SEXP r_context = PROTECT(
        R_MakeExternalPtr(new ContextSPtr(context), R_NilValue, R_NilValue));

    R_RegisterCFinalizerEx(r_context, Context::destroy_sexp, TRUE);

    setAttrib(r_context, R_ClassSymbol, Context::get_class());

    UNPROTECT(1);

    return r_context;
}

void Context::destroy_sexp(SEXP r_context) {
    delete static_cast<ContextSPtr*>(R_ExternalPtrAddr(r_context));
    R_SetExternalPtrAddr(r_context, nullptr);
}

} // namespace instrumentr
