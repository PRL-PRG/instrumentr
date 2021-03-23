#include "interop.h"
#include "utilities.h"
#include "character.h"
#include "state.h"
#include "char.h"
#include "value.h"

/********************************************************************************
 * definition
 *******************************************************************************/
struct instrumentr_character_impl_t {
    struct instrumentr_value_impl_t value;
};

/********************************************************************************
 * finalize
 *******************************************************************************/

void instrumentr_character_finalize(instrumentr_value_t value) {
}

/********************************************************************************
 * create
 *******************************************************************************/

instrumentr_character_t instrumentr_character_create(instrumentr_state_t state,
                                                     SEXP r_sexp) {
    /* TODO: make foreign for instrumentr character */
    instrumentr_value_t value =
        instrumentr_value_create(state,
                                 sizeof(struct instrumentr_character_impl_t),
                                 INSTRUMENTR_VALUE_TYPE_CHARACTER,
                                 instrumentr_character_finalize,
                                 INSTRUMENTR_ORIGIN_LOCAL,
                                 r_sexp);

    instrumentr_character_t character = (instrumentr_character_t)(value);

    return character;
}

/********************************************************************************
 * interop
 *******************************************************************************/

INSTRUMENTR_VALUE_DEFINE_API(INSTRUMENTR_VALUE_TYPE_CHARACTER,
                             character,
                             character)

int instrumentr_character_get_size(instrumentr_character_t character) {
    return Rf_length(instrumentr_character_get_sexp(character));
}

SEXP r_instrumentr_character_get_size(SEXP r_character) {
    instrumentr_character_t character =
        instrumentr_character_unwrap(r_character);
    int result = instrumentr_character_get_size(character);
    return instrumentr_c_int_to_r_integer(result);
}

instrumentr_char_t
instrumentr_character_get_element(instrumentr_character_t character,
                                  int index) {
    instrumentr_state_t state = instrumentr_character_get_state(character);
    SEXP r_char = STRING_ELT(instrumentr_character_get_sexp(character), index);

    instrumentr_value_t value =
        instrumentr_state_value_table_lookup(state, r_char, 1);

    return instrumentr_value_as_char(value);
}

SEXP r_instrumentr_character_get_element(SEXP r_character, SEXP r_index) {
    instrumentr_character_t character =
        instrumentr_character_unwrap(r_character);
    int index = instrumentr_r_integer_to_c_int(r_index);
    instrumentr_char_t result =
        instrumentr_character_get_element(character, index);
    return instrumentr_char_wrap(result);
}

bool instrumentr_character_is_na(instrumentr_character_t character, int index) {
    instrumentr_char_t result =
        instrumentr_character_get_element(character, index);
    return instrumentr_char_is_na(result);
}

SEXP r_instrumentr_character_is_na(SEXP r_character, SEXP r_index) {
    instrumentr_character_t character =
        instrumentr_character_unwrap(r_character);
    int index = instrumentr_r_integer_to_c_int(r_index);
    bool result = instrumentr_character_is_na(character, index);
    return instrumentr_c_int_to_r_logical(result);
}
