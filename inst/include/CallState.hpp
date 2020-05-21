#ifndef LIGHTR_CALL_STATE_HPP
#define LIGHTR_CALL_STATE_HPP

#include <string>

namespace lightr {

enum class CallState { Active, Success, Error };

std::string call_state_to_string(CallState call_state);
CallState string_to_call_state(const std::string str);

} // namespace lightr

#endif /* LIGHTR_CALL_STATE_HPP */
