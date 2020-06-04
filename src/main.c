/*
                 GNU LESSER GENERAL PUBLIC LICENSE
                       Version 3, 29 June 2007

 Copyright (C) 2007 Free Software Foundation, Inc. <http://fsf.org/>
 Everyone is permitted to copy and distribute verbatim copies
 of this license document, but changing it is not allowed.


  This version of the GNU Lesser General Public License incorporates
the terms and conditions of version 3 of the GNU General Public
License, supplemented by the additional permissions listed below.

  0. Additional Definitions.

  As used herein, "this License" refers to version 3 of the GNU Lesser
General Public License, and the "GNU GPL" refers to version 3 of the GNU
General Public License.

  "The Library" refers to a covered work governed by this License,
other than an Application or a Combined Work as defined below.

  An "Application" is any work that makes use of an interface provided
by the Library, but which is not otherwise based on the Library.
Defining a subclass of a class defined by the Library is deemed a mode
of using an interface provided by the Library.

  A "Combined Work" is a work produced by combining or linking an
Application with the Library.  The particular version of the Library
with which the Combined Work was made is also called the "Linked
Version".

  The "Minimal Corresponding Source" for a Combined Work means the
Corresponding Source for the Combined Work, excluding any source code
for portions of the Combined Work that, considered in isolation, are
based on the Application, and not on the Linked Version.

  The "Corresponding Application Code" for a Combined Work means the
object code and/or source code for the Application, including any data
and utility programs needed for reproducing the Combined Work from the
Application, but excluding the System Libraries of the Combined Work.

  1. Exception to Section 3 of the GNU GPL.

  You may convey a covered work under sections 3 and 4 of this License
without being bound by section 3 of the GNU GPL.

  2. Conveying Modified Versions.

  If you modify a copy of the Library, and, in your modifications, a
facility refers to a function or data to be supplied by an Application
that uses the facility (other than as an argument passed when the
facility is invoked), then you may convey a copy of the modified
version:

   a) under this License, provided that you make a good faith effort to
   ensure that, in the event an Application does not supply the
   function or data, the facility still operates, and performs
   whatever part of its purpose remains meaningful, or

   b) under the GNU GPL, with none of the additional permissions of
   this License applicable to that copy.

  3. Object Code Incorporating Material from Library Header Files.

  The object code form of an Application may incorporate material from
a header file that is part of the Library.  You may convey such object
code under terms of your choice, provided that, if the incorporated
material is not limited to numerical parameters, data structure
layouts and accessors, or small macros, inline functions and templates
(ten or fewer lines in length), you do both of the following:

   a) Give prominent notice with each copy of the object code that the
   Library is used in it and that the Library and its use are
   covered by this License.

   b) Accompany the object code with a copy of the GNU GPL and this license
   document.

  4. Combined Works.

  You may convey a Combined Work under terms of your choice that,
taken together, effectively do not restrict modification of the
portions of the Library contained in the Combined Work and reverse
engineering for debugging such modifications, if you also do each of
the following:

   a) Give prominent notice with each copy of the Combined Work that
   the Library is used in it and that the Library and its use are
   covered by this License.

   b) Accompany the Combined Work with a copy of the GNU GPL and this license
   document.

   c) For a Combined Work that displays copyright notices during
   execution, include the copyright notice for the Library among
   these notices, as well as a reference directing the user to the
   copies of the GNU GPL and this license document.

   d) Do one of the following:

       0) Convey the Minimal Corresponding Source under the terms of this
       License, and the Corresponding Application Code in a form
       suitable for, and under terms that permit, the user to
       recombine or relink the Application with a modified version of
       the Linked Version to produce a modified Combined Work, in the
       manner specified by section 6 of the GNU GPL for conveying
       Corresponding Source.

       1) Use a suitable shared library mechanism for linking with the
       Library.  A suitable mechanism is one that (a) uses at run time
       a copy of the Library already present on the user's computer
       system, and (b) will operate properly with a modified version
       of the Library that is interface-compatible with the Linked
       Version.

   e) Provide Installation Information, but only if you would otherwise
   be required to provide such information under section 6 of the
   GNU GPL, and only to the extent that such information is
   necessary to install and execute a modified version of the
   Combined Work produced by recombining or relinking the
   Application with a modified version of the Linked Version. (If
   you use option 4d0, the Installation Information must accompany
   the Minimal Corresponding Source and Corresponding Application
   Code. If you use option 4d1, you must provide the Installation
   Information in the manner specified by section 6 of the GNU GPL
   for conveying Corresponding Source.)

  5. Combined Libraries.

  You may place library facilities that are a work based on the
Library side by side in a single library together with other library
facilities that are not Applications and are not covered by this
License, and convey such a combined library under terms of your
choice, if you do both of the following:

   a) Accompany the combined library with a copy of the same work based
   on the Library, uncombined with any other library facilities,
   conveyed under the terms of this License.

   b) Give prominent notice with the combined library that part of it
   is a work based on the Library, and explaining where to find the
   accompanying uncombined form of the same work.

  6. Revised Versions of the GNU Lesser General Public License.

  The Free Software Foundation may publish revised and/or new versions
of the GNU Lesser General Public License from time to time. Such new
versions will be similar in spirit to the present version, but may
differ in detail to address new problems or concerns.

  Each version is given a distinguishing version number. If the
Library as you received it specifies that a certain numbered version
of the GNU Lesser General Public License "or any later version"
applies to it, you have the option of following the terms and
conditions either of that published version or of any later version
published by the Free Software Foundation. If the Library as you
received it does not specify a version number of the GNU Lesser
General Public License, you may choose any version of the GNU Lesser
General Public License ever published by the Free Software Foundation.

  If the Library as you received it specifies that a proxy can decide
whether future versions of the GNU Lesser General Public License shall
apply, that proxy's public statement of acceptance of any version is
permanent authorization for you to choose that version for the
Library.
*/


