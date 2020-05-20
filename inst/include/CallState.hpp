#ifndef LIGHTR_CALL_STATE_HPP
#define LIGHTR_CALL_STATE_HPP

#include <string>

enum class CallState { Active, Success, Error };

std::string call_state_to_string(CallState call_state);
CallState string_to_call_state(const std::string str);

#endif /* LIGHTR_CALL_STATE_HPP */
