#ifndef LIGHTR_CALL_STACK_HPP
#define LIGHTR_CALL_STACK_HPP

#include <vector>
#include "Call.hpp"

namespace lightr {

class CallStack {
  public:
    CallStack() {
    }

    int get_size() const {
        return stack_.size();
    }

    CallSPtr get_frame(int index) const {
        /* TODO: error if index > size  */
        return stack_.at(index);
    }

    CallSPtr peek_frame(int index) const {
        /* TODO: error if index > size  */
        return stack_.at(index);
    }

    void push_frame(CallSPtr call) {
        stack_.push_back(call);
    }

    void pop_frame() {
        stack_.pop_back();
    }

    static std::shared_ptr<CallStack> from_sexp(SEXP r_call_stack);

    static SEXP to_sexp(std::shared_ptr<CallStack> call_stack);

    static void destroy_sexp(SEXP r_call_stack);

  private:
    std::vector<CallSPtr> stack_;
};

using CallStackSPtr = std::shared_ptr<CallStack>;

CallStackSPtr get_call_stack();
void initialize_call_stack();

} // namespace lightr

#endif /* LIGHTR_CALL_STACK_HPP */
