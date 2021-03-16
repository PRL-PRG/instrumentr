#include "utilities.h"
#include <string.h>
#include <stdlib.h>

SEXP DelayedAssignSymbol = NULL;
SEXP InstrumentrSymbol = NULL;
SEXP TripleColonSymbol = NULL;
SEXP DotCallSymbol = NULL;
SEXP TemporarySymbol = NULL;
SEXP TemporarySymbolString = NULL;
SEXP QuoteSymbol = NULL;
SEXP StateEnvironment = NULL;
SEXP PackageEnvironment = NULL;

#define BUFFER_SIZE 1024

char buffer[BUFFER_SIZE];

void instrumentr_initialize_utilities(SEXP r_package_environment,
                                      SEXP r_state_environment) {
    DelayedAssignSymbol = Rf_install("delayedAssign");

    InstrumentrSymbol = Rf_install("instrumentr");

    TripleColonSymbol = Rf_install(":::");

    DotCallSymbol = Rf_install(".Call");

    TemporarySymbol = Rf_install("*instrumentr-tmp*");

    TemporarySymbolString = mkString("*instrumentr-tmp*");

    QuoteSymbol = Rf_install("quote");

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

SEXP create_environment(SEXP hash, SEXP parent) {
    return Rf_eval(Rf_lang3(Rf_install("new.env"), hash, parent), R_BaseEnv);
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

SEXP Rf_lang9(SEXP p,
              SEXP q,
              SEXP r,
              SEXP s,
              SEXP t,
              SEXP u,
              SEXP v,
              SEXP w,
              SEXP x) {
    PROTECT(p);
    p = LCONS(p, Rf_list8(q, r, s, t, u, v, w, x));
    UNPROTECT(1);
    return p;
}

SEXP Rf_list7(SEXP r, SEXP s, SEXP t, SEXP u, SEXP v, SEXP w, SEXP x) {
    PROTECT(r);
    r = CONS(r, Rf_list6(s, t, u, v, w, x));
    UNPROTECT(1);
    return r;
}

SEXP Rf_list8(SEXP q, SEXP r, SEXP s, SEXP t, SEXP u, SEXP v, SEXP w, SEXP x) {
    PROTECT(q);
    q = CONS(q, Rf_list7(r, s, t, u, v, w, x));
    UNPROTECT(1);
    return q;
}

SEXP instrumentr_sexp_quote(SEXP r_sexp) {
    return Rf_lang2(QuoteSymbol, r_sexp);
}

char* instrumentr_duplicate_string(const char* original) {
    if (original == NULL) {
        return NULL;
    }
    int length = strlen(original) + 1;
    char* duplicate = (char*) malloc(length * sizeof(char));
    strcpy(duplicate, original);
    return duplicate;
}

char* int_to_string(int value) {
    snprintf(buffer, BUFFER_SIZE, "%d", value);
    return buffer;
}

char* address_to_string(void* ptr) {
    snprintf(buffer, BUFFER_SIZE, "%p", ptr);
    return buffer;
}

SEXP create_class(const std::vector<std::string>& class_names) {
    int size = class_names.size();

    SEXP klass = PROTECT(allocVector(STRSXP, size));

    for (int i = 0; i < size; ++i) {
        SET_STRING_ELT(klass, i, mkChar(class_names[i].c_str()));
    }

    UNPROTECT(1);

    return klass;
}