#include <stdio.h>
#include <stdlib.h>
#include "lulu.h"



int main(int argc, char *argv[])
{
    if (argc < 3) usage();
    fprintf(stderr, "lulu C port v0.0\n");

    char *otufilename = argv[1];
    otut_t otutable = {NULL, NULL, NULL, NULL, NULL, 0};
    if (!lulu_readotus(otufilename, &otutable)) usage();
    // Compute spread (Number of samles with > 1 read per sample)
    lulu_computespread(&otutable);

    char *matchfilename = argv[2];
    khash_t(matchh) *matchtable = NULL;
    if (!lulu_readmatches(matchfilename, &matchtable)) usage();
    //lulu_printmatches(matchtable, otutable);

    //Start main lulu algorithm
    khash_t(parenth) *parents =  lulu_startlulu(otutable, matchtable);
    lulu_curationtable(otutable, parents);

    lulu_cleanmatchtable(matchtable);
    lulu_cleanotutable(otutable);
}


void lulu_curationtable(otut_t otutable, khash_t(parenth) *parents)
{
    int numsamples = otutable.numsamples;
    khash_t(otuh) *curationtable = kh_init(otuh);
    otucount_t otu;
    uint32_t *otucounts;
    khint_t otuk;
    khint_t parentk;
    uint32_t parent;
    uint32_t *parentcount;
    int absent;
    // Loop over otus from greatest spread and total
    for (int i = kh_size(otutable.h) - 1; i >= 0; i--) {
        otu = otutable.countspersample[i];
        // Extract counts
        otuk = kh_get(otuh, otutable.h, otu.otu);
        otucounts = kh_val(otutable.h, otuk);

        // Extract OTU's parent
        parentk = kh_get(parenth, parents, otu.otu);
        parent = kh_val(parents, parentk);
        // Test if parent has been added to curation table
        parentk = kh_get(otuh, curationtable, parent);
        if (parentk == kh_end(curationtable)) {
            // Add new entry to curated table
            parentk = kh_put(otuh, curationtable, parent, &absent);
            kh_val(curationtable, parentk) = otucounts;
            //Delete otu entry from otutable
            kh_del(otuh, otutable.h, otuk);

        }
        else {
            // Add otu counts to parent's entry
            parentcount = kh_val(curationtable, parentk);
            for (int i = 0; i < numsamples; i++) {
                parentcount[i] += otucounts[i];
            }
        }
    }
    fprintf(stderr, "Size of curation table: %u\n", kh_size(curationtable));


    lulu_printcurationtable(curationtable, otutable.ids ,numsamples);
    for (otuk = kh_begin(curationtable); otuk != kh_end(curationtable); ++otuk) {
        if (kh_exist(curationtable, otuk)) {
            free(kh_value(curationtable, otuk));
        }
    }
    kh_destroy(otuh, curationtable);
    kh_destroy(parenth, parents);
}



