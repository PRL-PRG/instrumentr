#include "Call.hpp"

namespace instrumentr {

SEXP Call::class_ = NULL;

void Call::initialize() {
    class_ = Object::create_class({"instrumentr_call"});
    R_PreserveObject(class_);
}

void Call::finalize() {
    R_ReleaseObject(class_);
    class_ = NULL;
}

SEXP Call::get_class() {
    return class_;
}

} // namespace instrumentr
