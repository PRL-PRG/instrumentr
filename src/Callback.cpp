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
    case Callback::Type::ApplicationLoad:
        return "instrumentr_application_load_callback";
    case Callback::Type::ApplicationUnload:
        return "instrumentr_application_unload_callback";
    case Callback::Type::ApplicationAttach:
        return "instrumentr_application_attach_callback";
    case Callback::Type::ApplicationDetach:
        return "instrumentr_application_detach_callback";
    case Callback::Type::PackageLoad:
        return "instrumentr_package_load_callback";
    case Callback::Type::PackageUnload:
        return "instrumentr_package_unload_callback";
    case Callback::Type::PackageAttach:
        return "instrumentr_package_attach_callback";
    case Callback::Type::PackageDetach:
        return "instrumentr_package_detach_callback";
    case Callback::Type::FunctionAttach:
        return "instrumentr_function_attach_callback";
    case Callback::Type::FunctionDetach:
        return "instrumentr_function_detach_callback";
    case Callback::Type::CallEntry:
        return "instrumentr_call_entry_callback";
    case Callback::Type::CallExit:
        return "instrumentr_call_exit_callback";
    case Callback::Type::GcAllocation:
        return "instrumentr_gc_allocation_callback";
    case Callback::Type::VariableDefinition:
        return "instrumentr_variable_definition_callback";
    case Callback::Type::VariableAssignment:
        return "instrumentr_variable_assignment_callback";
    case Callback::Type::VariableRemoval:
        return "instrumentr_variable_removal_callback";
    case Callback::Type::VariableLookup:
        return "instrumentr_variable_lookup_callback";
    }
    return "undefined";
}

} // namespace instrumentr
