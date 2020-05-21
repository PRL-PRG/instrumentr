#ifndef LIGHTR_OBJECT_HPP
#define LIGHTR_OBJECT_HPP

#include <memory>
#include <Rinternals.h>

namespace lightr {

class Object {
  public:
    Object(): id_(Object::get_next_id_()) {
    }

    virtual ~Object() {
    }

    int get_id() const {
        return id_;
    }

    static void initialize();

    static SEXP get_class();

    static std::shared_ptr<Object> from_sexp(SEXP r_object);

  private:
    int id_;

    static int get_next_id_();
    static int id_counter_;
    static SEXP class_;
};

using ObjectSPtr = std::shared_ptr<Object>;

} // namespace lightr

#endif /* LIGHTR_OBJECT_HPP */
