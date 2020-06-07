#ifndef LIGHTR_UTILITIES_H
#define LIGHTR_UTILITIES_H

#include <Rinternals.h>

extern SEXP FunctionEntryCallbackSymbol;
extern SEXP FunctionExitCallbackSymbol;
extern SEXP CallEntryCallbackSymbol;
extern SEXP CallExitCallbackSymbol;

void initialize_utilities(SEXP r_package_environment, SEXP r_state_environment);

SEXP delayed_assign(SEXP variable_sym,
                    SEXP variable_str,
                    SEXP value,
                    SEXP eval_env,
                    SEXP assign_env,
                    SEXP rho);

SEXP create_promise(SEXP value, SEXP eval_env);

#endif /* LIGHTR_UTILITIES_H */
