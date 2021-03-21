#include "interop.h"
#include "utilities.h"
#include "bytecode.h"

/********************************************************************************
 * definition
 *******************************************************************************/
struct instrumentr_bytecode_impl_t {
    struct instrumentr_model_impl_t model;
    SEXP r_sexp;
};

/********************************************************************************
 * finalize
 *******************************************************************************/

void instrumentr_bytecode_finalize(instrumentr_model_t model) {
    instrumentr_bytecode_t bytecode = (instrumentr_bytecode_t)(model);

    bytecode->r_sexp = NULL;
}

/********************************************************************************
 * create
 *******************************************************************************/

instrumentr_bytecode_t instrumentr_bytecode_create(instrumentr_state_t state,
                                                   SEXP r_sexp) {
    /* TODO: make foreign for instrumentr bytecode */
    instrumentr_model_t model =
        instrumentr_model_create(state,
                                 sizeof(struct instrumentr_bytecode_impl_t),
                                 INSTRUMENTR_MODEL_TYPE_BYTECODE,
                                 instrumentr_bytecode_finalize,
                                 INSTRUMENTR_ORIGIN_LOCAL);

    instrumentr_bytecode_t bytecode = (instrumentr_bytecode_t)(model);

    bytecode->r_sexp = r_sexp;

    return bytecode;
}

/********************************************************************************
 * interop
 *******************************************************************************/

INSTRUMENTR_MODEL_INTEROP_DEFINE_API(bytecode, INSTRUMENTR_MODEL_TYPE_BYTECODE)

SEXP instrumentr_bytecode_get_sexp(instrumentr_bytecode_t bytecode) {
    return bytecode->r_sexp;
}

SEXP r_instrumentr_bytecode_get_sexp(SEXP r_bytecode) {
    instrumentr_bytecode_t bytecode = instrumentr_bytecode_unwrap(r_bytecode);
    return instrumentr_bytecode_get_sexp(bytecode);
}
