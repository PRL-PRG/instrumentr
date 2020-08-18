#ifndef INSTRUMENTR_PACKAGE_ATTACH_CALLBACK_HPP
#define INSTRUMENTR_PACKAGE_ATTACH_CALLBACK_HPP

#include "Callback.hpp"

namespace instrumentr {

class PackageAttachCallback: public Callback {
  public:
    PackageAttachCallback(void* function, bool is_r_callback)
        : Callback(Type::PackageAttachCallback, function, is_r_callback) {
    }

    void invoke(SEXP r_context, SEXP r_application, SEXP r_package);

    static void initialize();

    static void finalize();

    static SEXP get_class();

  private:
    static SEXP class_;
};

using PackageAttachCallbackSPtr = std::shared_ptr<PackageAttachCallback>;

} // namespace instrumentr

#endif /* INSTRUMENTR_PACKAGE_ATTACH_CALLBACK_HPP */
