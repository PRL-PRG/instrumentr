#include <instrumentr/log.h>
#include <instrumentr/Rincludes.h>
#include <stdio.h>

const int MESSAGE_BUFFER_SIZE = 4096;

char message_buffer[MESSAGE_BUFFER_SIZE];

char* get_message_buffer() {
    return message_buffer;
}

void instrumentr_log_error(const char* fmt, ...) {
    char* buffer = get_message_buffer();
    va_list args;
    va_start(args, fmt);
    vsprintf(buffer, fmt, args);
    va_end(args);
    Rf_error(buffer);
}

void instrumentr_log_warning(const char* fmt, ...) {
    char* buffer = get_message_buffer();
    va_list args;
    va_start(args, fmt);
    vsprintf(buffer, fmt, args);
    va_end(args);
    Rf_warning(buffer);
}
