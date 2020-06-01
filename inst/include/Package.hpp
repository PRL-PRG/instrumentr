#ifndef LIGHTR_PACKAGE_HPP
#define LIGHTR_PACKAGE_HPP

#include <vector>
#include <string>
#include "Object.hpp"
#include "Function.hpp"

namespace lightr {

class Package: public Object {
  public:
    Package(const std::string& name,
            const std::string& directory,
            SEXP environment)
        : Object()
        , name_(name)
        , directory_(directory)
        , environment_(environment) {
    }

    const std::string& get_name() const {
        return name_;
    }

    const std::string& get_directory() const {
        return directory_;
    }

    SEXP get_environment() {
        return environment_;
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

    static void initialize();

    static SEXP get_class();

    static std::shared_ptr<Package> from_sexp(SEXP r_package);

    static SEXP to_sexp(std::shared_ptr<Package> package);

    static void destroy_sexp(SEXP r_package);

  private:
    std::string name_;
    std::string directory_;
    SEXP environment_;
    std::vector<FunctionSPtr> functions_;

    int parameter_count_;

    static SEXP class_;
};

using PackageSPtr = std::shared_ptr<Package>;

} // namespace lightr

#endif /* LIGHTR_PACKAGE_HPP */
