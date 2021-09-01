//#: Title        :Package for calling hierarchy of domains
//#: Date         :10.05.2016
//#: Author       :Zhan Yinxiu
//#: Version      :1.0.0

#include "CaTCH.h"

float max(float a, float b)
{

	if (a > b)
		return a;
	else
		return b;
}

float min(float a, float b)
{

	if (a < b)
		return a;
	else
		return b;
}
//calculate total counts
float sum(int i, int j, unsigned short **mat)
{
	float x = 0, h;
	int k, l;

	for (k = i; k <= j; k++)
		for (l = i; l <= j; l++)
			x += mat[k][l];
	return x;
}

float dist(int i1, int j1, int i2, int j2, unsigned short **mat)
{
	float x = 0, v = 0, di = 0, d1 = 0, d2 = 0;
	int k, l;
	for (k = i1; k <= j1; k++)
		for (l = i2; l <= j2; l++)
			if (k != l && abs(l - k) >= MINDIST)
				x += mat[k][l];

	v = (j1 - i1 + 1) * (j2 - i2 + 1) - 1;

	for (k = i1; k <= j1; k++)
		for (l = i1; l <= j1; l++)
			if (k != l && abs(l - k) >= MINDIST)
				d1 += mat[k][l];

	for (k = i2; k <= j2; k++)
		for (l = i2; l <= j2; l++)
			if (k != l && abs(l - k) >= MINDIST)
				d2 += mat[k][l];

	di = (x / v) / ((d1 + d2) / ((j1 - i1 + 1) * (j1 - i1) + (j2 - i2 + 1) * (j2 - i2)));

	return di;
}

