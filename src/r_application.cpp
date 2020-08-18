#include "Application.hpp"
#include "Package.hpp"
#include "r_application.h"

using instrumentr::Application;
using instrumentr::ApplicationSPtr;
using instrumentr::CallStack;
using instrumentr::CallStackSPtr;
using instrumentr::from_sexp;
using instrumentr::Package;
using instrumentr::PackageSPtr;
using instrumentr::to_sexp;

SEXP r_application_create_application(SEXP r_name,
                                      SEXP r_directory,
                                      SEXP r_code,
                                      SEXP r_environment,
                                      SEXP r_frame_position) {
    std::string name = CHAR(asChar(r_name));
    std::string directory = CHAR(asChar(r_directory));
    int frame_position = asInteger(r_frame_position);

    ApplicationSPtr application = std::make_shared<Application>(
        name, directory, r_code, r_environment, frame_position);

    return to_sexp<Application>(application);
}

SEXP r_application_get_name(SEXP r_application) {
    ApplicationSPtr application = from_sexp<Application>(r_application);
    std::string name = application->get_name();
    return mkString(name.c_str());
}

SEXP r_application_get_directory(SEXP r_application) {
    ApplicationSPtr application = from_sexp<Application>(r_application);
    std::string directory = application->get_directory();
    return mkString(directory.c_str());
}

SEXP r_application_get_code(SEXP r_application) {
    ApplicationSPtr application = from_sexp<Application>(r_application);
    SEXP r_code = application->get_code();
    return r_code;
}

SEXP r_application_get_environment(SEXP r_application) {
    ApplicationSPtr application = from_sexp<Application>(r_application);
    SEXP r_environment = application->get_environment();
    return r_environment;
}

SEXP r_application_get_frame_position(SEXP r_application) {
    ApplicationSPtr application = from_sexp<Application>(r_application);
    int frame_position = application->get_frame_position();
    return ScalarInteger(frame_position);
}

SEXP r_application_get_packages(SEXP r_application) {
    ApplicationSPtr application = from_sexp<Application>(r_application);
    const std::vector<PackageSPtr>& packages = application->get_packages();

    int length = packages.size();
    SEXP result = PROTECT(allocVector(VECSXP, length));
    for (int index = 0; index < length; ++index) {
        SET_VECTOR_ELT(result, index, to_sexp<Package>(packages.at(index)));
    }
    UNPROTECT(1);

    return result;
}

SEXP r_application_get_call_stack(SEXP r_application) {
    ApplicationSPtr application = from_sexp<Application>(r_application);
    CallStackSPtr call_stack = application->get_call_stack();
    return to_sexp<CallStack>(call_stack);
}

SEXP r_application_add_package(SEXP r_application, SEXP r_package) {
    ApplicationSPtr application = from_sexp<Application>(r_application);
    PackageSPtr package = from_sexp<Package>(r_package);
    application->add_package(package);
    return r_application;
}
