#include "../inst/include/Package.hpp"

namespace lightr {

SEXP Package::class_ = nullptr;

void Package::initialize() {
    class_ = Object::create_class("lightr_package");
    R_PreserveObject(class_);
}

SEXP Package::get_class() {
    return class_;
}

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

    setAttrib(r_package, R_ClassSymbol, Package::get_class());

    UNPROTECT(1);

    return r_package;
}

void Package::destroy_sexp(SEXP r_package) {
    delete static_cast<PackageSPtr*>(R_ExternalPtrAddr(r_package));
    R_SetExternalPtrAddr(r_package, nullptr);
}

} // namespace lightr
