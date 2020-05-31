#include "../inst/include/lightr.hpp"
#include "../inst/include/Parameter.hpp"

namespace lightr {

ApplicationSPtr application_(nullptr);
ContextSPtr context_(nullptr);

ApplicationSPtr get_application() {
    return application_;
}

void initialize_lightr() {
    if (!application_) {
        Context::initialize();
        Application::initialize();
        CallStack::initialize();
        Package::initialize();
        Function::initialize();
        Call::initialize();
        Parameter::initialize();
        Argument::initialize();
        CallStackSPtr call_stack = std::make_shared<CallStack>();
        application_ = std::make_shared<Application>(call_stack);
    }
}

ContextSPtr get_context() {
    return context_;
}

void set_context(ContextSPtr context) {
    context_ = context;
}

} // namespace lightr
