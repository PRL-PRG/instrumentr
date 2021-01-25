#ifndef INSTRUMENTR_OBJECT_DUPLICATE_CALLBACK_HPP
#define INSTRUMENTR_OBJECT_DUPLICATE_CALLBACK_HPP

#include "Callback.hpp"

namespace instrumentr {

class ObjectDuplicateCallback: public Callback {
  public:
    ObjectDuplicateCallback(void* function, bool is_r_callback)
        : Callback(Type::ObjectDuplicate, function, is_r_callback) {
    }

    void invoke(SEXP r_context,
                SEXP r_application,
                SEXP r_input,
                SEXP r_output,
                SEXP r_deep);

    static void initialize();

    static void finalize();

    static SEXP get_class();

  private:
    static SEXP class_;
};

using ObjectDuplicateCallbackSPtr = std::shared_ptr<ObjectDuplicateCallback>;

} // namespace instrumentr

#endif /* INSTRUMENTR_OBJECT_DUPLICATE_CALLBACK_HPP */
