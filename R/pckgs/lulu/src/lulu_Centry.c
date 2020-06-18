#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include <R.h>
#include <Rinternals.h>

#include "klib/khash.h"
#include "lulu_Centry.h"


SEXP lulu(SEXP Rotutname,
          SEXP Rmatchtname,
          SEXP Rminmatch,
          SEXP Rminrelcoo,
          SEXP Rminratio,
          SEXP Rminratiot) {

    SEXP ret = R_NilValue;
    luluparam_t lulup;
    lulu_paramRinit(&lulup,
                    Rotutname,
                    Rmatchtname,
                    Rminmatch,
                    Rminrelcoo,
                    Rminratio,
                    Rminratiot);

    lulu_paramprint(lulup);

    ret = lulu_run(lulup);

    exit:
        return(ret);
}


void lulu_paramRinit(luluparam_t *lulup,
                    SEXP Rotutname,
                    SEXP Rmatchtname,
                    SEXP Rminmatch,
                    SEXP Rminrelcoo,
                    SEXP Rminratio,
                    SEXP Rminratiot)
{
    lulup->otutname = CHAR(STRING_PTR(Rotutname)[0]);
    lulup->matchtname = CHAR(STRING_PTR(Rmatchtname)[0]);
    lulup->minmatch = (double)*REAL(Rminmatch);
    lulup->minrelcoo = (double)*REAL(Rminrelcoo);
    lulup->minratio = (double)*REAL(Rminratio);
    lulup->minratiot = CHAR(STRING_PTR(Rminratiot)[0]);
}


void lulu_paramprint(luluparam_t lulup)
{
    Rprintf("[LULU INFO]: LULU 0.0\n");
    Rprintf("[LULU INFO]: Parameters:\n");
    Rprintf("[LULU INFO]: \totu table filename: %s\n", lulup.otutname);
    Rprintf("[LULU INFO]: \tmatch table filename: %s\n", lulup.matchtname);
    Rprintf("[LULU INFO]: \tminimum match: %f\n", lulup.minmatch);
    Rprintf("[LULU INFO]: \tminimum relative cooccurance: %f\n", lulup.minrelcoo);
    Rprintf("[LULU INFO]: \tminimum ratio: %f\n", lulup.minratio);
    Rprintf("[LULU INFO]: \tminimum ratio type: %s\n", lulup.minratiot);
}


SEXP lulu_run(luluparam_t lulup)
{
    SEXP ret;
    int numelem = 0;
    PROTECT(ret = Rf_allocVector(VECSXP, 1)); // a list with 1 elements
    numelem++;

    //This is where lulu's algorithm is called on the passed parameters
    Rprintf("[LULU INFO]: Running lulu\n");
    otut_t otutable = {NULL, NULL, NULL, NULL, NULL};
    khash_t(otuh)   *curationtable;
    int numsamples;
    if (!lulu_Rinit(lulup, &otutable, &curationtable)) ret = R_NilValue;
    numsamples = kh_size(otutable.sampleids);
    fprintf(stderr, "||||numsamples: %d\n", numsamples);

    //Prepare R SEXP
    numelem += lulu_khash2dataframe(ret, curationtable, otutable);
    if (numelem == 1) fprintf(stderr, "[ERROR!!!!]\n");

    //lulu_Rclose(curationtable, otutable);
    Rprintf("[LULU INFO]: done\n");

    SEXP retnames;
    PROTECT(retnames = allocVector(STRSXP, 1));
    numelem++;
    SET_STRING_ELT(retnames, 0, mkChar("curated_table"));
    namesgets(ret, retnames);
    UNPROTECT(numelem);
    exit:
        return ret;
}


int lulu_khash2dataframe(SEXP ret, khash_t(otuh) *htable, otut_t otutable)
{
    int numelem = 0;
    SEXP curation_table, cls, nam, rownam;
    int numotus = kh_size(htable);
    uint32_t otuid;
    uint32_t otuidx;
    uint32_t *counts;
    char *name;
    khint_t kname;
    int numsamples = kh_size(otutable.sampleids);

    //otus as columns
    PROTECT(curation_table = Rf_allocVector(VECSXP, numotus));
    numelem++;
    PROTECT(nam = allocVector(STRSXP, numotus)); // names attribute (column names)
    numelem++;
    otuidx = 0;
    //Loop over otus columns
    for (khint_t k = kh_begin(htable); k != kh_end(htable); ++k) {
        if (kh_exist(htable, k)) {
            SEXP otuvec;
            PROTECT(otuvec = Rf_allocVector(INTSXP, numsamples));
            numelem++;
            otuid = kh_key(htable, k);
            kname = kh_get(idh, otutable.ids, otuid);
            name = kh_val(otutable.ids, kname);
            counts = kh_value(htable, k);
            //INTEGER(otuvec) = kh_value(htable, k);
            for (int i = 0; i < numsamples; i++) {
                INTEGER(otuvec)[i] = counts[i];
            }
            SET_VECTOR_ELT(curation_table, otuidx, otuvec);
            //Set column names
            SET_STRING_ELT(nam, otuidx++, mkChar(name));
        }
    }
    namesgets(curation_table, nam);

    PROTECT(cls = allocVector(STRSXP, 1));
    numelem++;
    SET_STRING_ELT(cls, 0, mkChar("data.frame"));
    classgets(curation_table, cls);

    PROTECT(rownam = allocVector(STRSXP, numsamples)); // row.names attribute
    numelem++;
    for (uint32_t i = 0; i < numsamples; i++) {
        kname = kh_get(idh, otutable.sampleids, i);
        name = kh_val(otutable.sampleids, kname);
        SET_STRING_ELT(rownam, i, mkChar(name));
    }
    setAttrib(curation_table, R_RowNamesSymbol, rownam);

    SET_VECTOR_ELT(ret, 0, curation_table);

    return numelem;
}


int lulu_createdframe(SEXP ret, int idx)
{
    SEXP curation_table, cls, nam, rownam;
    int numsamples = 4;
    int numotus = 10;
    char name[80];
    int numelem = 0;

    PROTECT(curation_table = Rf_allocVector(VECSXP, numsamples));
    for (int i = 0; i < numsamples; i++) {
        SEXP otuvec;
        PROTECT(otuvec = Rf_allocVector(INTSXP, numotus));
        numelem += 1;
        for (int j = 0; j < numotus; j++) {
            INTEGER(otuvec)[j] = j*4;
        }
        SET_VECTOR_ELT(curation_table, i, otuvec);
    }
    // class attribute
    PROTECT(cls = allocVector(STRSXP, 1));
    SET_STRING_ELT(cls, 0, mkChar("data.frame"));
    classgets(curation_table, cls);

    PROTECT(rownam = allocVector(STRSXP, numotus)); // row.names attribute
    for (int i = 0; i < numotus; i++) {
        sprintf(name, "%d", i);
        SET_STRING_ELT(rownam, i, mkChar(name));
    }
    setAttrib(curation_table, R_RowNamesSymbol, rownam);
    SET_VECTOR_ELT(ret, idx, curation_table);
    numelem += 4;
    return numelem;
}
