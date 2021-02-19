#ifndef INSTRUMENTR_CONTEXT_JUMP_CALLBACK_HPP
#define INSTRUMENTR_CONTEXT_JUMP_CALLBACK_HPP

#include "Callback.hpp"
#include "Application.hpp"

using instrumentr::Application;
using instrumentr::ApplicationSPtr;

namespace instrumentr {

class Context;

using ContextSPtr = std::shared_ptr<Context>;

class ContextJumpCallback: public Callback {
  public:
    ContextJumpCallback(void* function, bool is_r_callback)
        : Callback(Type::ContextJump, function, is_r_callback) {
    }

    void invoke(ContextSPtr context,
                ApplicationSPtr application,
                void* call_context);

    static void initialize();

    static void finalize();

    static SEXP get_class();

  private:
    static SEXP class_;
};

using ContextJumpCallbackSPtr = std::shared_ptr<ContextJumpCallback>;

} // namespace instrumentr

#endif /* INSTRUMENTR_CONTEXT_JUMP_CALLBACK_HPP */
