#include "Package.hpp"

namespace instrumentr {

SEXP Package::class_ = NULL;

void Package::initialize() {
    class_ = Object::create_class({"instrumentr_package"});
    R_PreserveObject(class_);
}

void Package::finalize() {
    R_ReleaseObject(class_);
    class_ = NULL;
}

SEXP Package::get_class() {
    return class_;
}

} // namespace instrumentr
