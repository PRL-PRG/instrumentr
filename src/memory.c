#include <instrumentr/memory.h>

void instrumentr_acquire_sexp(SEXP r_object) {
    R_PreserveObject(r_object);
}

void instrumentr_release_sexp(SEXP r_object) {
    R_ReleaseObject(r_object);
}
