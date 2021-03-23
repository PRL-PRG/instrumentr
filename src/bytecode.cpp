#include "interop.h"
#include "utilities.h"
#include "bytecode.h"

/********************************************************************************
 * definition
 *******************************************************************************/
struct instrumentr_bytecode_impl_t {
    struct instrumentr_value_impl_t value;
};

/********************************************************************************
 * finalize
 *******************************************************************************/

void instrumentr_bytecode_finalize(instrumentr_value_t value) {
}

/********************************************************************************
 * create
 *******************************************************************************/

instrumentr_bytecode_t instrumentr_bytecode_create(instrumentr_state_t state,
                                                   SEXP r_sexp) {
    /* TODO: make foreign for instrumentr bytecode */
    instrumentr_value_t value =
        instrumentr_value_create(state,
                                 sizeof(struct instrumentr_bytecode_impl_t),
                                 INSTRUMENTR_VALUE_TYPE_BYTECODE,
                                 instrumentr_bytecode_finalize,
                                 INSTRUMENTR_ORIGIN_LOCAL,
                                 r_sexp);

    instrumentr_bytecode_t bytecode = (instrumentr_bytecode_t)(value);

    return bytecode;
}

/********************************************************************************
 * interop
 *******************************************************************************/

INSTRUMENTR_VALUE_DEFINE_API(INSTRUMENTR_VALUE_TYPE_BYTECODE,
                             bytecode,
                             bytecode)
