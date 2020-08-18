#ifndef INSTRUMENTR_APPLICATION_UNLOAD_CALLBACK_HPP
#define INSTRUMENTR_APPLICATION_UNLOAD_CALLBACK_HPP

#include "Callback.hpp"

namespace instrumentr {

class ApplicationUnloadCallback: public Callback {
  public:
    ApplicationUnloadCallback(void* function, bool is_r_callback)
        : Callback(Type::ApplicationUnloadCallback, function, is_r_callback) {
    }

    void invoke(SEXP r_context, SEXP r_application);

    static void initialize();

    static void finalize();

    static SEXP get_class();

  private:
    static SEXP class_;
};

using ApplicationUnloadCallbackSPtr =
    std::shared_ptr<ApplicationUnloadCallback>;

} // namespace instrumentr

#endif /* INSTRUMENTR_APPLICATION_UNLOAD_CALLBACK_HPP */
