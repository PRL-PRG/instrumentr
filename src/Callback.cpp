#include "Callback.hpp"
#include "Context.hpp"

namespace instrumentr {

Callback::Callback(Type type, void* function, bool is_r_function)
    : type_(type)
    , function_(function)
    , is_r_function_(is_r_function)
    , status_({true}) {
    if (is_r_callback()) {
        r_function_name_ = Rf_install(callback_type_to_string(type_).c_str());
        R_PreserveObject(r_function_name_);
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
    case Callback::Type::ObjectCoerce:
        return "instrumentr_object_coerce_callback";
    case Callback::Type::ObjectDuplicate:
        return "instrumentr_object_duplicate_callback";
    case Callback::Type::VectorCopy:
        return "instrumentr_vector_copy_callback";
    case Callback::Type::MatrixCopy:
        return "instrumentr_matrix_copy_callback";
    case Callback::Type::BuiltinCallEntry:
        return "instrumentr_builtin_call_entry_callback";
    case Callback::Type::BuiltinCallExit:
        return "instrumentr_builtin_call_exit_callback";
    case Callback::Type::SpecialCallEntry:
        return "instrumentr_special_call_entry_callback";
    case Callback::Type::SpecialCallExit:
        return "instrumentr_special_call_exit_callback";
    case Callback::Type::ClosureCallEntry:
        return "instrumentr_closure_call_entry_callback";
    case Callback::Type::ClosureCallExit:
        return "instrumentr_closure_call_exit_callback";
    case Callback::Type::EvalEntry:
        return "instrumentr_eval_entry_callback";
    case Callback::Type::EvalExit:
        return "instrumentr_eval_exit_callback";
    case Callback::Type::GcAllocation:
        return "instrumentr_gc_allocation_callback";
    case Callback::Type::GcUnmark:
        return "instrumentr_gc_unmark_callback";
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
