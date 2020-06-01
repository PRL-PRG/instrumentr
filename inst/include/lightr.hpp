#ifndef LIGHTR_LIGHTR_HPP
#define LIGHTR_LIGHTR_HPP

#include "Application.hpp"
#include "Context.hpp"

namespace lightr {

ApplicationSPtr get_application();

void set_application(ApplicationSPtr application);

ContextSPtr get_context();

void set_context(ContextSPtr context);

} // namespace lightr

#endif /* LIGHTR_LIGHTR_HPP */
