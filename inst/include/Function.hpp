#ifndef LIGHTR_FUNCTION_HPP
#define LIGHTR_FUNCTION_HPP

#include <string>
#include "Object.hpp"

namespace lightr {

class Function: public Object {
  public:
    Function(const std::string& name, int parameter_count)
        : Object(), name_(name), parameter_count_(parameter_count) {
    }

    const std::string& get_name() const {
        return name_;
    }

    int get_parameter_count() const {
        return parameter_count_;
    }

    static void initialize();

    static SEXP get_class();

    static std::shared_ptr<Function> from_sexp(SEXP r_function);

    static SEXP to_sexp(std::shared_ptr<Function> function);

    static void destroy_sexp(SEXP r_function);

  private:
    std::string name_;
    int parameter_count_;

    static SEXP class_;
};

using FunctionSPtr = std::shared_ptr<Function>;

} // namespace lightr

#endif /* LIGHTR_FUNCTION_HPP */
