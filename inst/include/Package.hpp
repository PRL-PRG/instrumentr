#ifndef LIGHTR_PACKAGE_HPP
#define LIGHTR_PACKAGE_HPP

#include <vector>
#include <string>
#include "Object.hpp"

namespace lightr {

class Application;
class Function;

class Package: public Object {
  public:
    Package(std::shared_ptr<Application> application, const std::string& name)
        : Object(), application_(application), name_(name) {
    }

    std::shared_ptr<Application> get_application() {
        return application_;
    }

    const std::shared_ptr<Application> get_application() const {
        return application_;
    }

    const std::string& get_name() const {
        return name_;
    }

    std::vector<std::shared_ptr<Function>>& get_functions() {
        return functions_;
    }

    const std::vector<std::shared_ptr<Function>>& get_functions() const {
        return functions_;
    }

    void add_function(std::shared_ptr<Function> function) {
        functions_.push_back(function);
    }

    static void initialize();

    static SEXP get_class();

    static std::shared_ptr<Package> from_sexp(SEXP r_package);

    static SEXP to_sexp(std::shared_ptr<Package> package);

    static void destroy_sexp(SEXP r_package);

  private:
    std::shared_ptr<Application> application_;
    std::string name_;
    std::vector<std::shared_ptr<Function>> functions_;

    int parameter_count_;

    static SEXP class_;
};

using PackageSPtr = std::shared_ptr<Package>;

} // namespace lightr

#endif /* LIGHTR_PACKAGE_HPP */
