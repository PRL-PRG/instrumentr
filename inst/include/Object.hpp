#ifndef INSTRUMENTR_OBJECT_HPP
#define INSTRUMENTR_OBJECT_HPP

#include <memory>
#include <string>
#include "instrumentr.hpp"

namespace instrumentr {

class Object {
  public:
    Object(): id_(Object::get_next_id_()), r_data_(get_undefined_object()) {
    }

    virtual ~Object() {
        if (has_data()) {
            R_ReleaseObject(r_data_);
        }
    }

    int get_id() const {
        return id_;
    }

    void set_data(SEXP r_data) {
        r_data_ = r_data;
        R_PreserveObject(r_data_);
    }

    SEXP get_data() {
        return r_data_;
    }

    void remove_data() {
        R_ReleaseObject(r_data_);
        r_data_ = get_undefined_object();
    }

    bool has_data() const {
        return is_defined_object(r_data_);
    }

    static SEXP create_class(const char* subclass);

    static SEXP create_class(const std::string& subclass);

    static std::shared_ptr<Object> from_sexp(SEXP r_object);

  private:
    int id_;
    SEXP r_data_;

    static int get_next_id_();
    static int id_counter_;
};

using ObjectSPtr = std::shared_ptr<Object>;

} // namespace instrumentr

#endif /* INSTRUMENTR_OBJECT_HPP */
