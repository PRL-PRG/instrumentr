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

} // namespace instrumentr
