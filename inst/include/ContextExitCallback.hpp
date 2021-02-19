#ifndef INSTRUMENTR_CONTEXT_EXIT_CALLBACK_HPP
#define INSTRUMENTR_CONTEXT_EXIT_CALLBACK_HPP

#include "Callback.hpp"
#include "Application.hpp"

using instrumentr::Application;
using instrumentr::ApplicationSPtr;

namespace instrumentr {

class Context;

using ContextSPtr = std::shared_ptr<Context>;

class ContextExitCallback: public Callback {
  public:
    ContextExitCallback(void* function, bool is_r_callback)
        : Callback(Type::ContextExit, function, is_r_callback) {
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

using ContextExitCallbackSPtr = std::shared_ptr<ContextExitCallback>;

} // namespace instrumentr

#endif /* INSTRUMENTR_CONTEXT_EXIT_CALLBACK_HPP */
