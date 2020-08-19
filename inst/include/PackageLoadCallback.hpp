#ifndef INSTRUMENTR_PACKAGE_LOAD_CALLBACK_HPP
#define INSTRUMENTR_PACKAGE_LOAD_CALLBACK_HPP

#include "Callback.hpp"

namespace instrumentr {

class PackageLoadCallback: public Callback {
  public:
    PackageLoadCallback(void* function, bool is_r_callback)
        : Callback(Type::PackageLoad, function, is_r_callback) {
    }

    void invoke(SEXP r_context, SEXP r_application, SEXP r_package);

    static void initialize();

    static void finalize();

    static SEXP get_class();

  private:
    static SEXP class_;
};

using PackageLoadCallbackSPtr = std::shared_ptr<PackageLoadCallback>;

} // namespace instrumentr

#endif /* INSTRUMENTR_PACKAGE_LOAD_CALLBACK_HPP */
