#include "r_api.h"
#include "../inst/include/lightr.hpp"
#include "../inst/include/Package.hpp"
#include "../inst/include/Function.hpp"
#include "../inst/include/Call.hpp"
#include "../inst/include/Parameter.hpp"
#include "../inst/include/Context.hpp"
#include "utilities.h"

//#define DEBUG
bool interception_enabled = true;

#ifdef DEBUG
int indentation = 0;
#endif

using lightr::Application;
using lightr::ApplicationSPtr;
using lightr::Argument;
using lightr::ArgumentSPtr;
using lightr::Call;
using lightr::CallSPtr;
using lightr::CallStack;
using lightr::CallStackSPtr;
using lightr::Context;
using lightr::ContextSPtr;
using lightr::Function;
using lightr::FunctionSPtr;
using lightr::Package;
using lightr::PackageSPtr;
using lightr::Parameter;
using lightr::ParameterSPtr;

using lightr::get_application;
using lightr::get_context;

SEXP r_lightr_initialize(SEXP r_application_name,
                         SEXP r_application_directory,
                         SEXP r_global_environment,
                         SEXP r_package_environment,
                         SEXP r_state_environment) {
    initialize_utilities(r_package_environment, r_state_environment);
    Context::initialize();
    Application::initialize();
    CallStack::initialize();
    Package::initialize();
    Function::initialize();
    Call::initialize();
    Parameter::initialize();
    Argument::initialize();

    std::string application_name = CHAR(asChar(r_application_name));
    std::string application_directory = CHAR(asChar(r_application_directory));
    CallStackSPtr call_stack = std::make_shared<CallStack>();

    set_application(std::make_shared<Application>(application_name,
                                                  application_directory,
                                                  r_global_environment,
                                                  call_stack));

    ApplicationSPtr application = get_application();

#ifdef DEBUG
    fprintf(stderr,
            "┌── Application(name='%s', directory='%s', environment='%p')\n",
            application->get_name().c_str(),
            application->get_directory().c_str(),
            (void*) (application->get_environment()));
    ++indentation;
#endif

    return R_NilValue;
}

SEXP r_lightr_finalize() {
    ApplicationSPtr application = get_application();

#ifdef DEBUG
    --indentation;
    fprintf(stderr,
            "└── Application(name='%s', directory='%s', environment='%p')\n",
            application->get_name().c_str(),
            application->get_directory().c_str(),
            (void*) (application->get_environment()));
#endif

    SEXP r_application = Application::to_sexp(application);
    ContextSPtr context = get_context();

    if (context && context->has_finalizer()) {
        SEXP finalizer = context->get_finalizer();
        SEXP env = context->get_environment();

        Rf_eval(Rf_lang2(finalizer, r_application), env);
    }

    return R_NilValue;
}

SEXP r_lightr_intercept_package_entry(SEXP r_package) {
#ifdef DEBUG
    PackageSPtr package = Package::from_sexp(r_package);
    fprintf(stderr,
            "├── Package(name='%s', directory='%s', environment='%p')\n",
            package->get_name().c_str(),
            package->get_directory().c_str(),
            (void*) (package->get_environment()));
    ++indentation;
#endif

    ApplicationSPtr application = get_application();
    SEXP r_application = Application::to_sexp(application);
    ContextSPtr context = get_context();

    if (context && context->has_package_entry_callback()) {
        SEXP package_entry_callback = context->get_package_entry_callback();
        SEXP env = context->get_environment();

        Rf_eval(Rf_lang3(package_entry_callback, r_application, r_package),
                env);
    }

    return R_NilValue;
}

SEXP r_lightr_intercept_package_exit(SEXP r_package) {
#ifdef DEBUG
    --indentation;
    PackageSPtr package = Package::from_sexp(r_package);
    fprintf(stderr,
            "├── Package(name='%s', directory='%s', environment='%p')\n",
            package->get_name().c_str(),
            package->get_directory().c_str(),
            (void*) (package->get_environment()));
#endif

    ApplicationSPtr application = get_application();
    SEXP r_application = Application::to_sexp(application);
    ContextSPtr context = get_context();

    if (context && context->has_package_exit_callback()) {
        SEXP package_exit_callback = context->get_package_exit_callback();
        SEXP env = context->get_environment();

        Rf_eval(Rf_lang3(package_exit_callback, r_application, r_package), env);
    }

    return R_NilValue;
}

ArgumentSPtr
create_argument(SEXP r_argument_name, SEXP r_argument_value, SEXP r_call_env) {
    /* value (promise optimized away by compiler)  */
    if (TYPEOF(r_argument_value) != PROMSXP) {
        SEXP r_argument_name_str =
            PROTECT(mkString(CHAR(PRINTNAME(r_argument_name))));

        r_argument_value = delayed_assign(r_argument_name,
                                          r_argument_name_str,
                                          r_argument_value,
                                          r_call_env,
                                          r_call_env,
                                          r_call_env);
        UNPROTECT(1);
    }

    ArgumentSPtr argument =
        std::make_shared<Argument>(CHAR(PRINTNAME(r_argument_name)));

    argument->set_value(r_argument_value);

    return argument;
}

