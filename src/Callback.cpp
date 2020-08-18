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

void Callback::initialize_pre_invocation_(ContextSPtr context) {
    context->disable_tracing();
    context->push_callback_type(get_type());
}

void Callback::finalize_post_invocation_(ContextSPtr context) {
    context->pop_callback_type();
    context->reinstate_tracing();
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
    }
    return "Undefined";
}

} // namespace instrumentr
