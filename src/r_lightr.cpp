#include "../inst/include/lightr.hpp"
#include "r_api.h"

using lightr::Application;

SEXP r_lightr_get_application() {
    return Application::to_sexp(lightr::get_application());
}
