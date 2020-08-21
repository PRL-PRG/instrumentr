#ifndef INSTRUMENTR_CLOSURE_CALL_ENTRY_CALLBACK_HPP
#define INSTRUMENTR_CLOSURE_CALL_ENTRY_CALLBACK_HPP

#include "Callback.hpp"

namespace instrumentr {

class ClosureCallEntryCallback: public Callback {
  public:
    ClosureCallEntryCallback(void* function, bool is_r_callback)
        : Callback(Type::ClosureCallEntry, function, is_r_callback) {
    }

    void invoke(SEXP r_context,
                SEXP r_application,
                SEXP r_call,
                SEXP r_op,
                SEXP r_args,
                SEXP r_rho);

    static void initialize();

    static void finalize();

    static SEXP get_class();

  private:
    static SEXP class_;
};

using ClosureCallEntryCallbackSPtr = std::shared_ptr<ClosureCallEntryCallback>;

} // namespace instrumentr

#endif /* INSTRUMENTR_CLOSURE_CALL_ENTRY_CALLBACK_HPP */
