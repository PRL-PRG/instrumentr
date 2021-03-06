#ifndef INSTRUMENTR_CALL_STACK_HPP
#define INSTRUMENTR_CALL_STACK_HPP

#include <vector>
#include "Call.hpp"
#include "Object.hpp"

namespace instrumentr {

class CallStack: public Object {
  public:
    CallStack(): Object() {
    }

    int get_size() const {
        return stack_.size();
    }

    CallSPtr peek_frame(int index) const {
        /* TODO: error if index >= size  */
        int reverse_index = get_size() - 1 - index;
        return stack_.at(reverse_index);
    }

    void push_frame(CallSPtr call) {
        stack_.push_back(call);
    }

    CallSPtr pop_frame() {
        CallSPtr call = stack_.back();
        stack_.pop_back();
        return call;
    }

    static void initialize();

    static void finalize();

    static SEXP get_class();

  private:
    std::vector<CallSPtr> stack_;

    static SEXP class_;
};

using CallStackSPtr = std::shared_ptr<CallStack>;

} // namespace instrumentr

#endif /* INSTRUMENTR_CALL_STACK_HPP */
