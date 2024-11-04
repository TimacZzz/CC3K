
CXX = g++					# compiler
CXXFLAGS = -std=c++20 -g -Wall -Werror=vla -MMD			# compiler flags
MAKEFILE_NAME = ${firstword ${MAKEFILE_LIST}}	# makefile name

SOURCES = $(wildcard *.cc floor/*.cc character/*.cc character/player/*.cc character/enemy/*.cc item/*.cc)			# source files (*.cc)
OBJECTS = ${SOURCES:.cc=.o} 
DEPENDS = ${OBJECTS:.o=.d}			# substitute ".o" with ".d"
EXEC = cc3k					# executable name


${EXEC} : ${OBJECTS}				
	${CXX} ${CXXFLAGS} ${OBJECTS} -o ${EXEC} 

-include ${DEPENDS}	

.PHONY : clean	

clean :						
	rm -f ${DEPENDS} ${OBJECTS} ${EXEC}
