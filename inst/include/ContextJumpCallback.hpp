#ifndef INSTRUMENTR_CONTEXT_JUMP_CALLBACK_HPP
#define INSTRUMENTR_CONTEXT_JUMP_CALLBACK_HPP

#include "Callback.hpp"

namespace instrumentr {

class ContextJumpCallback: public Callback {
  public:
    ContextJumpCallback(void* function, bool is_r_callback)
        : Callback(Type::ContextJump, function, is_r_callback) {
    }

    void invoke(SEXP r_context, SEXP r_application, SEXP r_call_context);

    static void initialize();

    static void finalize();

    static SEXP get_class();

  private:
    static SEXP class_;
};

using ContextJumpCallbackSPtr = std::shared_ptr<ContextJumpCallback>;

} // namespace instrumentr

#endif /* INSTRUMENTR_CONTEXT_JUMP_CALLBACK_HPP */
