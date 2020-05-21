#ifndef LIGHTR_FUNCTION_HPP
#define LIGHTR_FUNCTION_HPP

#include <string>
#include <memory>
#include <Rinternals.h>

class Package;

class Function {
  public:
    Function(std::shared_ptr<Package> package,
             const std::string& name,
             int parameter_count)
        : package_(package), name_(name), parameter_count_(parameter_count) {
    }

    std::shared_ptr<Package> get_package() {
        return package_;
    }

    const std::shared_ptr<Package> get_package() const {
        return package_;
    }

    const std::string& get_name() const {
        return name_;
    }

    int get_parameter_count() const {
        return parameter_count_;
    }

    static std::shared_ptr<Function> from_sexp(SEXP r_function);

    static SEXP to_sexp(std::shared_ptr<Function> function);

    static void destroy_sexp(SEXP r_function);

  private:
    std::shared_ptr<Package> package_;
    std::string name_;
    int parameter_count_;
};

using FunctionSPtr = std::shared_ptr<Function>;

#endif /* LIGHTR_FUNCTION_HPP */
