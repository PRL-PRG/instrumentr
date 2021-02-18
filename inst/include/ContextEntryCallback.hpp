#ifndef INSTRUMENTR_CONTEXT_ENTRY_CALLBACK_HPP
#define INSTRUMENTR_CONTEXT_ENTRY_CALLBACK_HPP

#include "Callback.hpp"

namespace instrumentr {

class ContextEntryCallback: public Callback {
  public:
    ContextEntryCallback(void* function, bool is_r_callback)
        : Callback(Type::ContextEntry, function, is_r_callback) {
    }

    void invoke(SEXP r_context, SEXP r_application, SEXP r_call_context);

    static void initialize();

    static void finalize();

    static SEXP get_class();

  private:
    static SEXP class_;
};

using ContextEntryCallbackSPtr = std::shared_ptr<ContextEntryCallback>;

} // namespace instrumentr

#endif /* INSTRUMENTR_CONTEXT_ENTRY_CALLBACK_HPP */
