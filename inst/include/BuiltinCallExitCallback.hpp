#ifndef INSTRUMENTR_BUILTIN_CALL_EXIT_CALLBACK_HPP
#define INSTRUMENTR_BUILTIN_CALL_EXIT_CALLBACK_HPP

#include "Callback.hpp"

namespace instrumentr {

class BuiltinCallExitCallback: public Callback {
  public:
    BuiltinCallExitCallback(void* function, bool is_r_callback)
        : Callback(Type::BuiltinCallExit, function, is_r_callback) {
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

using BuiltinCallExitCallbackSPtr = std::shared_ptr<BuiltinCallExitCallback>;

} // namespace instrumentr

#endif /* INSTRUMENTR_BUILTIN_CALL_EXIT_CALLBACK_HPP */
