#include "../inst/include/Object.hpp"
#include "r_api.h"

using lightr::Object;
using lightr::ObjectSPtr;

ObjectSPtr Object::from_sexp(SEXP r_object) {
    void* object = R_ExternalPtrAddr(r_object);
    if (object == nullptr) {
        // TODO error;
    }
    return *static_cast<ObjectSPtr*>(object);
}

SEXP r_object_get_id(SEXP r_object) {
    ObjectSPtr object = Object::from_sexp(r_object);
    return ScalarInteger(object->get_id());
}
