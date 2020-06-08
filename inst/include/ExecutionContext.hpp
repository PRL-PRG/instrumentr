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
    FunctionAttachCallback,
    FunctionDetachCallback,
    CallEntryCallback,
    CallExitCallback
};

std::string
execution_context_to_string(const ExecutionContext& execution_context);

} // namespace lightr
