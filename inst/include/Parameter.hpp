#ifndef LIGHTR_PARAMETER_HPP
#define LIGHTR_PARAMETER_HPP

#include <string>
#include <vector>
#include "Object.hpp"

namespace lightr {

class Argument;

class Parameter: public Object {
  public:
    Parameter(const std::string& name, int position)
        : Object(), name_(name), position_(position), arguments_() {
    }

    Parameter(const std::string& name,
              int position,
              std::shared_ptr<Argument> argument)
        : Parameter(name, position) {
        arguments_.push_back(argument);
    }

    Parameter(const std::string& name,
              int position,
              std::vector<std::shared_ptr<Argument>> arguments)
        : Parameter(name, position) {
        arguments_ = arguments;
    }

    const std::string& get_name() const {
        return name_;
    }

    int get_position() const {
        return position_;
    }

    const std::vector<std::shared_ptr<Argument>>& get_arguments() const {
        return arguments_;
    }

    bool is_missing() {
        return arguments_.size() == 0;
    }

    bool is_vararg() {
        return name_ == "...";
    }

    static void initialize();

    static SEXP get_class();

    static std::shared_ptr<Parameter> from_sexp(SEXP r_parameter);

    static SEXP to_sexp(std::shared_ptr<Parameter> parameter);

    static void destroy_sexp(SEXP r_parameter);

  private:
    const std::string name_;
    const int position_;
    std::vector<std::shared_ptr<Argument>> arguments_;

    static SEXP class_;
};

using ParameterSPtr = std::shared_ptr<Parameter>;

} // namespace lightr

#endif /* LIGHTR_PARAMETER_HPP */
