// prevent remapping e.g. Ralloc, which causes conflicts under windows
#define STRICT_R_HEADERS 1

#include <R_ext/Rdynload.h>
#include "CaTCH.h"

static const R_CallMethodDef callMethods[] = {
    /* CaTCH.c */
    {"Catch", (DL_FUNC) &catch, 1},
    {NULL, NULL, 0}
};


void R_init_CaTCH(DllInfo *info)
{
    R_registerRoutines(info, NULL, callMethods, NULL, NULL);
}

void R_unload_CaTCH(DllInfo *info){}
