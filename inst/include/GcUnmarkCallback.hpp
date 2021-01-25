#ifndef INSTRUMENTR_GC_UNMARK_CALLBACK_HPP
#define INSTRUMENTR_GC_UNMARK_CALLBACK_HPP

#include "Callback.hpp"

namespace instrumentr {

class GcUnmarkCallback: public Callback {
  public:
    GcUnmarkCallback(void* function, bool is_r_callback)
        : Callback(Type::GcUnmark, function, is_r_callback) {
    }

    void invoke(SEXP r_context, SEXP r_application, SEXP r_object);

    static void initialize();

    static void finalize();

    static SEXP get_class();

  private:
    static SEXP class_;
};

using GcUnmarkCallbackSPtr = std::shared_ptr<GcUnmarkCallback>;

} // namespace instrumentr

#endif /* INSTRUMENTR_GC_UNMARK_CALLBACK_HPP */
