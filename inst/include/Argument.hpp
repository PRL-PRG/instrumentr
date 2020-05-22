#ifndef LIGHTR_ARGUMENT_HPP
#define LIGHTR_ARGUMENT_HPP

#include <string>
#include "Object.hpp"

namespace lightr {

class Argument: public Object {
  public:
    Argument(const std::string& name, int position)
        : Object(), name_(name), position_(position), evaluated_(false) {
    }

    const std::string& get_name() const {
        return name_;
    }

    int get_position() const {
        return position_;
    }

    bool is_evaluated() const {
        return evaluated_;
    }

    void set_evaluated() {
        evaluated_ = true;
    }

    static void initialize();

    static SEXP get_class();

    static std::shared_ptr<Argument> from_sexp(SEXP r_argument);

    static SEXP to_sexp(std::shared_ptr<Argument> argument);

    static void destroy_sexp(SEXP r_argument);

  private:
    std::string name_;
    int position_;
    bool evaluated_;

    static SEXP class_;
};

using ArgumentSPtr = std::shared_ptr<Argument>;

} // namespace lightr

#endif /* LIGHTR_ARGUMENT_HPP */
