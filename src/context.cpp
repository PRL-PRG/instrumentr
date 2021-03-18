#include "context.h"
#include "interop.h"
#include "utilities.h"

/********************************************************************************
 * definition
 *******************************************************************************/

struct instrumentr_context_impl_t {
    struct instrumentr_model_impl_t model;
    void* pointer;
};

/********************************************************************************
 * finalize
 *******************************************************************************/

void instrumentr_context_finalize(instrumentr_model_t model) {
    instrumentr_context_t context = (instrumentr_context_t)(model);

    context->pointer = NULL;
}

/********************************************************************************
 * create
 *******************************************************************************/

instrumentr_context_t instrumentr_context_create(instrumentr_state_t state,
                                                 void* pointer) {
    instrumentr_model_t model =
        instrumentr_model_create(state,
                                 sizeof(struct instrumentr_context_impl_t),
                                 INSTRUMENTR_MODEL_TYPE_CONTEXT,
                                 instrumentr_context_finalize,
                                 INSTRUMENTR_ORIGIN_LOCAL);

    instrumentr_context_t context = (instrumentr_context_t)(model);
    context->pointer = pointer;

    return context;
}

/********************************************************************************
 * interop
 *******************************************************************************/

INSTRUMENTR_MODEL_INTEROP_DEFINE_API(context, INSTRUMENTR_MODEL_TYPE_CONTEXT)

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
    return instrumentr_c_pointer_to_r_externalptr(
        result, R_NilValue, R_NilValue, NULL);
}
