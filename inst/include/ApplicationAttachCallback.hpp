#ifndef INSTRUMENTR_APPLICATION_ATTACH_CALLBACK_HPP
#define INSTRUMENTR_APPLICATION_ATTACH_CALLBACK_HPP

#include "Callback.hpp"

namespace instrumentr {

class ApplicationAttachCallback: public Callback {
  public:
    ApplicationAttachCallback(void* function, bool is_r_callback)
        : Callback(Type::ApplicationAttach, function, is_r_callback) {
    }

    void invoke(SEXP r_context, SEXP r_application);

    static void initialize();

    static void finalize();

    static SEXP get_class();

  private:
    static SEXP class_;
};

using ApplicationAttachCallbackSPtr =
    std::shared_ptr<ApplicationAttachCallback>;

} // namespace instrumentr

#endif /* INSTRUMENTR_APPLICATION_ATTACH_CALLBACK_HPP */
