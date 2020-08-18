#include "Parameter.hpp"

namespace instrumentr {

SEXP Parameter::class_ = NULL;

void Parameter::initialize() {
    class_ = Object::create_class({"instrumentr_parameter"});
    R_PreserveObject(class_);
}

void Parameter::finalize() {
    R_ReleaseObject(class_);
    class_ = NULL;
}

SEXP Parameter::get_class() {
    return class_;
}

} // namespace instrumentr
