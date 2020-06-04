#include "../inst/include/Application.hpp"
#include "../inst/include/Package.hpp"
#include "r_api.h"

using lightr::Application;
using lightr::ApplicationSPtr;
using lightr::CallStack;
using lightr::CallStackSPtr;
using lightr::Package;
using lightr::PackageSPtr;

SEXP r_application_create_application(SEXP r_name,
                                      SEXP r_directory,
                                      SEXP r_code,
                                      SEXP r_environment) {
    std::string name = CHAR(asChar(r_name));
    std::string directory = CHAR(asChar(r_directory));

    ApplicationSPtr application =
        std::make_shared<Application>(name, directory, r_code, r_environment);

    return Application::to_sexp(application);
}

SEXP r_application_get_name(SEXP r_application) {
    ApplicationSPtr application = Application::from_sexp(r_application);
    std::string name = application->get_name();
    return mkString(name.c_str());
}

SEXP r_application_get_directory(SEXP r_application) {
    ApplicationSPtr application = Application::from_sexp(r_application);
    std::string directory = application->get_directory();
    return mkString(directory.c_str());
}

SEXP r_application_get_code(SEXP r_application) {
    ApplicationSPtr application = Application::from_sexp(r_application);
    SEXP r_code = application->get_code();
    return r_code;
}

SEXP r_application_get_environment(SEXP r_application) {
    ApplicationSPtr application = Application::from_sexp(r_application);
    SEXP r_environment = application->get_environment();
    return r_environment;
}

SEXP r_application_get_packages(SEXP r_application) {
    ApplicationSPtr application = Application::from_sexp(r_application);
    const std::vector<PackageSPtr>& packages = application->get_packages();

    int length = packages.size();
    SEXP result = PROTECT(allocVector(VECSXP, length));
    for (int index = 0; index < length; ++index) {
        SET_VECTOR_ELT(result, index, Package::to_sexp(packages.at(index)));
    }
    UNPROTECT(1);

    return result;
}

SEXP r_application_get_call_stack(SEXP r_application) {
    ApplicationSPtr application = Application::from_sexp(r_application);
    CallStackSPtr call_stack = application->get_call_stack();
    return CallStack::to_sexp(call_stack);
}

SEXP r_application_add_package(SEXP r_application, SEXP r_package) {
    ApplicationSPtr application = Application::from_sexp(r_application);
    PackageSPtr package = Package::from_sexp(r_package);
    application->add_package(package);
    return r_application;
}
