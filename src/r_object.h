#include "Rincludes.h"

extern "C" {
SEXP r_object_get_id(SEXP r_object);
SEXP r_object_set_data(SEXP r_object, SEXP r_data);
SEXP r_object_get_data(SEXP r_object);
SEXP r_object_remove_data(SEXP r_object);
SEXP r_object_has_data(SEXP r_object);
}
