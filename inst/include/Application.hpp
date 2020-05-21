#ifndef LIGHTR_APPLICATION_HPP
#define LIGHTR_APPLICATION_HPP

#include <vector>
#include "Object.hpp"

namespace lightr {

class Package;

class Application: public Object {
  public:
    Application(): Object() {
    }

    std::vector<std::shared_ptr<Package>>& get_packages() {
        return packages_;
    }

    const std::vector<std::shared_ptr<Package>>& get_packages() const {
        return packages_;
    }

    void add_package(std::shared_ptr<Package> package) {
        packages_.push_back(package);
    }

    static void initialize();

    static SEXP get_class();

    static std::shared_ptr<Application> from_sexp(SEXP r_application);

    static SEXP to_sexp(std::shared_ptr<Application> application);

    static void destroy_sexp(SEXP r_application);

  private:
    std::vector<std::shared_ptr<Package>> packages_;

    static SEXP class_;
};

using ApplicationSPtr = std::shared_ptr<Application>;

} // namespace lightr

#endif /* LIGHTR_APPLICATION_HPP */
