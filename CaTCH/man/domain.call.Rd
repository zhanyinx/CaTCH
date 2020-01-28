\name{domain.call}
\alias{domain.call}
\alias{domain.call.parallel}
\title{
Call a hierarchy of domains based on Hi-C data
}
\description{
This package allows building the hierarchy of domains
starting from Hi-C data. Each hierarchical level is identified
by a minimum value of physical insulation between
neighboring domains.
}
\usage{
domain.call(input)
domain.call.parallel(inputs,ncpu=parallel::detectCores()-1L)
}
\arguments{
  \item{input}{
File containing Hi-C data for the SINGLE CHROMOSOME with 6 columns:
\itemize{
\item{col1 = chromosome}
\item{col2 = genomic coordinate of the start region}
\item{col3 = genomic coordinate of the end region}
\item{col4 = bin of the start region (genomic coordinate divided by binsize)}
\item{col5 = bin of the end region}
\item{col6 = Hi-C counts}
}
************OR*************

File containing Hi-C data for the SINGLE CHROMOSOME with 4 columns:
\itemize{
\item{col1 = chromosome}
\item{col2 = bin of the start region (genomic coordinate divided by binsize)}
\item{col3 = bin of the end region (genomic coordinate divided by binsize)}
\item{col4 = Hi-C counts}
}
}
	\item{inputs}{
		A character vector of files as \code{input}
	}
	\item{ncpu}{
		Number of cpu that you want to use (Default ncpu=4)
	}

}
\details{
}
\value{
Output of

\item{\code{domain.call}}{ A list with two elements
   \itemize{
	\item{ncluster: }{A data.frame with 3 columns

		-Chromosome (chromosome)
		
		-Reciprocal insulation (RI)
		
		-Number of domains (ndomains)
	}


  	\item{clusters: }{A data.frame with 5 columns
	
		-Chromosome (chromosome)
		
		-Reciprocal insulation (RI)
		
		-Start of domain (start)
	
		-End of domain (end)
		
		-Actual insulation (insulation)

  	}
   }	
}

\item{\code{domain.call.parallel}}{  A list of outputs as in \code{domain.call} for each file	}
}
\references{
 Zhan et al....
}
\author{
  Yinxiu Zhan
}
\note{
 
}


\seealso{
}
\examples{
#R code to be here
	fileinput=system.file("Test.dat.gz",package="CaTCH")
	library(CaTCH)
	domain.call(fileinput)
}
\keyword{TADs}
\keyword{hierarchy}
