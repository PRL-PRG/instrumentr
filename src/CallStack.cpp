#include "CallStack.hpp"

namespace instrumentr {

SEXP CallStack::class_ = NULL;

void CallStack::initialize() {
    class_ = Object::create_class({"instrumentr_call_stack"});
    R_PreserveObject(class_);
}

void CallStack::finalize() {
    R_ReleaseObject(class_);
    class_ = NULL;
}

SEXP CallStack::get_class() {
    return class_;
}

} // namespace instrumentr
