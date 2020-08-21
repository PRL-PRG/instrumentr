#ifndef INSTRUMENTR_SPECIAL_CALL_ENTRY_CALLBACK_HPP
#define INSTRUMENTR_SPECIAL_CALL_ENTRY_CALLBACK_HPP

#include "Callback.hpp"

namespace instrumentr {

class SpecialCallEntryCallback: public Callback {
  public:
    SpecialCallEntryCallback(void* function, bool is_r_callback)
        : Callback(Type::SpecialCallEntry, function, is_r_callback) {
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

using SpecialCallEntryCallbackSPtr = std::shared_ptr<SpecialCallEntryCallback>;

} // namespace instrumentr

#endif /* INSTRUMENTR_SPECIAL_CALL_ENTRY_CALLBACK_HPP */
