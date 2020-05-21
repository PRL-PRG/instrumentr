#include "../inst/include/Object.hpp"

namespace lightr {

int Object::id_counter_ = -1;

SEXP Object::class_ = nullptr;

int Object::get_next_id_() {
    return ++id_counter_;
}

void Object::initialize() {
    class_ = mkString("lightr_object");
    R_PreserveObject(class_);
}

SEXP Object::get_class() {
    return class_;
}

} // namespace lightr
