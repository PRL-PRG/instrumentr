#include <string>

namespace instrumentr {

enum class ExecutionContext {
    Instrumentr,
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

} // namespace instrumentr
