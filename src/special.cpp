#include "special.h"
#include "interop.h"
#include "utilities.h"
#include "funtab.h"
#include "state.h"

/********************************************************************************
 * definition
 *******************************************************************************/

struct instrumentr_special_impl_t {
    struct instrumentr_model_impl_t model;
    SEXP r_sexp;
    int funtab_index;
};

/********************************************************************************
 * finalize
 *******************************************************************************/

void instrumentr_special_finalize(instrumentr_model_t model) {
    instrumentr_special_t special = (instrumentr_special_t)(model);
    special->r_sexp = NULL;
    special->funtab_index = -1;
}

/********************************************************************************
 * create
 *******************************************************************************/

instrumentr_special_t instrumentr_special_create(instrumentr_state_t state,
                                                 SEXP r_sexp) {
    instrumentr_model_t model =
        instrumentr_model_create(state,
                                 sizeof(struct instrumentr_special_impl_t),
                                 INSTRUMENTR_MODEL_TYPE_SPECIAL,
                                 instrumentr_special_finalize,
                                 INSTRUMENTR_ORIGIN_FOREIGN);

    instrumentr_special_t special = (instrumentr_special_t)(model);
    special->r_sexp = r_sexp;
    special->funtab_index = instrumentr_funtab_get_index(r_sexp);
    return special;
}

/********************************************************************************
 * interop
 *******************************************************************************/

INSTRUMENTR_MODEL_INTEROP_DEFINE_API(special, INSTRUMENTR_MODEL_TYPE_SPECIAL)

/********************************************************************************
 * name
 *******************************************************************************/

/* accessor  */
const char* instrumentr_special_get_name(instrumentr_special_t special) {
    return instrumentr_funtab_get_name(special->funtab_index);
}

SEXP r_instrumentr_special_get_name(SEXP r_special) {
    instrumentr_special_t special = instrumentr_special_unwrap(r_special);
    const char* name = instrumentr_special_get_name(special);
    return instrumentr_c_string_to_r_character(name);
}

/********************************************************************************
 * definition
 *******************************************************************************/

/* accessor  */
SEXP instrumentr_special_get_sexp(instrumentr_special_t special) {
    return special->r_sexp;
}

SEXP r_instrumentr_special_get_sexp(SEXP r_special) {
    instrumentr_special_t special = instrumentr_special_unwrap(r_special);
    return instrumentr_special_get_sexp(special);
}

/********************************************************************************
 * parameter_count
 *******************************************************************************/

/* accessor  */
int instrumentr_special_get_parameter_count(instrumentr_special_t special) {
    return instrumentr_funtab_get_parameter_count(special->funtab_index);
}

SEXP r_instrumentr_special_get_parameter_count(SEXP r_special) {
    instrumentr_special_t special = instrumentr_special_unwrap(r_special);
    int result = instrumentr_special_get_parameter_count(special);
    return instrumentr_c_int_to_r_integer(result);
}

/********************************************************************************
 * internal
 *******************************************************************************/

/* accessor  */
int instrumentr_special_is_internal(instrumentr_special_t special) {
    return instrumentr_funtab_is_internal(special->funtab_index);
}

SEXP r_instrumentr_special_is_internal(SEXP r_special) {
    instrumentr_special_t special = instrumentr_special_unwrap(r_special);
    int result = instrumentr_special_is_internal(special);
    return instrumentr_c_int_to_r_logical(result);
}

/********************************************************************************
 * primitive
 *******************************************************************************/

/* accessor  */
int instrumentr_special_is_primitive(instrumentr_special_t special) {
    return instrumentr_funtab_is_primitive(special->funtab_index);
}

SEXP r_instrumentr_special_is_primitive(SEXP r_special) {
    instrumentr_special_t special = instrumentr_special_unwrap(r_special);
    int result = instrumentr_special_is_primitive(special);
    return instrumentr_c_int_to_r_logical(result);
}
