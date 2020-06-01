#include "../inst/include/lightr.hpp"
#include "../inst/include/Parameter.hpp"

namespace lightr {

ApplicationSPtr application_(nullptr);
ContextSPtr context_(nullptr);

ApplicationSPtr get_application() {
    return application_;
}

void set_application(ApplicationSPtr application) {
    application_ = application;
}

ContextSPtr get_context() {
    return context_;
}

void set_context(ContextSPtr context) {
    context_ = context;

    ApplicationSPtr application = get_application();
    SEXP r_application = Application::to_sexp(application);

    if (context_ && context_->has_initializer()) {
        SEXP initializer = context_->get_initializer();
        SEXP env = context->get_environment();

        Rf_eval(Rf_lang2(initializer, r_application), env);
    }
}

} // namespace lightr
