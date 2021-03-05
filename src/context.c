#include "object.h"
#include "interop.h"
#include "utilities.h"
#include "object.h"

/********************************************************************************
 * definition
 *******************************************************************************/

struct instrumentr_context_impl_t {
    struct instrumentr_object_impl_t object;
    void *pointer;
};

/********************************************************************************
 * finalize
 *******************************************************************************/

void instrumentr_context_finalize(instrumentr_object_t object) {
    instrumentr_context_t context = (instrumentr_context_t)(object);

    context->pointer = NULL;
}

/********************************************************************************
 * create
 *******************************************************************************/

instrumentr_context_t instrumentr_context_create(void *pointer) {
    instrumentr_object_t object =
        instrumentr_object_create(sizeof(struct instrumentr_context_impl_t),
                                  INSTRUMENTR_CONTEXT,
                                  instrumentr_context_finalize);

    instrumentr_context_t context = (instrumentr_context_t)(object);
    context->pointer = pointer;

    return context;
}

/********************************************************************************
 * interop
 *******************************************************************************/

SEXP instrumentr_context_wrap(instrumentr_context_t context) {
    return instrumentr_object_wrap((instrumentr_object_t)(context));
}

instrumentr_context_t instrumentr_context_unwrap(SEXP r_context) {
    instrumentr_object_t object =
        instrumentr_object_unwrap(r_context, INSTRUMENTR_CONTEXT);
    return (instrumentr_context_t)(object);
}

/********************************************************************************
 * r_context
 *******************************************************************************/

/* accessor  */
void* instrumentr_context_get_pointer(instrumentr_context_t context) {
    return context->pointer;
}

SEXP r_instrumentr_context_get_pointer(SEXP r_context) {
    instrumentr_context_t context = instrumentr_context_unwrap(r_context);
    void* result = instrumentr_context_get_pointer(context);
    return instrumentr_c_pointer_to_r_externalptr(result, R_NilValue, R_NilValue, NULL);
}