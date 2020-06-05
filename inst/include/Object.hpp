#ifndef LIGHTR_OBJECT_HPP
#define LIGHTR_OBJECT_HPP

#include <memory>
#include <string>
#include "lightr.hpp"

namespace lightr {

class Object {
  public:
    Object(): id_(Object::get_next_id_()), r_data_(get_invalid_value()) {
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
        r_data_ = get_invalid_value();
    }

    bool has_data() const {
        return is_valid_value(r_data_);
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

} // namespace lightr

#endif /* LIGHTR_OBJECT_HPP */
