#include "CallStack.hpp"

namespace instrumentr {

SEXP CallStack::class_ = NULL;

void CallStack::initialize() {
    class_ = Object::create_class({"instrumentr_call_stack"});
    R_PreserveObject(class_);
}

void CallStack::finalize() {
    R_ReleaseObject(class_);
    class_ = NULL;
}

SEXP CallStack::get_class() {
    return class_;
}

CallStackSPtr CallStack::from_sexp(SEXP r_call_stack) {
    void* call_stack = R_ExternalPtrAddr(r_call_stack);
    if (call_stack == NULL) {
        Rf_errorcall(R_NilValue, "CallStack::from_sexp: object is null");
    } else {
        return *static_cast<CallStackSPtr*>(call_stack);
    }
}

SEXP CallStack::to_sexp(CallStackSPtr call_stack) {
    SEXP r_call_stack = PROTECT(R_MakeExternalPtr(
        new CallStackSPtr(call_stack), R_NilValue, R_NilValue));

    R_RegisterCFinalizerEx(r_call_stack, CallStack::destroy_sexp, TRUE);

    setAttrib(r_call_stack, R_ClassSymbol, CallStack::get_class());

    UNPROTECT(1);

    return r_call_stack;
}

void CallStack::destroy_sexp(SEXP r_call_stack) {
    delete static_cast<CallStackSPtr*>(R_ExternalPtrAddr(r_call_stack));
    R_SetExternalPtrAddr(r_call_stack, NULL);
}

} // namespace instrumentr
