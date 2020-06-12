#include "Package.hpp"

namespace instrumentr {

SEXP Package::class_ = NULL;

void Package::initialize() {
    class_ = Object::create_class("instrumentr_package");
    R_PreserveObject(class_);
}

void Package::finalize() {
    R_ReleaseObject(class_);
    class_ = NULL;
}

SEXP Package::get_class() {
    return class_;
}

PackageSPtr Package::from_sexp(SEXP r_package) {
    void* package = R_ExternalPtrAddr(r_package);
    if (package == NULL) {
        Rf_errorcall(R_NilValue, "Package::from_sexp: object is null");
    } else {
        return *static_cast<PackageSPtr*>(package);
    }
}

SEXP Package::to_sexp(PackageSPtr package) {
    SEXP r_package = PROTECT(
        R_MakeExternalPtr(new PackageSPtr(package), R_NilValue, R_NilValue));

    R_RegisterCFinalizerEx(r_package, Package::destroy_sexp, TRUE);

    setAttrib(r_package, R_ClassSymbol, Package::get_class());

    UNPROTECT(1);

    return r_package;
}

void Package::destroy_sexp(SEXP r_package) {
    delete static_cast<PackageSPtr*>(R_ExternalPtrAddr(r_package));
    R_SetExternalPtrAddr(r_package, NULL);
}

} // namespace instrumentr
