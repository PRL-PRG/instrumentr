#ifndef INSTRUMENTR_TRACE_H
#define INSTRUMENTR_TRACE_H

#include <instrumentr/Rincludes.h>

SEXP r_trace_code(SEXP r_tracer, SEXP r_code, SEXP r_environment);

SEXP r_trace_application_load(SEXP r_tracer, SEXP r_application);

SEXP r_trace_application_unload(SEXP r_tracer, SEXP r_application);

SEXP r_trace_application_attach(SEXP r_tracer, SEXP r_application);

SEXP r_trace_application_detach(SEXP r_tracer, SEXP r_application);

SEXP r_trace_package_load(SEXP r_tracer, SEXP r_application, SEXP r_package);

SEXP r_trace_package_unload(SEXP r_tracer, SEXP r_application, SEXP r_package);

SEXP r_trace_package_attach(SEXP r_tracer, SEXP r_application, SEXP r_package);

SEXP r_trace_package_detach(SEXP r_tracer, SEXP r_application, SEXP r_package);

SEXP r_trace_function_attach(SEXP r_tracer,
                             SEXP r_application,
                             SEXP r_package,
                             SEXP r_function);

SEXP r_trace_function_detach(SEXP r_tracer,
                             SEXP r_application,
                             SEXP r_package,
                             SEXP r_function);

SEXP r_trace_call_entry(SEXP r_tracer,
                        SEXP r_application,
                        SEXP r_package,
                        SEXP r_function,
                        SEXP r_call);

SEXP r_trace_call_exit(SEXP r_tracer,
                       SEXP r_application,
                       SEXP r_package,
                       SEXP r_function,
                       SEXP r_result);

#endif /* INSTRUMENTR_TRACE_H */
