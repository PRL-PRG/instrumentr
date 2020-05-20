#include "../inst/include/Call.hpp"

call_id_t Call::call_id_counter_ = -1;

call_id_t Call::get_next_call_id_() {
    return ++call_id_counter_;
}
