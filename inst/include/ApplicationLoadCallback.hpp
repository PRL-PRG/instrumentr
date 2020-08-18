#ifndef INSTRUMENTR_APPLICATION_LOAD_CALLBACK_HPP
#define INSTRUMENTR_APPLICATION_LOAD_CALLBACK_HPP

#include "Callback.hpp"

namespace instrumentr {

class ApplicationLoadCallback: public Callback {
  public:
    ApplicationLoadCallback(void* function, bool is_r_callback)
        : Callback(Type::ApplicationLoadCallback, function, is_r_callback) {
    }

    void invoke(SEXP r_context, SEXP r_application);

    static void initialize();

    static void finalize();

    static SEXP get_class();

  private:
    static SEXP class_;
};

using ApplicationLoadCallbackSPtr = std::shared_ptr<ApplicationLoadCallback>;

} // namespace instrumentr

#endif /* INSTRUMENTR_APPLICATION_LOAD_CALLBACK_HPP */
