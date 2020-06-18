KHASH_MAP_INIT_INT(otuh, uint32_t *)
KHASH_MAP_INIT_INT(idh, char *)

typedef struct {
    uint32_t otu;
    uint32_t spread;
    uint32_t sum;
} otucount_t;
KHASH_MAP_INIT_INT(statsh, otucount_t *)

typedef struct {
    khash_t(otuh)   *h;            //OTU count hash djb2(OTUIDs) - uint32 *counts
    khash_t(statsh) *sh;           //OTU stats hash djb2(OTUIDs) - otucount_t
    khash_t(idh)    *ids;          //OTU id hash    djb2(OTUIDs) - OTUIDs
    khash_t(idh)    *sampleids;    //sampleindex hash uint32     - sampleIDs)
    otucount_t *countspersample;   //{djb2(OTUid), spread, sum}
    //int numsamples;              //Number of samples sampleids length
} otut_t;


typedef struct {
    const char *otutname;
    const char *matchtname;
    const char *minratiot;
    double minrelcoo;
    double minratio;
    double minmatch;
} luluparam_t;


void lulu_paramRinit(luluparam_t *lulup,
                     SEXP Rotutname,
                     SEXP Rmatchtname,
                     SEXP Rminmatch,
                     SEXP Rminrelcoo,
                     SEXP Rminratio,
                     SEXP Rminrt);


void lulu_paramprint(luluparam_t lulup);


SEXP lulu_run(luluparam_t lulup);


int lulu_createdframe(SEXP ret, int idx);


int lulu_Rinit(luluparam_t lulup,
               otut_t  *otutable,
               khash_t(otuh) **curationtable);


void lulu_Rclose(khash_t(otuh) *curationtable, otut_t otutable);


int lulu_khash2dataframe(SEXP ret, khash_t(otuh) *htable, otut_t otutable);
