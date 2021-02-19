#ifndef INSTRUMENTR_GC_UNMARK_CALLBACK_HPP
#define INSTRUMENTR_GC_UNMARK_CALLBACK_HPP

#include "Callback.hpp"
#include "Application.hpp"

namespace instrumentr {

class Context;

using ContextSPtr = std::shared_ptr<Context>;

class GcUnmarkCallback: public Callback {
  public:
    GcUnmarkCallback(void* function, bool is_r_callback)
        : Callback(Type::GcUnmark, function, is_r_callback) {
    }

    void
    invoke(ContextSPtr context, ApplicationSPtr application, SEXP r_object);

    static void initialize();

    static void finalize();

    static SEXP get_class();

  private:
    static SEXP class_;
};

using GcUnmarkCallbackSPtr = std::shared_ptr<GcUnmarkCallback>;

} // namespace instrumentr

#endif /* INSTRUMENTR_GC_UNMARK_CALLBACK_HPP */
