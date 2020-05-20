#include "../inst/include/Argument.hpp"

ArgumentSPtr Argument::from_sexp(SEXP r_argument) {
    void* argument = R_ExternalPtrAddr(r_argument);
    if (argument == nullptr) {
        // TODO error;
    }
    return *static_cast<ArgumentSPtr*>(argument);
}

SEXP Argument::to_sexp(ArgumentSPtr argument) {
    SEXP r_argument = PROTECT(
        R_MakeExternalPtr(new ArgumentSPtr(argument), R_NilValue, R_NilValue));

    R_RegisterCFinalizerEx(r_argument, Argument::destroy_sexp, TRUE);

    /*TODO: global constant */
    setAttrib(r_argument, R_ClassSymbol, mkString("lightr_argument"));

    UNPROTECT(1);

    return r_argument;
}

void Argument::destroy_sexp(SEXP r_argument) {
    delete static_cast<ArgumentSPtr*>(R_ExternalPtrAddr(r_argument));
    R_SetExternalPtrAddr(r_argument, nullptr);
}
