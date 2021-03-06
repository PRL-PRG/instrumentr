#ifndef INSTRUMENTR_R_INCLUDES_H
#define INSTRUMENTR_R_INCLUDES_H

#include <R.h>
#include <R_ext/Error.h>
#include <Rdefines.h>
#include <Rinternals.h>
#ifdef USING_DYNTRACE
#    include <Rdyntrace.h>
#endif
#undef length

#endif /* INSTRUMENTR_R_INCLUDES_H */
