#include <stdio.h>
#include <R.h>
#include <Rinternals.h>

SEXP afun() {
    fprintf(stderr, "This is my first shared library.\n");
    SEXP ret, ans1, ans2, cls, nam, rownam;
    PROTECT(ret = Rf_allocVector(VECSXP, 2)); // a list with two elements

    PROTECT(ans1 = Rf_allocVector(INTSXP, 3)); // first column
    PROTECT(ans2 = Rf_allocVector(INTSXP, 3)); // second column

    for (int i=0; i<3; ++i) { // some data
        //Column 1
        INTEGER(ans1)[i] = i+1;
        //Column 2
        INTEGER(ans2)[i] = -(i+1);
    }
    SET_VECTOR_ELT(ret, 0, ans1);
    SET_VECTOR_ELT(ret, 1, ans2);
    PROTECT(cls = allocVector(STRSXP, 1)); // class attribute
    SET_STRING_ELT(cls, 0, mkChar("data.frame"));
    classgets(ret, cls);

    PROTECT(nam = allocVector(STRSXP, 2)); // names attribute (column names)
    SET_STRING_ELT(nam, 0, mkChar("a"));
    SET_STRING_ELT(nam, 1, mkChar("b"));
    namesgets(ret, nam);

    PROTECT(rownam = allocVector(STRSXP, 3)); // row.names attribute
    SET_STRING_ELT(rownam, 0, mkChar("1"));
    SET_STRING_ELT(rownam, 1, mkChar("2"));
    SET_STRING_ELT(rownam, 2, mkChar("3"));
    setAttrib(ret, R_RowNamesSymbol, rownam);
    UNPROTECT(6);
    fprintf(stderr, "All done.\n");
    return ret;
}
