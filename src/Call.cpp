#include "../inst/include/Call.hpp"

call_id_t Call::id_counter_ = -1;

call_id_t Call::get_next_id_() {
    return ++id_counter_;
}
