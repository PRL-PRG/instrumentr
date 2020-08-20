#include "Context.hpp"

namespace instrumentr {

SEXP Context::class_ = NULL;

std::shared_ptr<Context> Context::create(SEXP r_environment) {
#ifdef USING_DYNTRACE
    dyntracer_t* dyntracer = rdyntrace_create_dyntracer();
    /* NOTE: this is done in two steps because constructor is private and can't
     * be accessed by make_shared  */
    Context* context_ptr = new Context(r_environment, dyntracer);
    ContextSPtr context(context_ptr);
    /* NOTE: dyntracer has a non-owning reference to the context.
       This works fine with the smart pointers used elsewhere in the code
       because dyntracer gets deleted by the context. Their lifetimes are tied
       to each other. */
    /* FIXME: this creates cyclic references,
       Context holds a reference to itself via dyntracer, so a context object
       never gets deleted.
    */
    dyntracer_set_data(dyntracer, new ContextSPtr(context));
#else
    Context* context_ptr = new Context(r_environment);
    ContextSPtr context(context_ptr);
#endif /* USING_DYNTRACE  */
    // cppcheck-suppress deallocret
    return context;
}

void Context::initialize() {
    class_ = Object::create_class({"instrumentr_context"});
    R_PreserveObject(class_);
}

void Context::finalize() {
    R_ReleaseObject(class_);
    class_ = NULL;
}

SEXP Context::get_class() {
    return class_;
}

} // namespace instrumentr
