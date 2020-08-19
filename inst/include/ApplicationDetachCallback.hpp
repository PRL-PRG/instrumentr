#ifndef INSTRUMENTR_APPLICATION_DETACH_CALLBACK_HPP
#define INSTRUMENTR_APPLICATION_DETACH_CALLBACK_HPP

#include "Callback.hpp"

namespace instrumentr {

class ApplicationDetachCallback: public Callback {
  public:
    ApplicationDetachCallback(void* function, bool is_r_callback)
        : Callback(Type::ApplicationDetach, function, is_r_callback) {
    }

    void invoke(SEXP r_context, SEXP r_application);

    static void initialize();

    static void finalize();

    static SEXP get_class();

  private:
    static SEXP class_;
};

using ApplicationDetachCallbackSPtr =
    std::shared_ptr<ApplicationDetachCallback>;

} // namespace instrumentr

#endif /* INSTRUMENTR_APPLICATION_DETACH_CALLBACK_HPP */
