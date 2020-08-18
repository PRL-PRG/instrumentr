#include "Application.hpp"

namespace instrumentr {

SEXP Application::class_ = NULL;

void Application::initialize() {
    class_ = Object::create_class({"instrumentr_application"});
    R_PreserveObject(class_);
}

void Application::finalize() {
    R_ReleaseObject(class_);
    class_ = NULL;
}

SEXP Application::get_class() {
    return class_;
}

} // namespace instrumentr
