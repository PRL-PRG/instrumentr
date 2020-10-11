#include "utilities.h"

SEXP DelayedAssignSymbol = NULL;
SEXP InstrumentrSymbol = NULL;
SEXP TripleColonSymbol = NULL;
SEXP DotCallSymbol = NULL;
SEXP TemporarySymbol = NULL;
SEXP TemporarySymbolString = NULL;
SEXP StateEnvironment = NULL;
SEXP PackageEnvironment = NULL;

void instrumentr_initialize_utilities(SEXP r_package_environment,
                                      SEXP r_state_environment) {
    DelayedAssignSymbol = Rf_install("delayedAssign");

    InstrumentrSymbol = Rf_install("instrumentr");

    TripleColonSymbol = Rf_install(":::");

    DotCallSymbol = Rf_install(".Call");

    TemporarySymbol = Rf_install("*instrumentr-tmp*");

    TemporarySymbolString = mkString("*instrumentr-tmp*");
    R_PreserveObject(TemporarySymbolString);

    PackageEnvironment = r_package_environment;

    StateEnvironment = r_state_environment;
}

void instrumentr_finalize_utilities() {
    R_ReleaseObject(TemporarySymbolString);
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

SEXP Rf_lang7(SEXP r, SEXP s, SEXP t, SEXP u, SEXP v, SEXP w, SEXP x) {
    PROTECT(r);
    r = LCONS(r, Rf_list6(s, t, u, v, w, x));
    UNPROTECT(1);
    return r;
}

SEXP Rf_lang8(SEXP q, SEXP r, SEXP s, SEXP t, SEXP u, SEXP v, SEXP w, SEXP x) {
    PROTECT(q);
    q = LCONS(q, Rf_list7(r, s, t, u, v, w, x));
    UNPROTECT(1);
    return q;
}

SEXP Rf_list7(SEXP r, SEXP s, SEXP t, SEXP u, SEXP v, SEXP w, SEXP x) {
    PROTECT(r);
    r = CONS(r, Rf_list6(s, t, u, v, w, x));
    UNPROTECT(1);
    return r;
}
