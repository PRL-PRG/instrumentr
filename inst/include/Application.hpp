#ifndef LIGHTR_APPLICATION_HPP
#define LIGHTR_APPLICATION_HPP

#include <vector>
#include "Object.hpp"
#include "Package.hpp"
#include "CallStack.hpp"

namespace lightr {

class Application: public Object {
  public:
    Application(std::shared_ptr<CallStack> call_stack)
        : Object(), call_stack_(call_stack) {
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

    const std::shared_ptr<CallStack> get_call_stack() const {
        return call_stack_;
    }

    std::shared_ptr<CallStack> get_call_stack() {
        return call_stack_;
    }

    static void initialize();

    static SEXP get_class();

    static std::shared_ptr<Application> from_sexp(SEXP r_application);

    static SEXP to_sexp(std::shared_ptr<Application> application);

    static void destroy_sexp(SEXP r_application);

  private:
    std::vector<std::shared_ptr<Package>> packages_;
    std::shared_ptr<CallStack> call_stack_;

    static SEXP class_;
};

using ApplicationSPtr = std::shared_ptr<Application>;

} // namespace lightr

#endif /* LIGHTR_APPLICATION_HPP */
