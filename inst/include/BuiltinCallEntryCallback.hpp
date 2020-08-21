#ifndef INSTRUMENTR_BUILTIN_CALL_ENTRY_CALLBACK_HPP
#define INSTRUMENTR_BUILTIN_CALL_ENTRY_CALLBACK_HPP

#include "Callback.hpp"

namespace instrumentr {

class BuiltinCallEntryCallback: public Callback {
  public:
    BuiltinCallEntryCallback(void* function, bool is_r_callback)
        : Callback(Type::BuiltinCallEntry, function, is_r_callback) {
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

using BuiltinCallEntryCallbackSPtr = std::shared_ptr<BuiltinCallEntryCallback>;

} // namespace instrumentr

#endif /* INSTRUMENTR_BUILTIN_CALL_ENTRY_CALLBACK_HPP */
