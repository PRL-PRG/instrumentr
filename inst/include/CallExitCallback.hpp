#ifndef INSTRUMENTR_CALL_EXIT_CALLBACK_HPP
#define INSTRUMENTR_CALL_EXIT_CALLBACK_HPP

#include "Callback.hpp"

namespace instrumentr {

class CallExitCallback: public Callback {
  public:
    CallExitCallback(void* function, bool is_r_callback)
        : Callback(Type::CallExitCallback, function, is_r_callback) {
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

using CallExitCallbackSPtr = std::shared_ptr<CallExitCallback>;

} // namespace instrumentr

#endif /* INSTRUMENTR_CALL_EXIT_CALLBACK_HPP */
