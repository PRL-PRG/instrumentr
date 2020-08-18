#ifndef INSTRUMENTR_PACKAGE_DETACH_CALLBACK_HPP
#define INSTRUMENTR_PACKAGE_DETACH_CALLBACK_HPP

#include "Callback.hpp"

namespace instrumentr {

class PackageDetachCallback: public Callback {
  public:
    PackageDetachCallback(void* function, bool is_r_callback)
        : Callback(Type::PackageDetachCallback, function, is_r_callback) {
    }

    void invoke(SEXP r_context, SEXP r_application, SEXP r_package);

    static void initialize();

    static void finalize();

    static SEXP get_class();

  private:
    static SEXP class_;
};

using PackageDetachCallbackSPtr = std::shared_ptr<PackageDetachCallback>;

} // namespace instrumentr

#endif /* INSTRUMENTR_PACKAGE_DETACH_CALLBACK_HPP */
