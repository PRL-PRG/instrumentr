#ifndef LIGHTR_OBJECT_HPP
#define LIGHTR_OBJECT_HPP

#include <memory>
#include <Rinternals.h>

namespace lightr {

class Object {
  public:
    Object(): id_(Object::get_next_id_()), r_data_(nullptr) {
    }

    virtual ~Object() {
    }

    int get_id() const {
        return id_;
    }

    void set_data(SEXP r_data) {
        r_data_ = r_data;
    }

    SEXP get_data() {
        return r_data_;
    }

    void remove_data() {
        r_data_ = nullptr;
    }

    bool has_data() const {
        return r_data_ != nullptr;
    }

    static void initialize();

    static SEXP get_class();

    static std::shared_ptr<Object> from_sexp(SEXP r_object);

  private:
    int id_;
    SEXP r_data_;

    static int get_next_id_();
    static int id_counter_;
    static SEXP class_;
};

using ObjectSPtr = std::shared_ptr<Object>;

} // namespace lightr

#endif /* LIGHTR_OBJECT_HPP */
