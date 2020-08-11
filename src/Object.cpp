#include "Object.hpp"

namespace instrumentr {

int Object::id_counter_ = -1;

int Object::get_next_id_() {
    return ++id_counter_;
}

SEXP Object::create_class(const std::vector<const char*>& subclasses) {
    int size = subclasses.size();

    SEXP classnames = PROTECT(allocVector(STRSXP, 1 + size));

    for (int index = 0; index < size; ++index) {
        SET_STRING_ELT(classnames, index, mkChar(subclasses.at(index)));
    }

    SET_STRING_ELT(classnames, size, mkChar("instrumentr_object"));

    UNPROTECT(1);

    return classnames;
}

ObjectSPtr Object::from_sexp(SEXP r_object) {
    void* object = R_ExternalPtrAddr(r_object);
    if (object == NULL) {
        Rf_errorcall(R_NilValue, "Object::from_sexp: object is null");
    } else {
        return *static_cast<ObjectSPtr*>(object);
    }
}

} // namespace instrumentr
