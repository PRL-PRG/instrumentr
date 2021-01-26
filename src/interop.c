#include "interop.h"
#include <stdio.h>
#include "utilities.h"
#include <stdarg.h>

#define MESSAGE_BUFFER_SIZE 4096

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

SEXP instrumentr_sexp_set_names(SEXP r_object, SEXP r_names) {
    Rf_setAttrib(r_object, R_NamesSymbol, r_names);
    return r_object;
}

SEXP instrumentr_sexp_set_row_names(SEXP r_object, SEXP r_row_names) {
    Rf_setAttrib(r_object, R_RowNamesSymbol, r_row_names);
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

SEXP instrumentr_c_double_to_r_double(double value) {
    return ScalarReal(value);
}

double instrumentr_r_double_to_c_double(SEXP r_value) {
    return asReal(r_value);
}

SEXP instrumentr_c_pointer_to_r_externalptr(void* pointer,
                                            SEXP r_tag,
                                            SEXP r_prot,
                                            R_CFinalizer_t finalizer) {
    SEXP r_value = R_MakeExternalPtr(pointer, r_tag, r_prot);
    if (finalizer != NULL) {
        R_RegisterCFinalizerEx(r_value, finalizer, TRUE);
    }
    return r_value;
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

void instrumentr_r_externalptr_clear(SEXP r_externalptr) {
    return R_ClearExternalPtr(r_externalptr);
}

SEXP create_list(int column_count, va_list columns) {

    va_list protection;

    va_copy(protection, columns);

    /* NOTE: protect all arguments before allocating memory */
    for (int i = 0; i < column_count; ++i) {
        const char* name = va_arg(protection, const char*);
        SEXP column = va_arg(protection, SEXP);
        PROTECT(column);
    }

    va_end(protection);

    SEXP r_list = PROTECT(allocVector(VECSXP, column_count));

    SEXP r_column_names = PROTECT(allocVector(STRSXP, column_count));

    for (int i = 0; i < column_count; ++i) {
        const char* name = va_arg(columns, const char*);
        SEXP column = va_arg(columns, SEXP);
        SET_STRING_ELT(r_column_names, i, mkChar(name));
        SET_VECTOR_ELT(r_list, i, column);
    }

    instrumentr_sexp_set_names(r_list, r_column_names);

    UNPROTECT(2 + column_count);

    return r_list;
}

SEXP set_row_names(SEXP r_data_frame) {
    int row_count = 0;

    if (LENGTH(r_data_frame) > 0) {
        row_count = LENGTH(VECTOR_ELT(r_data_frame, 0));
    }

    SEXP r_row_names = PROTECT(allocVector(STRSXP, row_count));

    for (int i = 0; i < row_count; ++i) {
        SET_STRING_ELT(r_row_names, i, mkChar(int_to_string(i + 1)));
    }

    instrumentr_sexp_set_row_names(r_data_frame, r_row_names);

    UNPROTECT(1);

    return r_data_frame;
}

SEXP instrumentr_create_list(int column_count, ...) {
    va_list columns;
    va_start(columns, column_count);

    SEXP r_list = PROTECT(create_list(column_count, columns));

    va_end(columns);

    UNPROTECT(1);

    return r_list;
}

SEXP instrumentr_create_data_frame(int column_count, ...) {
    va_list columns;
    va_start(columns, column_count);

    SEXP r_data_frame = PROTECT(create_list(column_count, columns));

    va_end(columns);

    set_row_names(r_data_frame);

    instrumentr_sexp_set_class(r_data_frame, mkString("data.frame"));

    UNPROTECT(1);

    return r_data_frame;
}

SEXP r_instrumentr_get_object_details(SEXP r_value,
                                      SEXP r_variable,
                                      SEXP r_environment,
                                      SEXP r_peek) {
    SEXP r_actual_value = r_value;

    if (r_variable != R_NilValue) {
        const char* variable = CHAR(STRING_ELT(r_variable, 0));

        r_actual_value = Rf_findVarInFrame(r_environment, Rf_install(variable));

        int peek = LOGICAL(r_peek)[0];

        if (peek && TYPEOF(r_actual_value) == PROMSXP) {
            r_actual_value = PREXPR(r_actual_value);
        }
    }

    char address[1024];

    if (sprintf(address, "%p", (void*) r_actual_value) == -1) {
        Rf_error("Getting address of SEXP failed");
    }

    SEXP r_result = PROTECT(Rf_allocVector(VECSXP, 2));
    SET_VECTOR_ELT(r_result, 0, Rf_mkString(address));
    SET_VECTOR_ELT(
        r_result, 1, Rf_mkString(Rf_type2char(TYPEOF(r_actual_value))));

    SEXP r_names = PROTECT(Rf_allocVector(STRSXP, 2));
    SET_STRING_ELT(r_names, 0, mkChar("address"));
    SET_STRING_ELT(r_names, 1, mkChar("type"));

    Rf_setAttrib(r_result, R_NamesSymbol, r_names);

    UNPROTECT(2);

    return r_result;
}