void lulu_printcurationtable(khash_t(otuh) *curationtable,
                             khash_t(idh) *ids,
                             int numsamples)
{
    khint_t k, kid;
    uint32_t otudjb;
    char *otuname;
    uint32_t *counts;
    for (k = kh_begin(curationtable); k != kh_end(curationtable); ++k) {
        if (kh_exist(curationtable, k)) {
            counts = kh_value(curationtable, k);
            //get key otudjb
            otudjb = kh_key(curationtable, k);
            kid = kh_get(idh, ids, otudjb);
            otuname = kh_val(ids, kid);
            fprintf(stdout, "%s", otuname);
            for (int i = 0; i < numsamples; i++) {
                fprintf(stdout, "\t%d", counts[i]);
            }
            fprintf(stdout, "\n");
        }
    }
}


khash_t(parenth) *lulu_startlulu(otut_t otutable, khash_t(matchh) *matchtable)
{
    char success;
    otucount_t otu;
    //char *otuname;
    match_t *matches;
    int nmatches;
    khash_t(parenth) *parents = kh_init(parenth);
    khint_t k;
    int absent;
    int numcurated = 0;
    //Loop over otu's from highest to lowest spread
    for (int i = kh_size(otutable.h) - 1; i >= 0; i--) {
        // Extract OTU
        otu = otutable.countspersample[i];
        //k = kh_get(idh, otutable.ids, otu.otu);
        //otuname = kh_val(otutable.ids, k);
        //Get matches
        matches = lulu_getotumatches(matchtable, otu.otu, &nmatches);
        if ( !matches ) {
            k = kh_put(parenth, parents, otu.otu, &absent);
            kh_val(parents, k) = otu.otu;
            numcurated++;
            //fprintf(stdout, "%s\t%s\n", otuname, otuname);
            continue;
        }

        //Analyze matches
        success = lulu_parsematches(matches, nmatches, otutable, otu, parents);

        if (!success) {
            //Add Current otu as parent.
            k = kh_put(parenth, parents, otu.otu, &absent);
            kh_val(parents, k) = otu.otu;
            numcurated++;
            //fprintf(stdout, "%s\t%s\n", otuname, otuname);
        }
    }
    fprintf(stderr, "Size of parents: %u uique: %d\n", kh_size(parents), numcurated);
    return parents;
}


match_t *lulu_getotumatches(khash_t(matchh) *matchtable, uint32_t otudjb, int *nmatches)
{
    match_t *matches;
    khint_t k;
    k = kh_get(matchh, matchtable, otudjb);
    if (k == kh_end(matchtable)) {
        return NULL;
    }
    matches = kh_val(matchtable, k);
    *nmatches = matches[0].length;
    return matches;
}


void lulu_computerelab(uint32_t *counts,
                       uint32_t *matchcounts,
                       float *coocurance,
                       float *minratio,
                       int numsamples)
{
    float doccur = 0;
    float poccur = 0;
    float ratiosum = 0;
    float ratio = 0;
    *minratio = 1.4e+38F;;
    // loop over counts
    for (int k = 0; k < numsamples; k++) {
        if (counts[k] > 0) {
            doccur++;
            poccur += (matchcounts[k] > 0);
            ratio = (float)matchcounts[k] / (float)counts[k];
            ratiosum += ratio;
            if ( ratio < *minratio) *minratio = ratio;
        }
    }
    *coocurance = poccur / doccur;
}


void lulu_addparent(uint32_t djbotu, uint32_t djbmatch, khash_t(parenth) *parents)
{
    khint_t kmatch;
    khint_t kotu;
    int absent;
    //uint32_t djbparent;
    kotu = kh_put(parenth, parents, djbotu, &absent);
    //Test if match has parent already
    kmatch = kh_get(parenth, parents, djbmatch);
    if (kmatch == kh_end(parents)) {
        //Add match as parent
        kh_val(parents, kotu) = djbmatch;
        //djbparent = djbmatch;
    }
    else {
        //Get match's parent and add as this otu's parent
        //djbparent = kh_val(parents, kmatch);
        kh_val(parents, kotu) = kh_val(parents, kmatch);
    }
    //return djbparent;
}


