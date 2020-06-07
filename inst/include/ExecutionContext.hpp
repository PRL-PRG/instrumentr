#include <string>

namespace lightr {

enum class ExecutionContext {
    Lightr,
    Application,
    ApplicationLoadCallback,
    ApplicationUnloadCallback,
    ApplicationAttachCallback,
    ApplicationDetachCallback,
    PackageLoadCallback,
    PackageUnloadCallback,
    PackageAttachCallback,
    PackageDetachCallback,
    FunctionEntryCallback,
    FunctionExitCallback,
    CallEntryCallback,
    CallExitCallback
};

std::string
execution_context_to_string(const ExecutionContext& execution_context);

} // namespace lightr
