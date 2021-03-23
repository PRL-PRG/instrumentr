#include "builtin.h"
#include "interop.h"
#include "utilities.h"
#include "funtab.h"
#include "state.h"

/********************************************************************************
 * definition
 *******************************************************************************/

struct instrumentr_builtin_impl_t {
    struct instrumentr_value_impl_t value;
    int funtab_index;
};

/********************************************************************************
 * finalize
 *******************************************************************************/

void instrumentr_builtin_finalize(instrumentr_value_t value) {
    instrumentr_builtin_t builtin = (instrumentr_builtin_t)(value);
    builtin->funtab_index = -1;
}

/********************************************************************************
 * create
 *******************************************************************************/

instrumentr_builtin_t instrumentr_builtin_create(instrumentr_state_t state,
                                                 SEXP r_sexp) {
    instrumentr_value_t value =
        instrumentr_value_create(state,
                                 sizeof(struct instrumentr_builtin_impl_t),
                                 INSTRUMENTR_VALUE_TYPE_BUILTIN,
                                 instrumentr_builtin_finalize,
                                 INSTRUMENTR_ORIGIN_FOREIGN,
                                 r_sexp);

    instrumentr_builtin_t builtin = (instrumentr_builtin_t)(value);
    builtin->funtab_index = instrumentr_funtab_get_index(r_sexp);
    return builtin;
}

/********************************************************************************
 * interop
 *******************************************************************************/

INSTRUMENTR_VALUE_DEFINE_API(INSTRUMENTR_VALUE_TYPE_BUILTIN, builtin, builtin)

/********************************************************************************
 * name
 *******************************************************************************/

/* accessor  */
const char* instrumentr_builtin_get_name(instrumentr_builtin_t builtin) {
    return instrumentr_funtab_get_name(builtin->funtab_index);
}

SEXP r_instrumentr_builtin_get_name(SEXP r_builtin) {
    instrumentr_builtin_t builtin = instrumentr_builtin_unwrap(r_builtin);
    const char* name = instrumentr_builtin_get_name(builtin);
    return instrumentr_c_string_to_r_character(name);
}

/********************************************************************************
 * parameter_count
 *******************************************************************************/

/* accessor  */
int instrumentr_builtin_get_parameter_count(instrumentr_builtin_t builtin) {
    return instrumentr_funtab_get_parameter_count(builtin->funtab_index);
}

SEXP r_instrumentr_builtin_get_parameter_count(SEXP r_builtin) {
    instrumentr_builtin_t builtin = instrumentr_builtin_unwrap(r_builtin);
    int result = instrumentr_builtin_get_parameter_count(builtin);
    return instrumentr_c_int_to_r_integer(result);
}

/********************************************************************************
 * internal
 *******************************************************************************/

/* accessor  */
int instrumentr_builtin_is_internal(instrumentr_builtin_t builtin) {
    return instrumentr_funtab_is_internal(builtin->funtab_index);
}

SEXP r_instrumentr_builtin_is_internal(SEXP r_builtin) {
    instrumentr_builtin_t builtin = instrumentr_builtin_unwrap(r_builtin);
    int result = instrumentr_builtin_is_internal(builtin);
    return instrumentr_c_int_to_r_logical(result);
}

/********************************************************************************
 * primitive
 *******************************************************************************/

/* accessor  */
int instrumentr_builtin_is_primitive(instrumentr_builtin_t builtin) {
    return instrumentr_funtab_is_primitive(builtin->funtab_index);
}

SEXP r_instrumentr_builtin_is_primitive(SEXP r_builtin) {
    instrumentr_builtin_t builtin = instrumentr_builtin_unwrap(r_builtin);
    int result = instrumentr_builtin_is_primitive(builtin);
    return instrumentr_c_int_to_r_logical(result);
}
