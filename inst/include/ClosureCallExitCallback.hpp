#ifndef INSTRUMENTR_CLOSURE_CALL_EXIT_CALLBACK_HPP
#define INSTRUMENTR_CLOSURE_CALL_EXIT_CALLBACK_HPP

#include "Callback.hpp"

namespace instrumentr {

class ClosureCallExitCallback: public Callback {
  public:
    ClosureCallExitCallback(void* function, bool is_r_callback)
        : Callback(Type::ClosureCallExit, function, is_r_callback) {
    }

    void invoke(SEXP r_context,
                SEXP r_application,
                SEXP r_call,
                SEXP r_op,
                SEXP r_args,
                SEXP r_rho,
                SEXP r_result);

    static void initialize();

    static void finalize();

    static SEXP get_class();

  private:
    static SEXP class_;
};

using ClosureCallExitCallbackSPtr = std::shared_ptr<ClosureCallExitCallback>;

} // namespace instrumentr

#endif /* INSTRUMENTR_CLOSURE_CALL_EXIT_CALLBACK_HPP */
