#ifndef INSTRUMENTR_ARGUMENT_HPP
#define INSTRUMENTR_ARGUMENT_HPP

#include <string>
#include "Object.hpp"

namespace instrumentr {

class Argument: public Object {
  public:
    Argument(const std::string& name, SEXP r_argument)
        : Object(), name_(name), r_argument_(r_argument) {
        R_PreserveObject(r_argument_);
    }

    ~Argument() {
        R_ReleaseObject(r_argument_);
    }

    const std::string& get_name() const {
        return name_;
    }

    bool is_evaluated() const {
        return PRVALUE(r_argument_) != R_UnboundValue;
    }

    SEXP get_argument() {
        return r_argument_;
    }

    SEXP get_expression() {
        return PREXPR(r_argument_);
    }

    SEXP get_result() {
        SEXP promise_result = PRVALUE(r_argument_);

        /* promise is not evaluated  */
        if (promise_result == R_UnboundValue) {
            return get_undefined_object();
        }
        /* promise is evaluated  */
        else {
            return promise_result;
        }
    }

    static void initialize();

    static void finalize();

    static SEXP get_class();

  private:
    std::string name_;
    SEXP r_argument_;

    static SEXP class_;
};

using ArgumentSPtr = std::shared_ptr<Argument>;

} // namespace instrumentr

#endif /* INSTRUMENTR_ARGUMENT_HPP */
