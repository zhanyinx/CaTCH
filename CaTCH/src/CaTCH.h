// prevent remapping of "Rf_length" to "length" in Rdefines.h, which clashes with fstream::length
#define R_NO_REMAP
// prevent remapping e.g. Ralloc, which causes conflicts under windows
#define STRICT_R_HEADERS 1

#include <Rdefines.h>
#include <R.h>
#include <Rinternals.h>
#include <Rmath.h>
#define ND	1000
#define MINSIZE	15
#define MAXMOVE	3
#define MINDIST 1

float max(float a, float b);
float min(float a, float b);
float sum(int i, int j, unsigned short **mat);
float dist(int i1, int j1, int i2, int j2, unsigned short **mat);
SEXP catch(SEXP input);


