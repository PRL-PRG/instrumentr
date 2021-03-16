#ifndef INSTRUMENTR_UTILITIES_H
#define INSTRUMENTR_UTILITIES_H

#include <string>
#include <vector>
#include <instrumentr/Rincludes.h>

#ifdef __cplusplus
extern "C" {
#endif

void instrumentr_initialize_utilities(SEXP r_package_environment,
                                      SEXP r_state_environment);

void instrumentr_finalize_utilities();

SEXP delayed_assign(SEXP variable_sym,
                    SEXP variable_str,
                    SEXP value,
                    SEXP eval_env,
                    SEXP assign_env,
                    SEXP rho);

SEXP create_promise(SEXP value, SEXP eval_env);

SEXP create_environment(SEXP hash, SEXP parent);

SEXP instrumentr_sexp_quote(SEXP r_sexp);

SEXP Rf_lang7(SEXP r, SEXP s, SEXP t, SEXP u, SEXP v, SEXP w, SEXP x);

SEXP Rf_lang8(SEXP q, SEXP r, SEXP s, SEXP t, SEXP u, SEXP v, SEXP w, SEXP x);

SEXP Rf_lang9(SEXP p,
              SEXP q,
              SEXP r,
              SEXP s,
              SEXP t,
              SEXP u,
              SEXP v,
              SEXP w,
              SEXP x);

SEXP Rf_list7(SEXP r, SEXP s, SEXP t, SEXP u, SEXP v, SEXP w, SEXP x);

SEXP Rf_list8(SEXP q, SEXP r, SEXP s, SEXP t, SEXP u, SEXP v, SEXP w, SEXP x);

char* instrumentr_duplicate_string(const char* original);

char* int_to_string(int value);

char* address_to_string(void* ptr);

SEXP create_class(const std::vector<std::string>& class_names);

#ifdef __cplusplus
}
#endif

#endif /* INSTRUMENTR_UTILITIES_H */
