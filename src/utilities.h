#ifndef INSTRUMENTR_UTILITIES_H
#define INSTRUMENTR_UTILITIES_H

#include "Rincludes.h"

void initialize_utilities(SEXP r_package_environment, SEXP r_state_environment);

void finalize_utilities();

SEXP delayed_assign(SEXP variable_sym,
                    SEXP variable_str,
                    SEXP value,
                    SEXP eval_env,
                    SEXP assign_env,
                    SEXP rho);

SEXP create_promise(SEXP value, SEXP eval_env);

#endif /* INSTRUMENTR_UTILITIES_H */
