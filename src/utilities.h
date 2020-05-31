#ifndef LIGHTR_UTILITIES_H
#define LIGHTR_UTILITIES_H

#include <Rinternals.h>

SEXP delayed_assign(SEXP variable_sym,
                    SEXP variable_str,
                    SEXP value,
                    SEXP eval_env,
                    SEXP assign_env,
                    SEXP rho);

SEXP create_promise(SEXP value, SEXP eval_env);

#endif /* LIGHTR_UTILITIES_H */
