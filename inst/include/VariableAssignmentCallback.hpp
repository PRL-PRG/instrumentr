#ifndef INSTRUMENTR_VARIABLE_ASSIGNMENT_CALLBACK_HPP
#define INSTRUMENTR_VARIABLE_ASSIGNMENT_CALLBACK_HPP

#include "Callback.hpp"

namespace instrumentr {

class VariableAssignmentCallback: public Callback {
  public:
    VariableAssignmentCallback(void* function, bool is_r_callback)
        : Callback(Type::VariableAssignment, function, is_r_callback) {
    }

    void invoke(SEXP r_context,
                SEXP r_application,
                SEXP r_variable,
                SEXP r_value,
                SEXP r_rho);

    static void initialize();

    static void finalize();

    static SEXP get_class();

  private:
    static SEXP class_;
};

using VariableAssignmentCallbackSPtr =
    std::shared_ptr<VariableAssignmentCallback>;

} // namespace instrumentr

#endif /* INSTRUMENTR_VARIABLE_ASSIGNMENT_CALLBACK_HPP */
