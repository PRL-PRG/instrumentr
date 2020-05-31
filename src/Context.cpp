#include "../inst/include/Context.hpp"

namespace lightr {

SEXP Context::class_ = nullptr;

void Context::initialize() {
    class_ = mkString("lightr_context");
    R_PreserveObject(class_);
}

SEXP Context::get_class() {
    return class_;
}

ContextSPtr Context::from_sexp(SEXP r_context) {
    void* context = R_ExternalPtrAddr(r_context);
    if (context == nullptr) {
        // TODO error;
    }
    return *static_cast<ContextSPtr*>(context);
}

SEXP Context::to_sexp(ContextSPtr context) {
    SEXP r_context = PROTECT(R_MakeExternalPtr(
        new ContextSPtr(context), R_NilValue, R_NilValue));

    R_RegisterCFinalizerEx(r_context, Context::destroy_sexp, TRUE);

    setAttrib(r_context, R_ClassSymbol, Context::get_class());

    UNPROTECT(1);

    return r_context;
}

void Context::destroy_sexp(SEXP r_context) {
    delete static_cast<ContextSPtr*>(R_ExternalPtrAddr(r_context));
    R_SetExternalPtrAddr(r_context, nullptr);
}

} // namespace lightr
