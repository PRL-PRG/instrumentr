#include "../inst/include/ExecutionContext.hpp"

namespace lightr {

std::string
execution_context_to_string(const ExecutionContext& execution_context) {
    switch (execution_context) {
    case ExecutionContext::Lightr:
        return "lightr";
        break;
    case ExecutionContext::Application:
        return "application";
        break;
    case ExecutionContext::ApplicationEntryCallback:
        return "application_entry_callback";
        break;
    case ExecutionContext::ApplicationExitCallback:
        return "application_exit_callback";
        break;
    case ExecutionContext::PackageEntryCallback:
        return "package_entry_callback";
        break;
    case ExecutionContext::PackageExitCallback:
        return "package_exit_callback";
        break;
    case ExecutionContext::FunctionEntryCallback:
        return "function_entry_callback";
        break;
    case ExecutionContext::FunctionExitCallback:
        return "function_exit_callback";
        break;
    case ExecutionContext::CallEntryCallback:
        return "call_entry_callback";
        break;
    case ExecutionContext::CallExitCallback:
        return "call_exit_callback";
        break;
    default:
        return "<undefined_execution_context>";
        break;
    }
}

} // namespace lightr
