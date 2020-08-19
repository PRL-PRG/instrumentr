#include "Callback.hpp"
#include "Context.hpp"

namespace instrumentr {

Callback::Callback(Type type, void* function, bool is_r_function)
    : type_(type), function_(function), is_r_function_(is_r_function) {
    if (is_r_callback()) {
        r_symbol_ = Rf_install(callback_type_to_string(type_).c_str());
        R_PreserveObject(r_symbol_);
        R_PreserveObject(get_function<SEXP>());
    }
}

SEXP Callback::create_class(const char* subclass) {
    return Object::create_class({subclass, "instrumentr_callback"});
}

std::string callback_type_to_string(Callback::Type callback_type) {
    switch (callback_type) {
    case Callback::Type::Instrumentr:
        return "instrumentr";
    case Callback::Type::Application:
        return "application";
    case Callback::Type::ApplicationLoadCallback:
        return "ApplicationLoadCallback";
    case Callback::Type::ApplicationUnloadCallback:
        return "ApplicationUnloadCallback";
    case Callback::Type::ApplicationAttachCallback:
        return "ApplicationAttachCallback";
    case Callback::Type::ApplicationDetachCallback:
        return "ApplicationDetachCallback";
    case Callback::Type::PackageLoadCallback:
        return "PackageLoadCallback";
    case Callback::Type::PackageUnloadCallback:
        return "PackageUnloadCallback";
    case Callback::Type::PackageAttachCallback:
        return "PackageAttachCallback";
    case Callback::Type::PackageDetachCallback:
        return "PackageDetachCallback";
    case Callback::Type::FunctionAttachCallback:
        return "FunctionAttachCallback";
    case Callback::Type::FunctionDetachCallback:
        return "FunctionDetachCallback";
    case Callback::Type::CallEntryCallback:
        return "CallEntryCallback";
    case Callback::Type::CallExitCallback:
        return "CallExitCallback";
    case Callback::Type::GcAllocationCallback:
        return "GcAllocationCallback";
    case Callback::Type::VariableDefinitionCallback:
        return "VariableDefinitionCallback";
    case Callback::Type::VariableAssignmentCallback:
        return "VariableAssignmentCallback";
    case Callback::Type::VariableRemovalCallback:
        return "VariableRemovalCallback";
    case Callback::Type::VariableLookupCallback:
        return "VariableLookupCallback";
    }
    return "Undefined";
}

} // namespace instrumentr
