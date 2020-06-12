#ifndef INSTRUMENTR_PACKAGE_HPP
#define INSTRUMENTR_PACKAGE_HPP

#include <vector>
#include <string>
#include "Object.hpp"
#include "Function.hpp"

namespace instrumentr {

class Package: public Object {
  public:
    Package(const std::string& name,
            const std::string& directory,
            SEXP r_environment)
        : Object()
        , name_(name)
        , directory_(directory)
        , r_environment_(r_environment) {
        R_PreserveObject(r_environment_);
    }

    ~Package() {
        R_ReleaseObject(r_environment_);
    }

    const std::string& get_name() const {
        return name_;
    }

    const std::string& get_directory() const {
        return directory_;
    }

    SEXP get_environment() {
        return r_environment_;
    }

    std::vector<FunctionSPtr>& get_functions() {
        return functions_;
    }

    const std::vector<FunctionSPtr>& get_functions() const {
        return functions_;
    }

    void add_function(FunctionSPtr function) {
        functions_.push_back(function);
    }

    void remove_function(FunctionSPtr function) {
        for (int index = functions_.size() - 1; index >= 0; --index) {
            if (functions_[index] == function) {
                functions_.erase(functions_.begin() + index);
                break;
            }
        }
    }

    static void initialize();

    static void finalize();

    static SEXP get_class();

    static std::shared_ptr<Package> from_sexp(SEXP r_package);

    static SEXP to_sexp(std::shared_ptr<Package> package);

    static void destroy_sexp(SEXP r_package);

  private:
    std::string name_;
    std::string directory_;
    SEXP r_environment_;
    std::vector<FunctionSPtr> functions_;

    static SEXP class_;
};

using PackageSPtr = std::shared_ptr<Package>;

} // namespace instrumentr

#endif /* INSTRUMENTR_PACKAGE_HPP */
