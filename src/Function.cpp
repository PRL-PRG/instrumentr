#include "Function.hpp"

namespace instrumentr {

SEXP Function::class_ = NULL;

void Function::initialize() {
    class_ = Object::create_class({"instrumentr_function"});
    R_PreserveObject(class_);
}

void Function::finalize() {
    R_ReleaseObject(class_);
    class_ = NULL;
}

SEXP Function::get_class() {
    return class_;
}

FunctionSPtr Function::from_sexp(SEXP r_function) {
    void* function = R_ExternalPtrAddr(r_function);
    if (function == NULL) {
        Rf_errorcall(R_NilValue, "Function::from_sexp: object is null");
    } else {
        return *static_cast<FunctionSPtr*>(function);
    }
}

SEXP Function::to_sexp(FunctionSPtr function) {
    SEXP r_function = PROTECT(
        R_MakeExternalPtr(new FunctionSPtr(function), R_NilValue, R_NilValue));

    R_RegisterCFinalizerEx(r_function, Function::destroy_sexp, TRUE);

    setAttrib(r_function, R_ClassSymbol, Function::get_class());

    UNPROTECT(1);

    return r_function;
}

void Function::destroy_sexp(SEXP r_function) {
    delete static_cast<FunctionSPtr*>(R_ExternalPtrAddr(r_function));
    R_SetExternalPtrAddr(r_function, NULL);
}

} // namespace instrumentr
