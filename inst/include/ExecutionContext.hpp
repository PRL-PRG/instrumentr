#include <string>

namespace lightr {

enum class ExecutionContext {
    Lightr,
    Application,
    ApplicationEntryCallback,
    ApplicationExitCallback,
    PackageEntryCallback,
    PackageExitCallback,
    FunctionEntryCallback,
    FunctionExitCallback,
    CallEntryCallback,
    CallExitCallback
};

std::string
execution_context_to_string(const ExecutionContext& execution_context);

} // namespace lightr