char lulu_parsematches(match_t *matches,
                       int nmatches,
                       otut_t otutable,
                       otucount_t otu,
                       khash_t(parenth) *parents)
{
    int numsamples = otutable.numsamples;
    char success = 0;
    uint32_t djbmatch;
    khint_t k;
    otucount_t *matchstat;
    uint32_t *counts;
    uint32_t *matchcounts;
    float coocurance;
    float ratio;
    uint32_t prevspread = 0;
    uint32_t prevsum = 0;
    uint32_t validsum = 0;
    //char *matchname;
    //uint32_t djbparent;
    for (int n = 0; n < nmatches; n++) {
        djbmatch = matches[n].djbmatch;
        //Get stats (spread and sum)
        k = kh_get(statsh, otutable.sh, djbmatch);
        matchstat = kh_val(otutable.sh, k);

        // if match spread greater than current otu and has greater spread overall
        if ( ( matchstat->spread>=otu.spread ) & ( matchstat->spread>=prevspread ) ) {
            //If sample with greater spread, update previous sum
            if (matchstat->spread > prevspread) {
                validsum = prevsum;
                prevsum = 0;
            }
            //Get otu counts
            k = kh_get(otuh, otutable.h, otu.otu);
            counts = kh_val(otutable.h, k);

            //Get match counts
            k = kh_get(otuh, otutable.h, djbmatch);
            matchcounts = kh_val(otutable.h, k);

            //Compute relative cooccurance
            lulu_computerelab(counts, matchcounts, &coocurance, &ratio, numsamples);

            if ((coocurance > .90) & (ratio >= 1.0) & (matchstat->sum >= prevsum)) {
                //Update values
                prevspread = matchstat->spread;
                prevsum = matchstat->sum;
                validsum = prevsum;
                success = 1;
                //Add otu to parents hash
                lulu_addparent(otu.otu, djbmatch, parents);
            }
            else {
                prevsum = validsum;
            }
        }
    }
    //if (success) {
    //    k = kh_get(idh, otutable.ids, djbparent);
    //    matchname = kh_val(otutable.ids, k);
        //fprintf(stdout, "%s\t%s\n", otuname, matchname);
    //}
    return success;
}


void lulu_sumsort(otut_t *otutable)
{
    uint32_t count = 1;
    uint32_t prev = otutable->countspersample[0].spread;
    for (uint32_t i = 1; i < kh_size(otutable->h); i++) {
        if (prev == otutable->countspersample[i].spread) {
            count++;
            continue;
        }
        else if (count > 1) {
            ks_mergesort(sum,
                         count,
                         otutable->countspersample+i-count,
                         0);
            count = 1;
        }
        prev = otutable->countspersample[i].spread;
    }
}


void lulu_computespread(otut_t *otutable)
{
    otutable->countspersample = malloc(kh_size(otutable->h)*sizeof(otucount_t));
    // Loop over otus
    khint_t sk;
    uint32_t otu;
    uint32_t *counts;
    uint32_t sum;
    uint32_t spread;
    int i = 0;
    int absent;
    // Loop over OTUs
    for (khint_t k = kh_begin(otutable->h); k!= kh_end(otutable->h); ++k) {
        if (kh_exist(otutable->h, k)) {
            sum = 0;
            spread = 0;

            // Get otu count data
            otu = kh_key(otutable->h, k);
            counts = kh_val(otutable->h, k);

            // Compute spread and sum over samples
            for (int j = 0; j < otutable->numsamples; j++) {
                sum += *(counts + j);
                spread += (*(counts + j) >= 1);
            }
            //kotu = kh_get(idh, otutable->ids, otu);
            //name = kh_val(otutable->ids, kotu);
            //fprintf(stdout, "%s\t%u\t%u\n", name, spread, sum);

            otutable->countspersample[i] = (otucount_t){otu , spread, sum};
            //sk = kh_put(statsh, otutable->sh, otu, &absent);
            //kh_val(otutable->sh, sk) = otutable->countspersample + i;
            i++;
        }
    }
    //Sort by spread
    ks_mergesort(spread, kh_size(otutable->h), otutable->countspersample, 0);
    //Sort by sum
    lulu_sumsort(otutable);

    //Create stats hash //TODO move to function
    otutable->sh = kh_init(statsh);
    for (i = 0; i < kh_size(otutable->h); i ++) {
        sk = kh_put(statsh, otutable->sh, otutable->countspersample[i].otu, &absent);
        kh_val(otutable->sh, sk) = otutable->countspersample + i;
    }
}


