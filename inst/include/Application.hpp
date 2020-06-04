#ifndef LIGHTR_APPLICATION_HPP
#define LIGHTR_APPLICATION_HPP

#include <vector>
#include "Object.hpp"
#include "Package.hpp"
#include "CallStack.hpp"

namespace lightr {

class Application: public Object {
  public:
    Application(const std::string& name,
                const std::string& directory,
                SEXP r_code,
                SEXP r_environment)
        : Object()
        , name_(name)
        , directory_(directory)
        , r_code_(r_code)
        , r_environment_(r_environment)
        , call_stack_(new CallStack()) {
        R_PreserveObject(r_environment_);
        R_PreserveObject(r_code_);
    }

    ~Application() {
        R_ReleaseObject(r_environment_);
        R_ReleaseObject(r_code_);
    }

    const std::string& get_name() const {
        return name_;
    }

    const std::string& get_directory() const {
        return directory_;
    }

    SEXP get_code() {
        return r_code_;
    }

    SEXP get_environment() {
        return r_environment_;
    }

    const CallStackSPtr get_call_stack() const {
        return call_stack_;
    }

    CallStackSPtr get_call_stack() {
        return call_stack_;
    }

    std::vector<PackageSPtr>& get_packages() {
        return packages_;
    }

    const std::vector<PackageSPtr>& get_packages() const {
        return packages_;
    }

    void add_package(PackageSPtr package) {
        packages_.push_back(package);
    }

    static void initialize();

    static void finalize();

    static SEXP get_class();

    static std::shared_ptr<Application> from_sexp(SEXP r_application);

    static SEXP to_sexp(std::shared_ptr<Application> application);

    static void destroy_sexp(SEXP r_application);

  private:
    std::string name_;
    std::string directory_;
    SEXP r_code_;
    SEXP r_environment_;
    CallStackSPtr call_stack_;
    std::vector<PackageSPtr> packages_;

    static SEXP class_;
};

using ApplicationSPtr = std::shared_ptr<Application>;

} // namespace lightr

#endif /* LIGHTR_APPLICATION_HPP */
