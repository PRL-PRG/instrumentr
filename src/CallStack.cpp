#include "../inst/include/CallStack.hpp"

CallStackSPtr call_stack;

void initialize_call_stack() {
    call_stack = std::make_shared<CallStack>();
}

CallStackSPtr CallStack::from_sexp(SEXP r_call_stack) {
    void* call = R_ExternalPtrAddr(r_call_stack);
    if (call == nullptr) {
        // TODO error;
    }
    return *static_cast<CallStackSPtr*>(call);
}

SEXP CallStack::to_sexp(CallStackSPtr call) {
    SEXP r_call_stack = PROTECT(
        R_MakeExternalPtr(new CallStackSPtr(call), R_NilValue, R_NilValue));

    R_RegisterCFinalizerEx(r_call_stack, CallStack::destroy_sexp, TRUE);

    /*TODO: global constant */
    setAttrib(r_call_stack, R_ClassSymbol, mkString("injectr_call_stack"));

    UNPROTECT(1);

    return r_call_stack;
}

void CallStack::destroy_sexp(SEXP r_call_stack) {
    delete static_cast<CallStackSPtr*>(R_ExternalPtrAddr(r_call_stack));
    R_SetExternalPtrAddr(r_call_stack, nullptr);
}

CallStackSPtr get_call_stack() {
    return call_stack;
}
