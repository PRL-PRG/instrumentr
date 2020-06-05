#include "../inst/include/Object.hpp"

namespace lightr {

int Object::id_counter_ = -1;

int Object::get_next_id_() {
    return ++id_counter_;
}

SEXP Object::create_class(const char* subclass) {
    SEXP classnames = PROTECT(allocVector(STRSXP, 2));
    SET_STRING_ELT(classnames, 0, mkChar(subclass));
    SET_STRING_ELT(classnames, 1, mkChar("lightr_object"));
    UNPROTECT(1);
    return classnames;
}

SEXP Object::create_class(const std::string& subclass) {
    return Object::create_class(subclass.c_str());
}

} // namespace lightr
