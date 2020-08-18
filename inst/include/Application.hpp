#ifndef INSTRUMENTR_APPLICATION_HPP
#define INSTRUMENTR_APPLICATION_HPP

#include <vector>
#include "Object.hpp"
#include "Package.hpp"
#include "CallStack.hpp"

namespace instrumentr {

class Application: public Object {
  public:
    Application(const std::string& name,
                const std::string& directory,
                SEXP r_code,
                SEXP r_environment,
                int frame_position)
        : Object()
        , name_(name)
        , directory_(directory)
        , r_code_(r_code)
        , r_environment_(r_environment)
        , frame_position_(frame_position)
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

    int get_frame_position() const {
        return frame_position_;
    }

    CallStackSPtr get_call_stack() const {
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

    void remove_package(const PackageSPtr& package) {
        for (int index = packages_.size() - 1; index >= 0; --index) {
            if (packages_[index] == package) {
                packages_.erase(packages_.begin() + index);
                break;
            }
        }
    }

    static void initialize();

    static void finalize();

    static SEXP get_class();

  private:
    std::string name_;
    std::string directory_;
    SEXP r_code_;
    SEXP r_environment_;
    int frame_position_;
    CallStackSPtr call_stack_;
    std::vector<PackageSPtr> packages_;

    static SEXP class_;
};

using ApplicationSPtr = std::shared_ptr<Application>;

} // namespace instrumentr

#endif /* INSTRUMENTR_APPLICATION_HPP */
