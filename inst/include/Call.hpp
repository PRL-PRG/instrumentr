#ifndef LIGHTR_CALL_HPP
#define LIGHTR_CALL_HPP

#include <string>
#include <vector>

class Argument;

using call_id_t = int;

class Call {
  public:
    enum class State { ACTIVE, SUCCESS, ERROR };

    Call(const std::string& package_name, const std::string& function_name)
        : id_(Call::get_next_id_())
        , package_name_(package_name)
        , function_name_(function_name)
        , state_(State::ACTIVE) {
    }

    call_id_t get_id() const {
        return id_;
    }

    const std::string& get_package_name() const {
        return package_name_;
    }

    const std::string& get_function_name() const {
        return function_name_;
    }

    State get_state() const {
        return state_;
    }

    void set_state(State state) {
        state_ = state;
    }

    const std::vector<Argument*> get_arguments() const {
        return arguments_;
    }

  private:
    int id_;
    std::string package_name_;
    std::string function_name_;
    State state_;

    std::vector<Argument*> arguments_;

    static call_id_t get_next_id_();
    static call_id_t id_counter_;
};

#endif /* LIGHTR_CALL_HPP */
