#include "Application.hpp"
#include "Package.hpp"
#include "Function.hpp"
#include "r_package.h"

using instrumentr::Function;
using instrumentr::FunctionSPtr;
using instrumentr::Package;
using instrumentr::PackageSPtr;
using instrumentr::from_sexp;
using instrumentr::to_sexp;

SEXP r_package_create_package(SEXP r_package_name,
                              SEXP r_package_directory,
                              SEXP r_package_environment) {
    std::string package_name = CHAR(asChar(r_package_name));
    std::string package_directory = CHAR(asChar(r_package_directory));

    PackageSPtr package = std::make_shared<Package>(
        package_name, package_directory, r_package_environment);

    return to_sexp<Package>(package);
}

SEXP r_package_get_name(SEXP r_package) {
    PackageSPtr package = from_sexp<Package>(r_package);
    const std::string& name = package->get_name();
    return mkString(name.c_str());
}

SEXP r_package_get_directory(SEXP r_package) {
    PackageSPtr package = from_sexp<Package>(r_package);
    const std::string& directory = package->get_directory();
    return mkString(directory.c_str());
}

SEXP r_package_get_environment(SEXP r_package) {
    PackageSPtr package = from_sexp<Package>(r_package);
    SEXP r_environment = package->get_environment();
    return r_environment;
}

SEXP r_package_get_functions(SEXP r_package) {
    PackageSPtr package = from_sexp<Package>(r_package);
    const std::vector<FunctionSPtr>& functions = package->get_functions();

    int length = functions.size();
    SEXP result = PROTECT(allocVector(VECSXP, length));
    for (int index = 0; index < length; ++index) {
        SET_VECTOR_ELT(result, index, to_sexp<Function>(functions.at(index)));
    }
    UNPROTECT(1);

    return result;
}

SEXP r_package_add_function(SEXP r_package, SEXP r_function) {
    PackageSPtr package = from_sexp<Package>(r_package);
    FunctionSPtr function = from_sexp<Function>(r_function);
    package->add_function(function);
    return r_package;
}
