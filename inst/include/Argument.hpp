#ifndef LIGHTR_ARGUMENT_HPP
#define LIGHTR_ARGUMENT_HPP

#include <string>
#include "Object.hpp"

namespace lightr {

class Argument: public Object {
  public:
    Argument(const std::string& name)
        : Object(), name_(name), evaluated_(false) {
    }

    const std::string& get_name() const {
        return name_;
    }

    bool is_evaluated() const {
        return evaluated_;
    }

    void set_evaluated() {
        evaluated_ = true;
    }

    void set_value(SEXP r_argument_obj) {
        r_argument_obj_ = r_argument_obj;
    }

    SEXP get_value() {
        return r_argument_obj_;
    }

    static void initialize();

    static SEXP get_class();

    static std::shared_ptr<Argument> from_sexp(SEXP r_argument);

    static SEXP to_sexp(std::shared_ptr<Argument> argument);

    static void destroy_sexp(SEXP r_argument);

  private:
    std::string name_;
    bool evaluated_;
    SEXP r_argument_obj_;

    static SEXP class_;
};

using ArgumentSPtr = std::shared_ptr<Argument>;

} // namespace lightr

#endif /* LIGHTR_ARGUMENT_HPP */