SEXP catch (SEXP input)
{

	int i, j, k, id, joined, imin = 99999, size = 0, tot = 0, appo = 0;
	//float **insulation;
	float dt, p[ND + 1], prevdist = 0, newdist = 0;
	int nrow, ncol;

	unsigned short **cfrom, **cto, *ncl;
	unsigned short **mat;
	float **insulation; // store insulation scores
	SEXP out, attrib, prof, ncluster;
	FILE *fp;

	nrow = INTEGER(Rf_getAttrib(input, R_DimSymbol))[0];
	ncol = INTEGER(Rf_getAttrib(input, R_DimSymbol))[1];

	for (i = 0; i < nrow; i++)
		for (j = 0; j < 2; j++)
		{
			if ((j == 0 || j == 1) && REAL(input)[i + 2 * nrow] != -1)
			{
				if (REAL(input)[i + j * nrow] > size)
					size = REAL(input)[i + j * nrow];
				if (REAL(input)[i + j * nrow] < imin)
					imin = REAL(input)[i + j * nrow];
			}
		}

	size++;

	mat = (unsigned short **)calloc(size, sizeof(unsigned short *));
	for (i = 0; i < size; i++)
		mat[i] = (unsigned short *)calloc(size, sizeof(unsigned short));
	for (i = 0; i < size; i++)
		for (j = 0; j < size; j++)
			mat[i][j] = 0;

	insulation = (float **)calloc(ND + 1, sizeof(float *));
	for (i = 0; i < ND + 1; i++)
		insulation[i] = (float *)calloc(size, sizeof(float));

	for (i = 0; i < nrow; i++)
	{
		if (REAL(input)[i + 2 * nrow] != -1)
			mat[(int)REAL(input)[i + 0 * nrow]][(int)REAL(input)[i + 1 * nrow]] = (unsigned short)REAL(input)[i + 2 * nrow];
	}

	cfrom = (unsigned short **)calloc(ND + 1, sizeof(unsigned short *));
	for (i = 0; i < ND + 1; i++)
		cfrom[i] = (unsigned short *)calloc(size, sizeof(unsigned short));
	cto = (unsigned short **)calloc(ND + 1, sizeof(unsigned short *));
	for (i = 0; i < ND + 1; i++)
		cto[i] = (unsigned short *)calloc(size, sizeof(unsigned short));
	ncl = (unsigned short *)calloc(ND + 1, sizeof(unsigned short));

	for (i = 0; i < ND + 1; i++)
		ncl[i] = 0;
	for (i = 0; i < (int)(size - imin) / (MINDIST + 1); i++)
	{
		cfrom[0][i] = i * (1 + MINDIST) + imin;
		cto[0][i] = (i + 1) * (1 + MINDIST) + imin - 1;
		ncl[0]++;
		insulation[0][i] = -1.;
	}

	Rprintf("Clustering on different thresholds: \n");
	for (id = 1; id <= ND; id++) // increasing threshold
	{
		dt = (float)(ND - id) / ND;
		if (id % 100 == 0)
			Rprintf("Relative Insulation: %f\n", 1 - dt);
		for (i = 0; i < ncl[id - 1]; i++) // run on clusters
		{
			joined = -1;

			cfrom[id][ncl[id]] = cfrom[id - 1][i];
			cto[id][ncl[id]] = cto[id - 1][i];

			for (k = i + 1; k < ncl[id - 1]; k++) // clusters to join previous
			{

				if (dist(cfrom[id][ncl[id]], cto[id][ncl[id]], cfrom[id - 1][k], cto[id - 1][k], mat) >= dt)
				{
					cfrom[id][ncl[id]] = cfrom[id - 1][i];
					cto[id][ncl[id]] = cto[id - 1][k];

					joined = k;
				}
				else
				{
					insulation[id][ncl[id]] = 1 - dist(cfrom[id][ncl[id]], cto[id][ncl[id]], cfrom[id - 1][k], cto[id - 1][k], mat);
					break;
				}
			}

			if (joined == -1)
			{
				cfrom[id][ncl[id]] = cfrom[id - 1][i];
				cto[id][ncl[id]] = cto[id - 1][i];
				ncl[id]++;
			}
			else
			{

				ncl[id]++;
				i = joined;
			}
		}

		//movement
		for (i = 0; i < ncl[id] - 1; i++)
		{
			//except last
			if ((cto[id][i] - cfrom[id][i] > (2 * MAXMOVE) && cto[id][i + 1] - cfrom[id][i + 1] > (2 * MAXMOVE)) && (cto[id][i] - cfrom[id][i] > MINSIZE || cto[id][i + 1] - cfrom[id][i + 1] > MINSIZE))
			{
				prevdist = dist(cfrom[id][i], cto[id][i], cfrom[id][i + 1], cto[id][i + 1], mat);
				for (j = 1; j < MAXMOVE; j++)
				{
					newdist = dist(cfrom[id][i], cto[id][i] + j, cfrom[id][i + 1] + j, cto[id][i + 1], mat);
					if (newdist < prevdist)
					{
						prevdist = newdist;
						cto[id][i] = cto[id][i] + j;
						cfrom[id][i + 1] = cfrom[id][i + 1] + j;
					}
					newdist = dist(cfrom[id][i], cto[id][i] - j, cfrom[id][i + 1] - j, cto[id][i + 1], mat);

					if (newdist < prevdist)
					{
						prevdist = newdist;
						cto[id][i] = cto[id][i] - j;
						cfrom[id][i + 1] = cfrom[id][i + 1] - j;
					}
				}
			}
		}
	}
	Rprintf("\n");

	PROTECT(ncluster = Rf_allocMatrix(REALSXP, ND + 1, 2));

	for (i = 0; i < ND + 1; i++)
	{
		REAL(ncluster)
		[i + 0 * (ND + 1)] = (float)i / ND;
		REAL(ncluster)
		[i + 1 * (ND + 1)] = ncl[i];
	}

	tot = 0;
	for (i = 1; i <= ND; i++)
		for (j = 0; j < ncl[i]; j++)
			tot++;
	appo = 0;
	PROTECT(out = Rf_allocMatrix(REALSXP, tot, 4));

	for (i = 1; i <= ND; i++)
		for (j = 0; j < ncl[i]; j++)
		{
			REAL(out)
			[appo + tot * 0] = (float)i / ND;
			REAL(out)
			[appo + tot * 1] = cfrom[i][j];
			REAL(out)
			[appo + tot * 2] = cto[i][j];
			REAL(out)
			[appo + tot * 3] = insulation[i][j];
			appo++;
		}

	PROTECT(prof = Rf_allocVector(VECSXP, 2));
	PROTECT(attrib = Rf_allocVector(STRSXP, 2));
	SET_STRING_ELT(attrib, 0, Rf_mkChar("clusters"));
	SET_STRING_ELT(attrib, 1, Rf_mkChar("ncluster"));

	SET_VECTOR_ELT(prof, 0, out);
	SET_VECTOR_ELT(prof, 1, ncluster);
	Rf_setAttrib(prof, R_NamesSymbol, attrib);

	UNPROTECT(4);
	return prof;
}
