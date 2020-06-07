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
    case ExecutionContext::ApplicationLoadCallback:
        return "application_load_callback";
        break;
    case ExecutionContext::ApplicationUnloadCallback:
        return "application_unload_callback";
        break;
    case ExecutionContext::ApplicationAttachCallback:
        return "application_attach_callback";
        break;
    case ExecutionContext::ApplicationDetachCallback:
        return "application_detach_callback";
        break;
    case ExecutionContext::PackageLoadCallback:
        return "package_load_callback";
        break;
    case ExecutionContext::PackageUnloadCallback:
        return "package_unload_callback";
        break;
    case ExecutionContext::PackageAttachCallback:
        return "package_attach_callback";
        break;
    case ExecutionContext::PackageDetachCallback:
        return "package_detach_callback";
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
