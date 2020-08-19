#ifndef INSTRUMENTR_FUNCTION_ATTACH_CALLBACK_HPP
#define INSTRUMENTR_FUNCTION_ATTACH_CALLBACK_HPP

#include "Callback.hpp"

namespace instrumentr {

class FunctionAttachCallback: public Callback {
  public:
    FunctionAttachCallback(void* function, bool is_r_callback)
        : Callback(Type::FunctionAttach, function, is_r_callback) {
    }

    void
    invoke(SEXP r_context, SEXP r_application, SEXP r_package, SEXP r_function);

    static void initialize();

    static void finalize();

    static SEXP get_class();

  private:
    static SEXP class_;
};

using FunctionAttachCallbackSPtr = std::shared_ptr<FunctionAttachCallback>;

} // namespace instrumentr

#endif /* INSTRUMENTR_FUNCTION_ATTACH_CALLBACK_HPP */