//Requires otu table be in the format:
// #OTUid\tsampl1\tsample2\t.......sampleN\n
uint32_t *lulu_readsampleids(char *samplearray, int *numsamples)
{
    int sampleidx = 0;
    char *samplename;
    uint32_t *sampleids = malloc(1000*sizeof(uint32_t));
    samplename = strtok(samplearray, "\t");
    sampleids[sampleidx++] = djb2hash(samplename);
    while ( (samplename = strtok(NULL, "\t")) ) {
        sampleids[sampleidx++] = djb2hash(samplename);
    }
    *numsamples = sampleidx - 1;
    sampleids = realloc(sampleids, sampleidx*sizeof(uint32_t));
    return sampleids;
}


void lulu_addotu(khash_t(otuh) *h, khash_t(idh) *ids, char *otusamplestring, int numsamples)
{
    uint32_t *otucounts = malloc(numsamples*sizeof(uint32_t));
    char *count;
    char *samplename;
    khint_t k;
    int absent;
    int numcounts = 0;
    uint32_t namehash;
    samplename = strtok(otusamplestring, "\t");
    namehash = djb2hash(samplename);
    while ( (count = strtok(NULL, "\t")) ) {
        otucounts[numcounts++] = (uint32_t)strtol(count, NULL, 10);
    }
    if (numcounts != numsamples) {
        fprintf(stderr, "ERROR: Number of counts does not match number of samples.\n");
    }
    k = kh_put(otuh, h, namehash, &absent);
    kh_val(h, k) = otucounts;

    k = kh_put(idh, ids, namehash, &absent);
    kh_val(ids, k) = strdup(samplename);
}


int lulu_readotus(const char *filename, otut_t *otutable)
{
    int ret = 0;
    gzFile fp;
    kstream_t *ks = NULL;
    kstring_t str = {0,0,0};
    fp = gzopen(filename, "r");
    ks = ks_init(fp);
    if (!ks) goto exit;

    khash_t(otuh) *h = kh_init(otuh);
    khash_t(idh)  *ids = kh_init(idh);
    if (!(h)) goto exit;

    int firstflag = 0;
    int numsamples;
    while (ks_getuntil(ks, '\n', &str, 0) >= 0) {
        if (!firstflag) {
            //TODO check for errors
            otutable->sampleids = lulu_readsampleids(str.s, &numsamples);
            fprintf(stderr, "%d samples\n", numsamples);
            otutable->numsamples = numsamples;
            firstflag = 1;
            continue;
        }
        lulu_addotu(h, ids, str.s, numsamples);
    }
    otutable->h = h;
    otutable->ids = ids;
    ret = 1;
    exit:
        ks_destroy(ks);
        gzclose(fp);
        free(str.s);
        return ret;
}


void lulu_cleanotutable(otut_t otutable)
{
    free(otutable.sampleids);
    kh_destroy(statsh, otutable.sh);
    khint_t k;
    for (k = kh_begin(otutable.ids); k!= kh_end(otutable.ids); ++k) {
        if (kh_exist(otutable.ids, k)) {
            free(kh_value(otutable.ids, k));
        }
    }
    for (k = kh_begin(otutable.h); k!= kh_end(otutable.h); ++k) {
        if (kh_exist(otutable.h, k)) {
            free(kh_value(otutable.h, k));
        }
    }
    kh_destroy(idh, otutable.ids);
    kh_destroy(otuh, otutable.h);
    free(otutable.countspersample);
}


