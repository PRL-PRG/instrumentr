#ifndef INSTRUMENTR_CALL_ENTRY_CALLBACK_HPP
#define INSTRUMENTR_CALL_ENTRY_CALLBACK_HPP

#include "Callback.hpp"

namespace instrumentr {

class CallEntryCallback: public Callback {
  public:
    CallEntryCallback(void* function, bool is_r_callback)
        : Callback(Type::CallEntry, function, is_r_callback) {
    }

    void invoke(SEXP r_context,
                SEXP r_application,
                SEXP r_package,
                SEXP r_function,
                SEXP r_call);

    static void initialize();

    static void finalize();

    static SEXP get_class();

  private:
    static SEXP class_;
};

using CallEntryCallbackSPtr = std::shared_ptr<CallEntryCallback>;

} // namespace instrumentr

#endif /* INSTRUMENTR_CALL_ENTRY_CALLBACK_HPP */
