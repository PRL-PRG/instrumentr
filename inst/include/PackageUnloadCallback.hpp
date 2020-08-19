#ifndef INSTRUMENTR_PACKAGE_UNLOAD_CALLBACK_HPP
#define INSTRUMENTR_PACKAGE_UNLOAD_CALLBACK_HPP

#include "Callback.hpp"

namespace instrumentr {

class PackageUnloadCallback: public Callback {
  public:
    PackageUnloadCallback(void* function, bool is_r_callback)
        : Callback(Type::PackageUnload, function, is_r_callback) {
    }

    void invoke(SEXP r_context, SEXP r_application, SEXP r_package);

    static void initialize();

    static void finalize();

    static SEXP get_class();

  private:
    static SEXP class_;
};

using PackageUnloadCallbackSPtr = std::shared_ptr<PackageUnloadCallback>;

} // namespace instrumentr

#endif /* INSTRUMENTR_PACKAGE_UNLOAD_CALLBACK_HPP */
