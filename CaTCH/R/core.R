domain.call <- function (input){
	hicData=read.table(input,nrows=10)
	ncolumns=ncol(hicData)
	
	#check that each file contains a single chromosome
	stopifnot(length(unique(hicData[,1]))==1) 

	#check type of file
	if(ncolumns==6){
	  original=read.table(input,colClasses=c("character","numeric","numeric","numeric","numeric","numeric"))
	  hicData=original[,c(-2,-3)]
	}else if(ncolumns==4){
	  original=read.table(input,colClasses=c("character","numeric","numeric","numeric"))
	  hicData=original
	}else{
	  stop("input file has the wrong format. It needs 4 or 6 columns")	
	}
	
	#calling C function
	res <- .Call("Catch", as.matrix(hicData[,-1]), PACKAGE="CaTCH")
	colnames(res$ncluster) <- c("RI","ndomains")
	colnames(res$clusters) <- c("RI","start","end","insulation")
	res$ncluster <- data.frame(chromosome=unique(hicData[,1]),res$ncluster)
	res$clusters <- data.frame(chromosome=unique(hicData[,1]),res$clusters)
	return(res)
}

domain.call.parallel <- function(inputs, ncpu=4) {
	# check input parameters
	stopifnot(is.numeric(ncpu) || ncpu < 1L)
	stopifnot(is.character(inputs))
	stopifnot(all(file.exists(inputs)))

	# start parallel R processes
	cl <- parallel::makeCluster(min(ncpu, length(inputs)))
	message("loading CaTCH on ",length(cl)," nodes...", appendLF=FALSE)
	res <- parallel::clusterEvalQ(cl, library(CaTCH))
	if(!all(sapply(res, function(x) "CaTCH" %in% x)))
		stop("'CaTCH' could not be loaded on all cluster nodes")
	message("done")

	# segment chromosomes
	res <- parLapply(cl, inputs, domain.call)
	#res <- parLapply(cl, inputs, function(x) {
	#	message("now segmenting ", x)
	#	data.frame(chr=basename(x), start=1:3*10, end=2:4*10 -1)
	#})

	# post-process results list
	#res <- do.call(rbind, res)
	resComb <- list(clusters=do.call(rbind, lapply(res, "[[", "clusters")),
		        ncluster=do.call(rbind, lapply(res, "[[", "ncluster")))

	# stop cluster
	parallel::stopCluster(cl)

	# return result
	return(res)
}

