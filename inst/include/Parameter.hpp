#ifndef INSTRUMENTR_PARAMETER_HPP
#define INSTRUMENTR_PARAMETER_HPP

#include <string>
#include <vector>
#include "Object.hpp"
#include "Argument.hpp"

namespace instrumentr {

class Parameter: public Object {
  public:
    Parameter(const std::string& name, int position)
        : Object(), name_(name), position_(position), arguments_() {
    }

    const std::string& get_name() const {
        return name_;
    }

    int get_position() const {
        return position_;
    }

    const std::vector<ArgumentSPtr>& get_arguments() const {
        return arguments_;
    }

    std::vector<ArgumentSPtr>& get_arguments() {
        return arguments_;
    }

    void add_argument(ArgumentSPtr argument) {
        arguments_.push_back(argument);
    }

    bool is_missing() const {
        return arguments_.size() == 0;
    }

    bool is_vararg() const {
        return name_ == "...";
    }

    static void initialize();

    static void finalize();

    static SEXP get_class();

  private:
    const std::string name_;
    const int position_;
    std::vector<std::shared_ptr<Argument>> arguments_;

    static SEXP class_;
};

using ParameterSPtr = std::shared_ptr<Parameter>;

} // namespace instrumentr

#endif /* INSTRUMENTR_PARAMETER_HPP */
