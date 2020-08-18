#ifndef INSTRUMENTR_FUNCTION_DETACH_CALLBACK_HPP
#define INSTRUMENTR_FUNCTION_DETACH_CALLBACK_HPP

#include "Callback.hpp"

namespace instrumentr {

class FunctionDetachCallback: public Callback {
  public:
    FunctionDetachCallback(void* function, bool is_r_callback)
        : Callback(Type::FunctionDetachCallback, function, is_r_callback) {
    }

    void
    invoke(SEXP r_context, SEXP r_application, SEXP r_package, SEXP r_function);

    static void initialize();

    static void finalize();

    static SEXP get_class();

  private:
    static SEXP class_;
};

using FunctionDetachCallbackSPtr = std::shared_ptr<FunctionDetachCallback>;

} // namespace instrumentr

#endif /* INSTRUMENTR_FUNCTION_DETACH_CALLBACK_HPP */
