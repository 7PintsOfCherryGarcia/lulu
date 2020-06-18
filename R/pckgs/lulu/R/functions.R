lulu <- function(otutable,
                 matchlist,
                 minimum_match = 84,
                 minimum_relative_cooccurence = 0.95,
                 minimum_ratio = 1,
                 minimum_ratio_type = "min")
{
    #Check parameters are of correct types
    lulu_checkstr(otutable, "otutable")
    otutable <- lulu_checkpath(otutable)
    lulu_checkfile(otutable)

    lulu_checkstr(matchlist, "matchlist")
    matchlist <- lulu_checkpath(matchlist)
    lulu_checkfile(matchlist)

    lulu_checkstr(minimum_ratio_type, "minimum_ratio_type")
    minimum_ratio_type <- lulu_checkratiotype(minimum_ratio_type)

    lulu_checkreal(minimum_match, "minimum_match")
    lulu_checkreal(minimum_relative_cooccurence, "minimum_relative_cooccurance")
    lulu_checkreal(minimum_ratio, "minimum_ratio")



    ret <- .Call("lulu", otutable,
                         matchlist,
                         minimum_match,
                         minimum_relative_cooccurence,
                         minimum_ratio,
                         minimum_ratio_type,
                         PACKAGE="lulu")

    ret$curated_table <- t(ret$curated_table)
    return(ret)
}


lulu_checkstr <- function(lulustr, parameter)
{
    if(!is.character(lulustr))
    {
        stop("[LULU ERR] parameter: ", parameter, " = ", lulustr, " not a character type.")
    }
}


lulu_checkreal <- function(luluval, parameter)
{
    if( !is.numeric(luluval) )
    {
        stop("[LULU ERR] parameter: ", parameter, " = ", luluval, " not a numeric type.")
    }
}


lulu_checkratiotype <- function(luluval)
{
    if (luluval %in% c("MIN", "min", "minimum"))
    {
        return("min")
    }
    if (luluval %in% c("mean", "MEAN"))
    {
        return("mean")
    }
    stop("[LULU ERR] minimum_ratio_type: ", luluval, " not valid.")
}


lulu_checkfile <- function(filestr)
{
    if (file.access(filestr, 4) != 0 )
    {
        stop("[LULU ERR] file: ",
             filestr,
             " could not be read. Make sure it exists and you have read permission.\n")
    }
}


lulu_checkpath <- function(pathstr)
{
    if ( grepl("~", pathstr, fixed=TRUE) )
    {
        retstr <- gsub("~", Sys.getenv("HOME", pathstr), pathstr)
        return(retstr)
    }
    return(pathstr)
}
