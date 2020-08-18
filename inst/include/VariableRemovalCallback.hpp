#ifndef INSTRUMENTR_VARIABLE_REMOVAL_CALLBACK_HPP
#define INSTRUMENTR_VARIABLE_REMOVAL_CALLBACK_HPP

#include "Callback.hpp"

namespace instrumentr {

class VariableRemovalCallback: public Callback {
  public:
    VariableRemovalCallback(void* function, bool is_r_callback)
        : Callback(Type::VariableRemovalCallback, function, is_r_callback) {
    }

    void invoke(SEXP r_context, SEXP r_application, SEXP r_variable, SEXP r_rho);

    static void initialize();

    static void finalize();

    static SEXP get_class();

  private:
    static SEXP class_;
};

using VariableRemovalCallbackSPtr = std::shared_ptr<VariableRemovalCallback>;

} // namespace instrumentr

#endif /* INSTRUMENTR_VARIABLE_REMOVAL_CALLBACK_HPP */
