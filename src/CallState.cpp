#include "../inst/include/CallState.hpp"

namespace lightr {

std::string call_state_to_string(CallState call_state) {
    switch (call_state) {
    case CallState::Active:
        return "active";
    case CallState::Success:
        return "success";
    case CallState::Error:
        return "error";
    default:
        return "undefined";
    }
}

CallState string_to_call_state(const std::string str) {
    if (str == "active") {
        return CallState::Active;
    } else if (str == "success") {
        return CallState::Success;
    } else if (str == "error") {
        return CallState::Error;
    } else {
        /* TODO: error  */
    }
}

} // namespace lightr
