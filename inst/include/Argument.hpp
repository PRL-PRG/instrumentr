#ifndef LIGHTR_ARGUMENT_HPP
#define LIGHTR_ARGUMENT_HPP

#include <string>

class Call;

class Argument {
  public:
    Argument(Call* call, int position, const std::string& name)
        : call_(call), position_(position), name_(name), evaluated_(false) {
    }

    const Call* get_call() const {
        return call_;
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

  private:
    Call* call_;
    int position_;
    std::string name_;
    bool evaluated_;
};

#endif /* LIGHTR_ARGUMENT_HPP */
