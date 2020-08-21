#ifndef INSTRUMENTR_EVAL_EXIT_CALLBACK_HPP
#define INSTRUMENTR_EVAL_EXIT_CALLBACK_HPP

#include "Callback.hpp"

namespace instrumentr {

class EvalExitCallback: public Callback {
  public:
    EvalExitCallback(void* function, bool is_r_callback)
        : Callback(Type::EvalExit, function, is_r_callback) {
    }

    void invoke(SEXP r_context,
                SEXP r_application,
                SEXP r_expression,
                SEXP r_rho,
                SEXP r_result);

    static void initialize();

    static void finalize();

    static SEXP get_class();

  private:
    static SEXP class_;
};

using EvalExitCallbackSPtr = std::shared_ptr<EvalExitCallback>;

} // namespace instrumentr

#endif /* INSTRUMENTR_EVAL_EXIT_CALLBACK_HPP */