SEXP r_lightr_intercept_call_entry(SEXP r_package_ptr,
                                   SEXP r_function_ptr,
                                   SEXP r_function_obj,
                                   SEXP r_call_env,
                                   SEXP r_caller_env) {
    ApplicationSPtr application = lightr::get_application();
    PackageSPtr package = Package::from_sexp(r_package_ptr);
    FunctionSPtr function = Function::from_sexp(r_function_ptr);
    CallSPtr call = std::make_shared<Call>(function);
    CallStackSPtr call_stack = application->get_call_stack();
    std::string argument_name;

    call->set_environment(r_call_env);

#ifdef DEBUG
    for (int i = 0; i < indentation - 1; ++i) {
        fprintf(stderr, "│   ");
    }
    fprintf(stderr, "├── Function Entry '%s'\n", function->get_name().c_str());
    ++indentation;
#endif

    call_stack->push_frame(call);

    SEXP r_parameters = FORMALS(r_function_obj);

    for (int parameter_position = 0; r_parameters != R_NilValue;
         ++parameter_position, r_parameters = CDR(r_parameters)) {
        SEXP r_argument_name = TAG(r_parameters);
        SEXP r_argument_value = Rf_findVarInFrame(r_call_env, r_argument_name);

        ParameterSPtr parameter = std::make_shared<Parameter>(
            R_CHAR(PRINTNAME(r_argument_name)), parameter_position);

        call->add_parameter(parameter);

        /* missing argument */
        if (r_argument_value == R_UnboundValue ||
            r_argument_value == R_MissingArg) {
            /* NOTE: do nothing */
        }
        /* ... parameter */
        else if (TYPEOF(r_argument_value) == DOTSXP) {
            std::vector<SEXP> dot_arguments;
            std::vector<SEXP> dot_tags;

            int index = 0;

            for (SEXP r_dot_argument_pointer = r_argument_value;
                 r_dot_argument_pointer != R_NilValue;
                 r_dot_argument_pointer = CDR(r_dot_argument_pointer)) {
                SEXP r_dot_argument_tag = TAG(r_dot_argument_pointer);

                SEXP r_dot_argument_value = CAR(r_dot_argument_pointer);

                if (TYPEOF(r_dot_argument_value) != PROMSXP) {
                    dot_arguments.push_back(PROTECT(
                        create_promise(r_dot_argument_value, r_caller_env)));
                } else {
                    dot_arguments.push_back(PROTECT(r_dot_argument_value));
                }

                dot_tags.push_back(r_dot_argument_tag);

                ++index;
            }

            SEXP dots = PROTECT(R_NilValue);

            for (int i = dot_arguments.size() - 1; i >= 0; --i) {
                SEXP r_dot_argument_value = dot_arguments[i];
                SEXP r_dot_argument_tag = dot_tags[i];
                std::string argument_name = "";

                if (r_dot_argument_tag == R_NilValue ||
                    r_dot_argument_tag == R_MissingArg ||
                    r_dot_argument_tag == R_UnboundValue) {
                    argument_name = "";
                } else {
                    argument_name = CHAR(PRINTNAME(r_dot_argument_tag));
                }

                UNPROTECT(1);
                dots = PROTECT(CONS(r_dot_argument_value, dots));
                SET_TAG(dots, r_dot_argument_tag);

                ArgumentSPtr argument =
                    std::make_shared<Argument>(argument_name);

                argument->set_value(r_dot_argument_value);

                parameter->add_argument(argument);
            }

            SET_TYPEOF(dots, DOTSXP);

            Rf_setVar(R_DotsSymbol, dots, r_call_env);

            UNPROTECT(1);

            UNPROTECT(dot_arguments.size());
        }
        /* non ... parameter  */
        else {
            // argument_name = CHAR(PRINTNAME(r_argument_name));
            parameter->add_argument(
                create_argument(r_argument_name, r_argument_value, r_call_env));
        }
    }

    ContextSPtr context = get_context();

    if (context && context->has_call_entry_callback()) {
        SEXP call_entry_callback = context->get_call_entry_callback();

        r_lightr_disable_interception();

        Rf_eval(Rf_lang5(call_entry_callback,
                         Application::to_sexp(get_application()),
                         r_package_ptr,
                         r_function_ptr,
                         Call::to_sexp(call)),
                context->get_environment());

        r_lightr_enable_interception();
    }

    return R_NilValue;
}

SEXP r_lightr_intercept_call_exit(SEXP r_package,
                                  SEXP r_function,
                                  SEXP result,
                                  SEXP failed) {
    PackageSPtr package = Package::from_sexp(r_package);
    FunctionSPtr function = Function::from_sexp(r_function);
    ApplicationSPtr application = lightr::get_application();
    CallStackSPtr call_stack = application->get_call_stack();
    CallSPtr call = call_stack->pop_frame();

#ifdef DEBUG
    --indentation;
    for (int i = 0; i < indentation - 1; ++i) {
        fprintf(stderr, "│   ");
    }
    fprintf(stderr, "├── Function Exit '%s'\n", function->get_name().c_str());
    for (int i = 0; i < indentation - 1; ++i) {
        fprintf(stderr, "│   ");
    }
    fprintf(stderr, "█\n");
#endif

    if (call->get_function()->get_name() != function->get_name()) {
        Rf_errorcall(R_NilValue,
                     "Error: unmatched call entry and exit, " //
                     "exiting '%s::%s' but expected to exit '%s'.",
                     package->get_name(),
                     function->get_name(),
                     call->get_function()->get_name());
    }

    ContextSPtr context = get_context();

    if (context && context->has_call_exit_callback()) {
        SEXP call_exit_callback = context->get_call_exit_callback();

        r_lightr_disable_interception();

        Rf_eval(Rf_lang5(call_exit_callback,
                         Application::to_sexp(get_application()),
                         r_package,
                         r_function,
                         Call::to_sexp(call)),
                context->get_environment());

        r_lightr_enable_interception();
    }

    return R_NilValue;
}

SEXP r_lightr_interception_is_enabled() {
    return ScalarLogical(interception_enabled);
}

SEXP r_lightr_disable_interception() {
    interception_enabled = false;
    return R_NilValue;
}

SEXP r_lightr_enable_interception() {
    interception_enabled = true;
    return R_NilValue;
}
