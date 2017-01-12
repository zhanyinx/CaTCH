# CaTCH

########Download CaTCH_1.0.tar.gz 

########Check that you have all libraries:
R CMD check CaTCH_1.0.tar.gz
#########should give just a note about empty directories

#########To install:
R CMD INSTALL CaTCH_1.0.tar.gz 

#########To load:
library(CaTCH_R)

#########Example
fileinput=system.file("Test.dat.gz",package="CaTCH")

library(CaTCH)

domain.call(fileinput)
