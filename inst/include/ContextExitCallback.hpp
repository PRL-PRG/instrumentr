#ifndef INSTRUMENTR_CONTEXT_EXIT_CALLBACK_HPP
#define INSTRUMENTR_CONTEXT_EXIT_CALLBACK_HPP

#include "Callback.hpp"

namespace instrumentr {

class ContextExitCallback: public Callback {
  public:
    ContextExitCallback(void* function, bool is_r_callback)
        : Callback(Type::ContextExit, function, is_r_callback) {
    }

    void invoke(SEXP r_context, SEXP r_application, SEXP r_call_context);

    static void initialize();

    static void finalize();

    static SEXP get_class();

  private:
    static SEXP class_;
};

using ContextExitCallbackSPtr = std::shared_ptr<ContextExitCallback>;

} // namespace instrumentr

#endif /* INSTRUMENTR_CONTEXT_EXIT_CALLBACK_HPP */
