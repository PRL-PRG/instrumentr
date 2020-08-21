#ifndef INSTRUMENTR_EVAL_ENTRY_CALLBACK_HPP
#define INSTRUMENTR_EVAL_ENTRY_CALLBACK_HPP

#include "Callback.hpp"

namespace instrumentr {

class EvalEntryCallback: public Callback {
  public:
    EvalEntryCallback(void* function, bool is_r_callback)
        : Callback(Type::EvalEntry, function, is_r_callback) {
    }

    void
    invoke(SEXP r_context, SEXP r_application, SEXP r_expression, SEXP r_rho);

    static void initialize();

    static void finalize();

    static SEXP get_class();

  private:
    static SEXP class_;
};

using EvalEntryCallbackSPtr = std::shared_ptr<EvalEntryCallback>;

} // namespace instrumentr

#endif /* INSTRUMENTR_EVAL_ENTRY_CALLBACK_HPP */
