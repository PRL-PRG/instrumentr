#include "../inst/include/Call.hpp"

namespace lightr {

call_id_t Call::id_counter_ = -1;
SEXP Call::class_ = nullptr;

call_id_t Call::get_next_id_() {
    return ++id_counter_;
}

void Call::initialize() {
    class_ = mkString("lightr_call");
    R_PreserveObject(class_);
}

SEXP Call::get_class() {
    return class_;
}

CallSPtr Call::from_sexp(SEXP r_call) {
    void* call = R_ExternalPtrAddr(r_call);
    if (call == nullptr) {
        // TODO error;
    }
    return *static_cast<CallSPtr*>(call);
}

SEXP Call::to_sexp(CallSPtr call) {
    SEXP r_call =
        PROTECT(R_MakeExternalPtr(new CallSPtr(call), R_NilValue, R_NilValue));

    R_RegisterCFinalizerEx(r_call, Call::destroy_sexp, TRUE);

    setAttrib(r_call, R_ClassSymbol, Call::get_class());

    UNPROTECT(1);

    return r_call;
}

void Call::destroy_sexp(SEXP r_call) {
    delete static_cast<CallSPtr*>(R_ExternalPtrAddr(r_call));
    R_SetExternalPtrAddr(r_call, nullptr);
}

} // namespace lightr
