#ifndef INSTRUMENTR_MATRIX_COPY_CALLBACK_HPP
#define INSTRUMENTR_MATRIX_COPY_CALLBACK_HPP

#include "Callback.hpp"

namespace instrumentr {

class MatrixCopyCallback: public Callback {
  public:
    MatrixCopyCallback(void* function, bool is_r_callback)
        : Callback(Type::MatrixCopy, function, is_r_callback) {
    }

    void
    invoke(SEXP r_context, SEXP r_application, SEXP r_input, SEXP r_output);

    static void initialize();

    static void finalize();

    static SEXP get_class();

  private:
    static SEXP class_;
};

using MatrixCopyCallbackSPtr = std::shared_ptr<MatrixCopyCallback>;

} // namespace instrumentr

#endif /* INSTRUMENTR_MATRIX_COPY_CALLBACK_HPP */
