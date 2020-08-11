#include "Object.hpp"
#include "r_object.h"

using instrumentr::Object;
using instrumentr::ObjectSPtr;

SEXP r_object_get_id(SEXP r_object) {
    ObjectSPtr object = Object::from_sexp(r_object);
    return ScalarInteger(object->get_id());
}

SEXP r_object_set_data(SEXP r_object, SEXP r_data) {
    ObjectSPtr object = Object::from_sexp(r_object);
    object->set_data(r_data);
    return R_NilValue;
}

SEXP r_object_get_data(SEXP r_object) {
    ObjectSPtr object = Object::from_sexp(r_object);
    SEXP r_data = object->get_data();
    return r_data;
}

SEXP r_object_remove_data(SEXP r_object) {
    ObjectSPtr object = Object::from_sexp(r_object);
    object->remove_data();
    return R_NilValue;
}

SEXP r_object_has_data(SEXP r_object) {
    ObjectSPtr object = Object::from_sexp(r_object);
    return ScalarLogical(object->has_data());
}
