#include "../inst/include/lightr.hpp"
#include "../inst/include/CallStack.hpp"

namespace lightr {

ApplicationSPtr application(nullptr);

ApplicationSPtr get_application() {
    return application;
}

void initialize_lightr() {
    if (!application) {
        CallStackSPtr call_stack = std::make_shared<CallStack>();
        application = std::make_shared<Application>(call_stack); //);
    }
}

} // namespace lightr
