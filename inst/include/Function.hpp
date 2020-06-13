#ifndef INSTRUMENTR_FUNCTION_HPP
#define INSTRUMENTR_FUNCTION_HPP

#include <string>
#include "Object.hpp"

namespace instrumentr {

class Function: public Object {
  public:
    Function(const std::string& name,
             int parameter_count,
             SEXP r_definition,
             bool pub,
             bool s3_generic,
             bool s3_method)
        : Object()
        , name_(name)
        , parameter_count_(parameter_count)
        , r_definition_(r_definition)
        , public_(pub)
        , s3_generic_(s3_generic)
        , s3_method_(s3_method) {
        R_PreserveObject(r_definition_);
    }

    ~Function() {
        R_ReleaseObject(r_definition_);
    }

    const std::string& get_name() const {
        return name_;
    }

    int get_parameter_count() const {
        return parameter_count_;
    }

    SEXP get_definition() {
        return r_definition_;
    }

    bool is_public() const {
        return public_;
    }

    bool is_s3_generic() const {
        return s3_generic_;
    }

    bool is_s3_method() const {
        return s3_method_;
    }

    static void initialize();

    static void finalize();

    static SEXP get_class();

    static std::shared_ptr<Function> from_sexp(SEXP r_function);

    static SEXP to_sexp(std::shared_ptr<Function> function);

    static void destroy_sexp(SEXP r_function);

  private:
    std::string name_;
    int parameter_count_;
    SEXP r_definition_;
    bool public_;
    bool s3_generic_;
    bool s3_method_;

    static SEXP class_;
};

using FunctionSPtr = std::shared_ptr<Function>;

} // namespace instrumentr

#endif /* INSTRUMENTR_FUNCTION_HPP */
