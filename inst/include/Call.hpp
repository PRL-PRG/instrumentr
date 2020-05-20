#ifndef LIGHTR_CALL_HPP
#define LIGHTR_CALL_HPP

#include <string>

using call_id_t = int;

class Call {
  public:
    Call(const std::string& package_name, const std::string& function_name)
        : call_id_(Call::get_next_call_id_())
        , package_name_(package_name)
        , function_name_(function_name)
        , finished_(false)
        , failed_(false) {
    }

    call_id_t get_call_id() const {
        return call_id_;
    }

    const std::string& get_package_name() const {
        return package_name_;
    }

    const std::string& get_function_name() const {
        return function_name_;
    }

    bool has_finished() const {
        return finished_;
    }

    bool has_failed() const {
        return failed_;
    }

    void set_finished(bool failed = false) {
        finished_ = true;
        failed_ = failed;
    }

  private:
    int call_id_;
    std::string package_name_;
    std::string function_name_;
    bool finished_;
    bool failed_;

    static call_id_t get_next_call_id_();
    static call_id_t call_id_counter_;
};

#endif /* LIGHTR_CALL_HPP */
