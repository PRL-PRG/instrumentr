#ifndef INSTRUMENTR_OBJECT_H
#define INSTRUMENTR_OBJECT_H

#include <instrumentr/api.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef void (*instrumentr_object_finalizer_t)(instrumentr_object_t object);

/*******************************************************************************
 * definition
 *******************************************************************************/

struct instrumentr_object_impl_t {
    instrumentr_object_type_t type;
    int reference_count;
    instrumentr_object_finalizer_t finalizer;
};

/*******************************************************************************
 * create
 *******************************************************************************/

instrumentr_object_t
instrumentr_object_create(int size,
                          instrumentr_object_type_t type,
                          instrumentr_object_finalizer_t finalizer);

/*******************************************************************************
 * interop
 *******************************************************************************/

SEXP instrumentr_object_wrap(instrumentr_object_t object);
instrumentr_object_t instrumentr_object_unwrap(SEXP r_object,
                                               instrumentr_object_type_t type);

#define INSTRUMENTR_OBJECT_INTEROP_DECLARE_API(OBJECT_NAME, OBJECT_TYPE) \
    SEXP instrumentr_##OBJECT_NAME##_wrap(                               \
        instrumentr_##OBJECT_NAME##_t OBJECT_NAME);                      \
    instrumentr_##OBJECT_NAME##_t instrumentr_##OBJECT_NAME##_unwrap(    \
        SEXP r_##OBJECT_NAME);

#define INSTRUMENTR_OBJECT_INTEROP_DEFINE_API(OBJECT_NAME, OBJECT_TYPE)      \
    SEXP instrumentr_##OBJECT_NAME##_wrap(                                   \
        instrumentr_##OBJECT_NAME##_t OBJECT_NAME) {                         \
        return instrumentr_object_wrap((instrumentr_object_t)(OBJECT_NAME)); \
    }                                                                        \
                                                                             \
    instrumentr_##OBJECT_NAME##_t instrumentr_##OBJECT_NAME##_unwrap(        \
        SEXP r_##OBJECT_NAME) {                                              \
        instrumentr_object_t object =                                        \
            instrumentr_object_unwrap(r_##OBJECT_NAME, OBJECT_TYPE);         \
        return (instrumentr_##OBJECT_NAME##_t)(object);                      \
    }

INSTRUMENTR_OBJECT_API_MAP(INSTRUMENTR_API_INCLUDER)

#ifdef __cplusplus
}
#endif

#endif /* INSTRUMENTR_OBJECT_H */
