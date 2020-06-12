#include <R.h>
#include <R_ext/Error.h>
#include <Rdefines.h>

extern "C" {
SEXP r_context_create_context(SEXP r_environment);
SEXP r_context_set_application_load_callback(SEXP r_context,
                                             SEXP r_application_load_callback);
SEXP r_context_get_application_load_callback(SEXP r_context);
SEXP r_context_has_application_load_callback(SEXP r_context);
SEXP r_context_set_application_unload_callback(
    SEXP r_context,
    SEXP r_application_unload_callback);
SEXP r_context_get_application_unload_callback(SEXP r_context);
SEXP r_context_has_application_unload_callback(SEXP r_context);
SEXP r_context_set_application_attach_callback(
    SEXP r_context,
    SEXP r_application_attach_callback);
SEXP r_context_get_application_attach_callback(SEXP r_context);
SEXP r_context_has_application_attach_callback(SEXP r_context);
SEXP r_context_set_application_detach_callback(
    SEXP r_context,
    SEXP r_application_detach_callback);
SEXP r_context_get_application_detach_callback(SEXP r_context);
SEXP r_context_has_application_detach_callback(SEXP r_context);
SEXP r_context_set_package_load_callback(SEXP r_context,
                                         SEXP r_package_load_callback);
SEXP r_context_get_package_load_callback(SEXP r_context);
SEXP r_context_has_package_load_callback(SEXP r_context);
SEXP r_context_set_package_unload_callback(SEXP r_context,
                                           SEXP r_package_unload_callback);
SEXP r_context_get_package_unload_callback(SEXP r_context);
SEXP r_context_has_package_unload_callback(SEXP r_context);
SEXP r_context_set_package_attach_callback(SEXP r_context,
                                           SEXP r_package_attach_callback);
SEXP r_context_get_package_attach_callback(SEXP r_context);
SEXP r_context_has_package_attach_callback(SEXP r_context);
SEXP r_context_set_package_detach_callback(SEXP r_context,
                                           SEXP r_package_detach_callback);
SEXP r_context_get_package_detach_callback(SEXP r_context);
SEXP r_context_has_package_detach_callback(SEXP r_context);
SEXP r_context_set_function_attach_callback(SEXP r_context,
                                            SEXP r_function_attach_callback);
SEXP r_context_get_function_attach_callback(SEXP r_context);
SEXP r_context_has_function_attach_callback(SEXP r_context);
SEXP r_context_set_function_detach_callback(SEXP r_context,
                                            SEXP r_function_detach_callback);
SEXP r_context_get_function_detach_callback(SEXP r_context);
SEXP r_context_has_function_detach_callback(SEXP r_context);
SEXP r_context_set_call_entry_callback(SEXP r_context,
                                       SEXP r_call_entry_callback);
SEXP r_context_get_call_entry_callback(SEXP r_context);
SEXP r_context_has_call_entry_callback(SEXP r_context);
SEXP r_context_set_call_exit_callback(SEXP r_context,
                                      SEXP r_call_exit_callback);
SEXP r_context_get_call_exit_callback(SEXP r_context);
SEXP r_context_has_call_exit_callback(SEXP r_context);
SEXP r_context_set_environment(SEXP r_context, SEXP r_environment);
SEXP r_context_get_environment(SEXP r_context);
SEXP r_context_trace_package(SEXP r_context, SEXP r_package_name);
SEXP r_context_trace_function(SEXP r_context,
                              SEXP r_package_name,
                              SEXP r_function_name);
SEXP r_context_get_traced_packages(SEXP r_context);
SEXP r_context_get_traced_functions(SEXP r_context, SEXP r_package_name);
SEXP r_context_is_package_traced(SEXP r_context, SEXP r_package_name);
SEXP r_context_is_function_traced(SEXP r_context,
                                  SEXP r_package_name,
                                  SEXP r_function_name);
}
