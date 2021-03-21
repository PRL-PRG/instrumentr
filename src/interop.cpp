#include "interop.h"
#include <stdio.h>
#include "utilities.h"
#include <stdarg.h>
#include "sha3.h"

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

void instrumentr_log_message(const char* fmt, ...) {
    char* buffer = get_message_buffer();
    va_list args;
    va_start(args, fmt);
    vsprintf(buffer, fmt, args);
    va_end(args);
    Rprintf(buffer);
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

SEXP instrumentr_c_complex_to_r_complex(Rcomplex value) {
    return ScalarComplex(value);
}

Rcomplex instrumentr_r_complex_to_c_complex(SEXP r_value) {
    return COMPLEX_ELT(r_value, 0);
}

SEXP instrumentr_c_raw_to_r_raw(Rbyte value) {
    return ScalarRaw(value);
}

Rbyte instrumentr_r_raw_to_c_raw(SEXP r_value) {
    return RAW_ELT(r_value, 0);
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
    if (string == NULL) {
        SEXP r_string = PROTECT(allocVector(STRSXP, 1));
        SET_STRING_ELT(r_string, 0, NA_STRING);
        UNPROTECT(1);
        return r_string;
    } else {
        return mkString(string);
    }
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
        /* read the column name to advance to the next va_arg */
        va_arg(protection, const char*);
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

std::vector<std::string> instrumentr_sexp_to_string(SEXP r_expr,
                                                    bool collapse) {
    std::vector<std::string> result;
    int linecount = 0;
    SEXP r_strvec = PROTECT(serialize_sexp(r_expr, &linecount));

    if (collapse) {
        std::string repr;
        for (int i = 0; i < linecount - 1; ++i) {
            repr.append(CHAR(STRING_ELT(r_strvec, i))).append("\n");
        }

        if (linecount >= 1) {
            repr.append(CHAR(STRING_ELT(r_strvec, linecount - 1)));
        }
        result.push_back(repr);
    }
    /* collapse = FALSE */
    else {
        for (int i = 0; i < linecount; ++i) {
            result.push_back(std::string(CHAR(STRING_ELT(r_strvec, i))));
        }
    }

    UNPROTECT(1);

    return result;
}

char const* get_alphabet() {
    static char constexpr tab[] = {"ABCDEFGHIJKLMNOP"
                                   "QRSTUVWXYZabcdef"
                                   "ghijklmnopqrstuv"
                                   "wxyz0123456789-_"};
    return &tab[0];
}

signed char const* get_inverse() {
    static signed char constexpr tab[] = {
        -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1, //   0-15
        -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1, //  16-31
        -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, 62, -1, -1, //  32-47
        52, 53, 54, 55, 56, 57, 58, 59,
        60, 61, -1, -1, -1, -1, -1, -1, //  48-63
        -1, 0,  1,  2,  3,  4,  5,  6,
        7,  8,  9,  10, 11, 12, 13, 14, //  64-79
        15, 16, 17, 18, 19, 20, 21, 22,
        23, 24, 25, -1, -1, -1, -1, 63, //  80-95
        -1, 26, 27, 28, 29, 30, 31, 32,
        33, 34, 35, 36, 37, 38, 39, 40, //  96-111
        41, 42, 43, 44, 45, 46, 47, 48,
        49, 50, 51, -1, -1, -1, -1, -1, // 112-127
        -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1, // 128-143
        -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1, // 144-159
        -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1, // 160-175
        -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1, // 176-191
        -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1, // 192-207
        -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1, // 208-223
        -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1, // 224-239
        -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1 // 240-255
    };
    return &tab[0];
}

/// Returns max chars needed to encode a base64 string
std::size_t constexpr encoded_size(std::size_t n) {
    return 4 * ((n + 2) / 3);
}

/// Returns max bytes needed to decode a base64 string
inline std::size_t constexpr decoded_size(std::size_t n) {
    return n / 4 * 3; // requires n&3==0, smaller
}

std::string instrumentr_string_encode(const std::string& src) {
    std::size_t length = src.size();
    std::string dest;
    dest.reserve(encoded_size(length) + 1);
    auto in = reinterpret_cast<unsigned char const*>(src.data());
    auto const tab = get_alphabet();

    for (auto n = length / 3; n--;) {
        dest.push_back(tab[(in[0] & 0xfc) >> 2]);
        dest.push_back(tab[((in[0] & 0x03) << 4) + ((in[1] & 0xf0) >> 4)]);
        dest.push_back(tab[((in[2] & 0xc0) >> 6) + ((in[1] & 0x0f) << 2)]);
        dest.push_back(tab[in[2] & 0x3f]);
        in += 3;
    }

    switch (length % 3) {
    case 2:
        dest.push_back(tab[(in[0] & 0xfc) >> 2]);
        dest.push_back(tab[((in[0] & 0x03) << 4) + ((in[1] & 0xf0) >> 4)]);
        dest.push_back(tab[(in[1] & 0x0f) << 2]);
        dest.push_back('=');
        break;

    case 1:
        dest.push_back(tab[(in[0] & 0xfc) >> 2]);
        dest.push_back(tab[((in[0] & 0x03) << 4)]);
        dest.push_back('=');
        dest.push_back('=');
        break;

    case 0:
        break;
    }

    return dest;
}

std::string instrumentr_string_decode(const std::string& src) {
    std::size_t length = src.length();
    std::string dest;
    dest.reserve(decoded_size(length) + 1);
    auto in = reinterpret_cast<unsigned char const*>(src.data());
    unsigned char c3[3], c4[4];
    int i = 0;
    int j = 0;

    auto const inverse = get_inverse();

    while (length-- && *in != '=') {
        auto const v = inverse[*in];
        if (v == -1)
            break;
        ++in;
        c4[i] = v;
        if (++i == 4) {
            c3[0] = (c4[0] << 2) + ((c4[1] & 0x30) >> 4);
            c3[1] = ((c4[1] & 0xf) << 4) + ((c4[2] & 0x3c) >> 2);
            c3[2] = ((c4[2] & 0x3) << 6) + c4[3];

            for (i = 0; i < 3; i++)
                dest.push_back(c3[i]);
            i = 0;
        }
    }

    if (i) {
        c3[0] = (c4[0] << 2) + ((c4[1] & 0x30) >> 4);
        c3[1] = ((c4[1] & 0xf) << 4) + ((c4[2] & 0x3c) >> 2);
        c3[2] = ((c4[2] & 0x3) << 6) + c4[3];

        for (j = 0; j < i - 1; j++)
            dest.push_back(c3[j]);
    }

    return dest;
}

SEXP r_instrumentr_string_encode(SEXP r_src) {
    int elt_count = Rf_length(r_src);
    SEXP r_dest = allocVector(STRSXP, elt_count);

    for (int i = 0; i < elt_count; ++i) {
        SEXP r_src_char = STRING_ELT(r_src, i);
        SEXP r_dest_char = NA_STRING;

        if (r_src_char != NA_STRING) {
            const std::string res = instrumentr_string_encode(CHAR(r_src_char));
            r_dest_char = mkChar(res.c_str());
        }

        SET_STRING_ELT(r_dest, i, r_dest_char);
    }

    return r_dest;
}

SEXP r_instrumentr_string_decode(SEXP r_src) {
    int elt_count = Rf_length(r_src);
    SEXP r_dest = allocVector(STRSXP, elt_count);

    for (int i = 0; i < elt_count; ++i) {
        SEXP r_src_char = STRING_ELT(r_src, i);
        SEXP r_dest_char = NA_STRING;

        if (r_src_char != NA_STRING) {
            const std::string res = instrumentr_string_decode(CHAR(r_src_char));
            r_dest_char = mkChar(res.c_str());
        }

        SET_STRING_ELT(r_dest, i, r_dest_char);
    }

    return r_dest;
}

std::string instrumentr_compute_hash(const std::string& src) {
    return SHA3(SHA3::Bits512)(src);
}

SEXP r_instrumentr_compute_hash(SEXP r_src, SEXP r_collapse) {
    int collapse = instrumentr_r_logical_to_c_int(r_collapse);

    int elt_count = Rf_length(r_src);

    if (collapse == 0) {
        SHA3 sha3(SHA3::Bits512);

        for (int i = 0; i < elt_count; ++i) {
            SEXP r_src_elt = STRING_ELT(r_src, i);
            if (r_src_elt != NA_STRING) {
                const char* input = CHAR(r_src_elt);
                sha3.add(input, strlen(input));
            }
        }

        return ScalarString(mkChar(sha3.getHash().c_str()));
    }

    /* only if collapse is not 0 */
    SEXP r_dest = PROTECT(allocVector(STRSXP, elt_count));
    SHA3 sha3(SHA3::Bits512);

    for (int i = 0; i < elt_count; ++i) {
        SEXP r_src_char = STRING_ELT(r_src, i);
        SEXP r_dest_char = NA_STRING;

        if (r_src_char != NA_STRING) {
            r_dest_char = mkChar(sha3(CHAR(r_src_char)).c_str());
        }

        SET_STRING_ELT(r_dest, i, r_dest_char);
    }

    UNPROTECT(1);

    return r_dest;
}
