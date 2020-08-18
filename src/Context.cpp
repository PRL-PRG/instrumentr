#include "Context.hpp"

namespace instrumentr {

SEXP Context::class_ = NULL;

void Context::initialize() {
    class_ = Object::create_class({"instrumentr_context"});
    R_PreserveObject(class_);
}

void Context::finalize() {
    R_ReleaseObject(class_);
    class_ = NULL;
}

SEXP Context::get_class() {
    return class_;
}

} // namespace instrumentr
