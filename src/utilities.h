#ifndef INSTRUMENTR_UTILITIES_H
#define INSTRUMENTR_UTILITIES_H

#include <string>
#include <vector>
#include <instrumentr/Rincludes.h>

#define PP_ARG_N( \
          _1,  _2,  _3,  _4,  _5,  _6,  _7,  _8,  _9, _10, \
         _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, \
         _21, _22, _23, _24, _25, _26, _27, _28, _29, _30, \
         _31, _32, _33, _34, _35, _36, _37, _38, _39, _40, \
         _41, _42, _43, _44, _45, _46, _47, _48, _49, _50, \
         _51, _52, _53, _54, _55, _56, _57, _58, _59, _60, \
         _61, _62, _63, N, ...) N

/* Note 63 is removed */
#define PP_RSEQ_N()                                        \
         62, 61, 60,                                       \
         59, 58, 57, 56, 55, 54, 53, 52, 51, 50,           \
         49, 48, 47, 46, 45, 44, 43, 42, 41, 40,           \
         39, 38, 37, 36, 35, 34, 33, 32, 31, 30,           \
         29, 28, 27, 26, 25, 24, 23, 22, 21, 20,           \
         19, 18, 17, 16, 15, 14, 13, 12, 11, 10,           \
          9,  8,  7,  6,  5,  4,  3,  2,  1,  0

#define PP_NARG_(...)    PP_ARG_N(__VA_ARGS__)

/* Note dummy first argument _ and ##__VA_ARGS__ instead of __VA_ARGS__ */
#define PP_NARG(...)     PP_NARG_(_, ##__VA_ARGS__, PP_RSEQ_N())

// general definition for any function name
#define OVERLOADED_MACRO_NAME_(NAME, n) NAME##_##n
#define OVERLOADED_MACRO_NAME(NAME, n) OVERLOADED_MACRO_NAME_(NAME, n)
#define OVERLOADED_MACRO(NAME, ...) \
    OVERLOADED_MACRO_NAME(NAME, PP_NARG(__VA_ARGS__))(__VA_ARGS__)

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
