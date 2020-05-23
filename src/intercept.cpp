#include "r_api.h"
#include "../inst/include/lightr.hpp"
#include <iostream>
#include "../inst/include/Package.hpp"
#include "../inst/include/Function.hpp"
#include "../inst/include/Call.hpp"

bool interception_enabled = true;
int indentation = 0;

using lightr::ApplicationSPtr;
using lightr::Call;
using lightr::CallSPtr;
using lightr::CallStackSPtr;
using lightr::Function;
using lightr::FunctionSPtr;
using lightr::Package;
using lightr::PackageSPtr;

SEXP r_lightr_intercept_application_entry(SEXP r_environment) {
    std::cerr << "Application entry" << std::endl;
    return R_NilValue;
}

SEXP r_lightr_intercept_application_exit(SEXP r_environment) {
    std::cerr << "Application exit" << std::endl;
    return R_NilValue;
}

SEXP r_lightr_intercept_package_entry(SEXP r_package) {
    std::cerr << "Package entry '" << Package::from_sexp(r_package)->get_name()
              << "'" << std::endl;
    return R_NilValue;
}

SEXP r_lightr_intercept_package_exit(SEXP r_package) {
    std::cerr << "Package exit '" << Package::from_sexp(r_package)->get_name()
              << "'" << std::endl;
    return R_NilValue;
}

SEXP r_lightr_intercept_call_entry(SEXP r_package,
                                   SEXP r_function,
                                   SEXP r_func,
                                   SEXP r_environment) {
    PackageSPtr package = Package::from_sexp(r_package);
    FunctionSPtr function = Function::from_sexp(r_function);
    CallSPtr call = std::make_shared<Call>(function);
    ApplicationSPtr application = lightr::get_application();
    CallStackSPtr call_stack = application->get_call_stack();

    for (int i = 0; i < indentation; ++i) {
        // std::cerr << " ";
    }

    // std::cerr << "+" << package->get_name() << "::" << function->get_name()
    //        << std::endl;
    ++indentation;

    call_stack->push_frame(call);

    return R_NilValue;
}

SEXP r_lightr_intercept_call_exit(SEXP r_package,
                                  SEXP r_function,
                                  SEXP result,
                                  SEXP failed) {
    PackageSPtr package = Package::from_sexp(r_package);
    FunctionSPtr function = Function::from_sexp(r_function);
    ApplicationSPtr application = lightr::get_application();
    CallStackSPtr call_stack = application->get_call_stack();
    CallSPtr call = call_stack->pop_frame();

    --indentation;
    for (int i = 0; i < indentation; ++i) {
        // std::cerr << " ";
    }

    // std::cerr << "-" << package->get_name() << "::" << function->get_name()
    //<< std::endl;

    if (call->get_function()->get_name() != function->get_name()) {
        std::cerr << "************** ERROR ***************" << std::endl;
        std::cerr << "Expected " << package->get_name()
                  << "::" << function->get_name() << " ";
        std::cerr << "Obtained " << call->get_function()->get_name()
                  << std::endl;
        exit(1);
    }

    return R_NilValue;
}

SEXP r_lightr_interception_is_enabled() {
    return ScalarLogical(interception_enabled);
}

SEXP r_lightr_disable_interception() {
    interception_enabled = false;
    return R_NilValue;
}

SEXP r_lightr_enable_interception() {
    interception_enabled = true;
    return R_NilValue;
}
