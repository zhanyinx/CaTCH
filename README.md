# CaTCH

## Steps
1) Download CaTCH_1.0.tar.gz 
2) Check that you have all libraries:
3) R CMD check CaTCH_1.0.tar.gz

#########To install:
R CMD INSTALL CaTCH_1.0.tar.gz 

#########To load:
library(CaTCH_R)

#########Example
fileinput=system.file("Test.dat.gz",package="CaTCH")

library(CaTCH)

domain.call(fileinput)


