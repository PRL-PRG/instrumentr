#include "interop.h"
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

void instrumentr_sexp_acquire(SEXP r_object) {
    if (r_object != NULL) {
        R_PreserveObject(r_object);
    }
}

void instrumentr_sexp_release(SEXP r_object) {
    if (r_object != NULL) {
        R_ReleaseObject(r_object);
    }
}

SEXP instrumentr_sexp_set_class(SEXP r_object, SEXP r_class) {
    Rf_setAttrib(r_object, R_ClassSymbol, r_class);
    return r_object;
}

SEXP instrumentr_c_int_to_r_logical(int value) {
    return ScalarLogical(value);
}

int instrumentr_r_logical_to_c_int(SEXP r_value) {
    return asLogical(r_value);
}

SEXP instrumentr_c_int_to_r_integer(int value) {
    return ScalarInteger(value);
}

int instrumentr_r_integer_to_c_int(SEXP r_value) {
    return asInteger(r_value);
}

SEXP instrumentr_c_int_to_r_double(int value) {
    return ScalarReal(value);
}

double instrumentr_r_double_to_c_int(SEXP r_value) {
    return (int) (asReal(r_value));
}

SEXP instrumentr_c_pointer_to_r_externalptr(void* pointer, char* tag) {
    SEXP r_tag = tag == NULL ? R_NilValue : mkString(tag);
    return R_MakeExternalPtr(pointer, r_tag, R_NilValue);
}

SEXP instrumentr_c_string_to_r_character(const char* string) {
    return mkString(string);
}

const char* instrumentr_r_character_to_c_string(SEXP r_character) {
    return CHAR(STRING_ELT(r_character, 0));
}

void* instrumentr_r_externalptr_to_c_pointer(SEXP r_pointer) {
    return R_ExternalPtrAddr(r_pointer);
}
