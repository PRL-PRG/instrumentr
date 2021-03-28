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
    struct instrumentr_value_impl_t value;
};

/********************************************************************************
 * finalize
 *******************************************************************************/

void instrumentr_symbol_finalize(instrumentr_value_t value) {
}

/********************************************************************************
 * create
 *******************************************************************************/

instrumentr_symbol_t instrumentr_symbol_create(instrumentr_state_t state,
                                               SEXP r_sexp) {
    /* TODO: make foreign for instrumentr symbol */
    instrumentr_value_t value =
        instrumentr_value_create(state,
                                 sizeof(struct instrumentr_symbol_impl_t),
                                 INSTRUMENTR_VALUE_TYPE_SYMBOL,
                                 instrumentr_symbol_finalize,
                                 INSTRUMENTR_ORIGIN_LOCAL,
                                 r_sexp);

    instrumentr_symbol_t symbol = (instrumentr_symbol_t)(value);

    return symbol;
}

/********************************************************************************
 * interop
 *******************************************************************************/

INSTRUMENTR_VALUE_DEFINE_API(INSTRUMENTR_VALUE_TYPE_SYMBOL, symbol, symbol)

instrumentr_char_t instrumentr_symbol_get_element(instrumentr_symbol_t symbol) {
    SEXP r_char = PRINTNAME(instrumentr_symbol_get_sexp(symbol));
    instrumentr_state_t state = instrumentr_symbol_get_state(symbol);
    instrumentr_value_t value =
        instrumentr_state_value_table_lookup(state, r_char, 1);

    return instrumentr_value_as_char(value);
}

SEXP r_instrumentr_symbol_get_element(SEXP r_symbol) {
    instrumentr_symbol_t symbol = instrumentr_symbol_unwrap(r_symbol);
    instrumentr_char_t charval = instrumentr_symbol_get_element(symbol);
    return instrumentr_char_wrap(charval);
}
