#ifndef INSTRUMENTR_VECTOR_COPY_CALLBACK_HPP
#define INSTRUMENTR_VECTOR_COPY_CALLBACK_HPP

#include "Callback.hpp"

namespace instrumentr {

class VectorCopyCallback: public Callback {
  public:
    VectorCopyCallback(void* function, bool is_r_callback)
        : Callback(Type::VectorCopy, function, is_r_callback) {
    }

    void
    invoke(SEXP r_context, SEXP r_application, SEXP r_input, SEXP r_output);

    static void initialize();

    static void finalize();

    static SEXP get_class();

  private:
    static SEXP class_;
};

using VectorCopyCallbackSPtr = std::shared_ptr<VectorCopyCallback>;

} // namespace instrumentr

#endif /* INSTRUMENTR_VECTOR_COPY_CALLBACK_HPP */
