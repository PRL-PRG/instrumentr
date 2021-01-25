#ifndef INSTRUMENTR_OBJECT_COERCE_CALLBACK_HPP
#define INSTRUMENTR_OBJECT_COERCE_CALLBACK_HPP

#include "Callback.hpp"

namespace instrumentr {

class ObjectCoerceCallback: public Callback {
  public:
    ObjectCoerceCallback(void* function, bool is_r_callback)
        : Callback(Type::ObjectCoerce, function, is_r_callback) {
    }

    void
    invoke(SEXP r_context, SEXP r_application, SEXP r_input, SEXP r_output);

    static void initialize();

    static void finalize();

    static SEXP get_class();

  private:
    static SEXP class_;
};

using ObjectCoerceCallbackSPtr = std::shared_ptr<ObjectCoerceCallback>;

} // namespace instrumentr

#endif /* INSTRUMENTR_OBJECT_COERCE_CALLBACK_HPP */
