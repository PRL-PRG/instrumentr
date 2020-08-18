#include "Argument.hpp"

namespace instrumentr {

SEXP Argument::class_ = NULL;

void Argument::initialize() {
    class_ = Object::create_class({"instrumentr_argument"});
    R_PreserveObject(class_);
}

void Argument::finalize() {
    R_ReleaseObject(class_);
    class_ = NULL;
}

SEXP Argument::get_class() {
    return class_;
}

} // namespace instrumentr
