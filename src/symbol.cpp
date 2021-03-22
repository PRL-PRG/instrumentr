#include "interop.h"
#include "utilities.h"
#include "symbol.h"
#include "char.h"
#include "state.h"
#include "value.h"

/********************************************************************************
 * definition
 *******************************************************************************/
struct instrumentr_symbol_impl_t {
    struct instrumentr_model_impl_t model;
    SEXP r_sexp;
};

/********************************************************************************
 * finalize
 *******************************************************************************/

void instrumentr_symbol_finalize(instrumentr_model_t model) {
    instrumentr_symbol_t symbol = (instrumentr_symbol_t)(model);

    symbol->r_sexp = NULL;
}

/********************************************************************************
 * create
 *******************************************************************************/

instrumentr_symbol_t instrumentr_symbol_create(instrumentr_state_t state,
                                               SEXP r_sexp) {
    /* TODO: make foreign for instrumentr symbol */
    instrumentr_model_t model =
        instrumentr_model_create(state,
                                 sizeof(struct instrumentr_symbol_impl_t),
                                 INSTRUMENTR_MODEL_TYPE_SYMBOL,
                                 instrumentr_symbol_finalize,
                                 INSTRUMENTR_ORIGIN_LOCAL);

    instrumentr_symbol_t symbol = (instrumentr_symbol_t)(model);

    symbol->r_sexp = r_sexp;

    return symbol;
}

/********************************************************************************
 * interop
 *******************************************************************************/

INSTRUMENTR_MODEL_INTEROP_DEFINE_API(symbol, INSTRUMENTR_MODEL_TYPE_SYMBOL)

SEXP instrumentr_symbol_get_sexp(instrumentr_symbol_t symbol) {
    return instrumentr_sexp_quote(symbol->r_sexp);
}

SEXP r_instrumentr_symbol_get_sexp(SEXP r_symbol) {
    instrumentr_symbol_t symbol = instrumentr_symbol_unwrap(r_symbol);
    return instrumentr_symbol_get_sexp(symbol);
}

instrumentr_char_t instrumentr_symbol_get_name(instrumentr_symbol_t symbol) {
    SEXP r_char = PRINTNAME(symbol->r_sexp);
    instrumentr_state_t state = instrumentr_model_get_state(symbol);
    instrumentr_value_t value =
        instrumentr_state_value_table_lookup(state, r_char, 1);

    return instrumentr_value_as_char(value);
}

SEXP r_instrumentr_symbol_get_name(SEXP r_symbol) {
    instrumentr_symbol_t symbol = instrumentr_symbol_unwrap(r_symbol);
    instrumentr_char_t charval = instrumentr_symbol_get_name(symbol);
    return instrumentr_char_wrap(charval);
}