void lulu_printotus(otut_t *otutable)
{
    uint32_t otu;
    uint32_t sum;
    uint32_t spread;
    for (int i = 0; i < kh_size(otutable->h); i++) {
        sum = otutable->countspersample[i].sum;
        spread = otutable->countspersample[i].spread;
        otu = otutable->countspersample[i].otu;
        fprintf(stderr, "%u %u %u\n", otu, sum, spread);
    }

}


void lulu_printmatches(khash_t(matchh) *matchtable, otut_t otutable)
{
    khint_t k;
    uint32_t otuid;
    match_t *match;
    uint32_t spread = 0;
    for (k = kh_begin(matchtable); k!= kh_end(matchtable); ++k) {
        if (kh_exist(matchtable, k)) {
            otuid = kh_key(matchtable, k);
            match = kh_val(matchtable, k);
            fprintf(stderr, "%u %u:\n", otuid, match->length);
            for (int i = 0; i < match->length; i++) {
                //k = kh_get(statsh, otutable.sh, (match + i)->djbmatch);
                //spread = (kh_val(otutable.sh, k))->spread;
                fprintf(stderr, "\t%u | %f | %u\n",
                        (match + i)->djbmatch, (match + i)->matchpc, spread);
            }
        }
    }
    fprintf(stderr, "match list size: %u\n", kh_size(matchtable));
}


void lulu_cleanmatchtable(khash_t(matchh) *matchtable)
{
    khint_t k;
    for (k = kh_begin(matchtable); k!= kh_end(matchtable); ++k) {
        if (kh_exist(matchtable, k)) {
            free(kh_value(matchtable, k));
        }
    }
    kh_destroy(matchh, matchtable);
}


void lulu_newotu(match_t otu, khint_t k, khash_t(matchh) *h)
{
    kh_val(h, k) = malloc(1000*sizeof(match_t));
    memcpy(kh_val(h, k), &otu, sizeof(match_t));
    kh_val(h, k)->length = 1;
}


void lulu_addmatch(match_t otu, khint_t k, khash_t(matchh) *h)
{
    match_t *matches = kh_val(h, k);
    uint32_t length = matches->length;
    memcpy(matches + length, &otu, sizeof(match_t));
    length+=1;
    for (uint32_t i = 0; i < length; i++) {
        matches[i].length = length;
    }
    if (!(length % 1000)) {
        matches = realloc(matches, (length+100)*sizeof(match_t));
        if (!matches) fprintf(stderr, "ERRORRA.\n");
        kh_val(h, k) = matches;
    }

}


int lulu_readmatches(const char *filename, khash_t(matchh) **h)
{
    int ret = 0;
    gzFile fp;
    kstream_t *ks = NULL;
    kstring_t str = {0,0,0};
    fp = gzopen(filename, "r");
    ks = ks_init(fp);
    if (!ks) goto exit;
    *h = kh_init(matchh);
    if (!(*h)) goto exit;
    khint_t k;
    int absent;

    char *otuid;
    char *matchid;
    char *pctmatch;
    float pct;
    int numm = 0;
    while (ks_getuntil(ks, '\n', &str, 0) >= 0) {
        otuid = strtok(str.s, "\t");
        matchid = strtok(NULL, "\t");
        pctmatch = strtok(NULL, "\t");
        if (!otuid | !matchid | !pctmatch | !strcmp(otuid, matchid)) {
            continue;
        }

        numm++;
        pct = strtof(pctmatch, NULL);
        if ( pct <= 90.0 ) continue;
        match_t otu = {djb2hash(matchid), pct};
        k = kh_put(matchh, *h, djb2hash(otuid), &absent);
        if (!absent) { // ID already in table
            lulu_addmatch(otu, k, *h);
        }
        else {
            lulu_newotu(otu, k, *h);
        }
    }

    ret = 1;
    exit:
        ks_destroy(ks);
        gzclose(fp);
        free(str.s);
        return ret;
}


uint32_t djb2hash(char *str)
{
    /*
      djb2 hash after Dan Bernstein, see http://www.cse.yorku.ca/~oz/hash.html
    */

    uint32_t hash = 5381;
    int c;

    while ((c = *str++))
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

    return hash;
}


void usage()
{
    fprintf(stderr, "ERROR\n");
    exit(-1);
}

