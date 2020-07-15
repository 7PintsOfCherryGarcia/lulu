#include <zlib.h>
#include <stdint.h>
#include "klib/kseq.h"
#include "klib/khash.h"
#include "klib/ksort.h"

//TODO
//Check for invalid input in match table
//Check for self hits in match table

KHASH_MAP_INIT_INT(otuh, uint32_t *)
typedef struct {
    uint32_t otu;
    uint32_t spread;
    uint32_t sum;
} otucount_t;
KHASH_MAP_INIT_INT(statsh, otucount_t *)

KHASH_MAP_INIT_INT(idh, char *)
typedef struct {
    khash_t(otuh)   *h;            //OTU count hash djb2(OTUIDs) - uint32 *counts
    khash_t(statsh) *sh;           //OTU stats hash djb2(OTUIDs) - otucount_t
    khash_t(idh)    *ids;          //OTU id hash    djb2(OTUIDs) - OTUIDs
    khash_t(idh)    *sampleids;    //sampleindex hash uint32     - sampleIDs)
    otucount_t *countspersample;   //{djb2(OTUid), spread, sum}
    //int numsamples;              //Number of samples sampleids length
} otut_t;

#define spread_lt(a, b) ((a).spread < (b).spread)
#define sum_lt(a, b) ((a).sum < (b).sum)
KSORT_INIT(spread, otucount_t, spread_lt)
KSORT_INIT(sum, otucount_t, sum_lt)
KSORT_INIT_GENERIC(uint32_t)

typedef struct {
    uint32_t djbmatch;
    float    matchpc;
    uint32_t length;
} match_t;
KHASH_MAP_INIT_INT(matchh, match_t*)

KHASH_MAP_INIT_INT(parenth, uint32_t)

KSTREAM_INIT(gzFile, gzread, 16384)

typedef struct {
    const char *otutname;
    const char *matchtname;
    const char *minratiot;
    double minrelcoo;
    double minratio;
    double minmatch;
} luluparam_t;


void usage();
uint32_t djb2hash(char *str);
khash_t(parenth) *lulu_startlulu(otut_t otutable, khash_t(matchh) *matchtable);

khash_t(idh) *lulu_readsampleids(char *samplearray);
void lulu_addotu(khash_t(otuh) *h, khash_t(idh) *idh, char *otusamplestrin, int numsamples);
int lulu_readotus(const char *filename, otut_t *otutable);
void lulu_cleanotutable(otut_t otutable);
void lulu_computespread(otut_t *otutable);
void lulu_sumsort(otut_t *otutable);
void lulu_printotus(otut_t *otutable);
void lulu_printmatches(khash_t(matchh) *matchtable, otut_t otutable);
void lulu_sortmatches(khash_t(matchh) *matchtable);

void lulu_newotu(match_t otu, khint_t k, khash_t(matchh) *h);
void lulu_addmatch(match_t otu, khint_t k, khash_t(matchh) *h);
int lulu_readmatches(const char *filename, khash_t(matchh) **h);
void lulu_cleanmatchtable(khash_t(matchh) *matchtable);

match_t *lulu_getotumatches(khash_t(matchh) *matchtable, uint32_t otudjb, int *nmatches);
void lulu_computerelab(uint32_t *counts,
                       uint32_t *matchcounts,
                       float *coocurance,
                       float *minratio,
                       int numsamples);
void lulu_addparent(uint32_t djbotu, uint32_t djbmatch, khash_t(parenth) *parents);
char lulu_parsematches(match_t *matches,
                       int nmatches,
                       otut_t otutable,
                       otucount_t otu,
                       khash_t(parenth) *parents);

khash_t(otuh) *lulu_curationtable(otut_t otutable, khash_t(parenth) *parents);
void lulu_printcurationtable(khash_t(otuh) *curationtable,
                             khash_t(idh) *ids,
                             int numsamples);
int lulu_init(luluparam_t lulup);

//khash_t(otuh) *lulu_Rinit(luluparam_t lulup, int *nsamples);
//void lulu_Rclose(khash_t(otuh) *curationtable);
