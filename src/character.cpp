#include "interop.h"
#include "utilities.h"
#include "character.h"
#include "state.h"
#include "char.h"

/********************************************************************************
 * definition
 *******************************************************************************/
struct instrumentr_character_impl_t {
    struct instrumentr_model_impl_t model;
    SEXP r_sexp;
};

/********************************************************************************
 * finalize
 *******************************************************************************/

void instrumentr_character_finalize(instrumentr_model_t model) {
    instrumentr_character_t character = (instrumentr_character_t)(model);

    character->r_sexp = NULL;
}

/********************************************************************************
 * create
 *******************************************************************************/

instrumentr_character_t instrumentr_character_create(instrumentr_state_t state,
                                                     SEXP r_sexp) {
    /* TODO: make foreign for instrumentr character */
    instrumentr_model_t model =
        instrumentr_model_create(state,
                                 sizeof(struct instrumentr_character_impl_t),
                                 INSTRUMENTR_MODEL_TYPE_CHARACTER,
                                 instrumentr_character_finalize,
                                 INSTRUMENTR_ORIGIN_LOCAL);

    instrumentr_character_t character = (instrumentr_character_t)(model);

    character->r_sexp = r_sexp;

    return character;
}

/********************************************************************************
 * interop
 *******************************************************************************/

INSTRUMENTR_MODEL_INTEROP_DEFINE_API(character,
                                     INSTRUMENTR_MODEL_TYPE_CHARACTER)

SEXP instrumentr_character_get_sexp(instrumentr_character_t character) {
    return character->r_sexp;
}

SEXP r_instrumentr_character_get_sexp(SEXP r_character) {
    instrumentr_character_t character =
        instrumentr_character_unwrap(r_character);
    return instrumentr_character_get_sexp(character);
}

int instrumentr_character_get_size(instrumentr_character_t character) {
    return Rf_length(character->r_sexp);
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
    return instrumentr_state_char_table_lookup(
        instrumentr_model_get_state(character),
        STRING_ELT(character->r_sexp, index),
        1);
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
