# CaTCH_R

########Check that you have all libraries:
R CMD check DOTH_1.0.tar.gz
#########should give just a note about empty directories

#########To install:
R CMD install DOTH_1.0.tar.gz 

#########To load:
library(DOTH)

#########Example
fileinput=system.file("Test.dat.gz",package="CaTCH")

library(CaTCH)

domain.call(fileinput)
