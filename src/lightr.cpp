#include "../inst/include/lightr.hpp"
#include "../inst/include/Parameter.hpp"

namespace lightr {

ApplicationSPtr application(nullptr);

ApplicationSPtr get_application() {
    return application;
}

void initialize_lightr() {
    if (!application) {
        Application::initialize();
        CallStack::initialize();
        Package::initialize();
        Function::initialize();
        Call::initialize();
        Parameter::initialize();
        Argument::initialize();
        CallStackSPtr call_stack = std::make_shared<CallStack>();
        application = std::make_shared<Application>(call_stack); //);
    }
}

} // namespace lightr
