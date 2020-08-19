#ifndef INSTRUMENTR_GC_ALLOCATION_CALLBACK_HPP
#define INSTRUMENTR_GC_ALLOCATION_CALLBACK_HPP

#include "Callback.hpp"

namespace instrumentr {

class GcAllocationCallback: public Callback {
  public:
    GcAllocationCallback(void* function, bool is_r_callback)
        : Callback(Type::GcAllocationCallback, function, is_r_callback) {
    }

    void invoke(SEXP r_context, SEXP r_application, SEXP r_object);

    static void initialize();

    static void finalize();

    static SEXP get_class();

  private:
    static SEXP class_;
};

using GcAllocationCallbackSPtr = std::shared_ptr<GcAllocationCallback>;

} // namespace instrumentr

#endif /* INSTRUMENTR_GC_ALLOCATION_CALLBACK_HPP */
