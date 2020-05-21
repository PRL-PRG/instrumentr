#include "../inst/include/Package.hpp"

PackageSPtr Package::from_sexp(SEXP r_package) {
    void* package = R_ExternalPtrAddr(r_package);
    if (package == nullptr) {
        // TODO error;
    }
    return *static_cast<PackageSPtr*>(package);
}

SEXP Package::to_sexp(PackageSPtr package) {
    SEXP r_package = PROTECT(
        R_MakeExternalPtr(new PackageSPtr(package), R_NilValue, R_NilValue));

    R_RegisterCFinalizerEx(r_package, Package::destroy_sexp, TRUE);

    /*TODO: global constant */
    setAttrib(r_package, R_ClassSymbol, mkString("lightr_package"));

    UNPROTECT(1);

    return r_package;
}

void Package::destroy_sexp(SEXP r_package) {
    delete static_cast<PackageSPtr*>(R_ExternalPtrAddr(r_package));
    R_SetExternalPtrAddr(r_package, nullptr);
}
