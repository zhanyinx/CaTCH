domain.call <- function(input) { # nolint
        hicdata <- read.table(input, nrows = 10)
        ncolumns <- ncol(hicdata)

        # check that each file contains a single chromosome
        stopifnot(length(unique(hicdata[, 1])) == 1)

        # check type of file
        if (ncolumns == 6) {
                original <- read.table(input, colClasses = c("character", "numeric", "numeric", "numeric", "numeric", "numeric")) # nolint
                hicdata <- original[, c(-2, -3)]
        } else if (ncolumns == 4) {
                original <- read.table(input, colClasses = c("character", "numeric", "numeric", "numeric")) # nolint
                hicdata <- original
        } else {
                stop("input file has the wrong format. It needs 4 or 6 columns")
        }

        # calling C function
        res <- .Call("Catch", as.matrix(hicdata[, -1]), PACKAGE = "CaTCH")
        colnames(res$ncluster) <- c("RI", "ndomains")
        colnames(res$clusters) <- c("RI", "start", "end", "insulation")
        res$ncluster <- data.frame(chromosome = unique(hicdata[, 1]), res$ncluster) # nolint
        res$clusters <- data.frame(chromosome = unique(hicdata[, 1]), res$clusters) # nolint
        return(res)
}

domain.call.parallel <- function(inputs, ncpu = 4) { # nolint
        # check input parameters
        stopifnot(is.numeric(ncpu) || ncpu < 1L)
        stopifnot(is.character(inputs))
        stopifnot(all(file.exists(inputs)))

        # start parallel R processes
        cl <- parallel::makeCluster(min(ncpu, length(inputs)))
        message("loading CaTCH on ", length(cl), " nodes...", appendLF = FALSE)
        res <- parallel::clusterEvalQ(cl, library(CaTCH))
        if (!all(sapply(res, function(x) "CaTCH" %in% x))) {
                stop("'CaTCH' could not be loaded on all cluster nodes")
        }
        message("done")

        # segment chromosomes
        res <- parLapply(cl, inputs, domain.call) # nolint

        res_comb <- list(
                clusters = do.call(rbind, lapply(res, "[[", "clusters")),
                ncluster = do.call(rbind, lapply(res, "[[", "ncluster"))
        )

        # stop cluster
        parallel::stopCluster(cl)

        # return result
        return(res)
}
