#include "utilities.h"
#include "r_api.h"

SEXP DelayedAssignSymbol = NULL;
SEXP LightrSymbol = NULL;
SEXP TripleColonSymbol = NULL;
SEXP DotCallSymbol = NULL;
SEXP TemporarySymbol = NULL;
SEXP TemporarySymbolString = NULL;
SEXP StateEnvironment = NULL;
SEXP PackageEnvironment = NULL;

void initialize_utilities(SEXP r_package_environment,
                          SEXP r_state_environment) {

    DelayedAssignSymbol = Rf_install("delayedAssign");

    LightrSymbol = Rf_install("lightr");

    TripleColonSymbol = Rf_install(":::");

    DotCallSymbol = Rf_install(".Call");

    TemporarySymbol = Rf_install("*lightr-tmp*");

    TemporarySymbolString = mkString("*lightr-tmp*");
    R_PreserveObject(TemporarySymbolString);

    PackageEnvironment = r_package_environment;

    StateEnvironment = r_state_environment;
}

SEXP delayed_assign(SEXP variable_sym,
                    SEXP variable_str,
                    SEXP value,
                    SEXP eval_env,
                    SEXP assign_env,
                    SEXP rho) {
    SEXP call = PROTECT(Rf_lang5(
        DelayedAssignSymbol, variable_str, value, eval_env, assign_env));

    Rf_eval(call, rho);

    UNPROTECT(1);

    return Rf_findVarInFrame(assign_env, variable_sym);
}

SEXP create_promise(SEXP value, SEXP eval_env) {
    /* create promise binding */
    SEXP promise = PROTECT(delayed_assign(TemporarySymbol,
                                          TemporarySymbolString,
                                          value,
                                          eval_env,
                                          StateEnvironment,
                                          PackageEnvironment));

    /* mutate promise binding  */
    Rf_setVar(TemporarySymbol, R_NilValue, StateEnvironment);

    UNPROTECT(1);

    return promise;
}
