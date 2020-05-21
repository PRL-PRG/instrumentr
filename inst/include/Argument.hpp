#ifndef LIGHTR_ARGUMENT_HPP
#define LIGHTR_ARGUMENT_HPP

#include <string>
#include <memory>
#include <Rinternals.h>

namespace lightr {

class Call;

class Argument {
  public:
    Argument(std::shared_ptr<Call> call, int position, const std::string& name)
        : call_(call), position_(position), name_(name), evaluated_(false) {
    }

    int get_position() {
        return position_;
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

    std::shared_ptr<Call> get_call() const {
        return call_;
    }

    static std::shared_ptr<Argument> from_sexp(SEXP r_argument);

    static SEXP to_sexp(std::shared_ptr<Argument> argument);

    static void destroy_sexp(SEXP r_argument);

  private:
    std::shared_ptr<Call> call_;
    int position_;
    std::string name_;
    bool evaluated_;
};

using ArgumentSPtr = std::shared_ptr<Argument>;

} // namespace lightr

#endif /* LIGHTR_ARGUMENT_HPP */
