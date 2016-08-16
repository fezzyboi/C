# Ric Rodriguez rirrodri
# $Id: Makefile,v 1.1 2016-03-09 22:49:03-08 - - $
MKFILE    = Makefile
NOINCLUDE = ci clean spotless

GCC       = gcc -g -O0 -Wall -Wextra -std=gnu11
GRIND     = valgrind --leak-check=full

CSOURCE   =  inssort.h linesort.c numsort.c inssort.c
SOURCES   = ${CSOURCE} ${MKFILE}
LISTSRC   = ${SOURCES}
all :
	gcc -g -O0 -Wall -Wextra -std=gnu99 linesort.c inssort.c -o linesort
	gcc -g -O0 -Wall -Wextra -std=gnu99 numsort.c inssort.c -o numsort

%.o : %.c
	cid + $<
	${GCC} -c $<

ci : ${SOURCES}
	cid + ${SOURCES}

lis : ${SOURCES} test
	mkpspdf ${LISTING} ${LISTSRC} ${OUTPUT}

clean :
	- rm mstat

spotless : clean
	- rm ${EXECBIN} ${LISTING} ${LISTING:.ps=.pdf}

test : ${EXECBIN}
	mk.tests

deps : ${CSOURCE} ${CHEADER}
	@ echo "# ${DEPSFILE} created `date`" >${DEPSFILE}
	${MKDEPS} ${CSOURCE} | sort | uniq >>${DEPSFILE}

${DEPSFILE} :
	@ touch ${DEPSFILE}
	${MAKE} --no-print-directory deps
submit :
	submit cmps012b-wm.w16 lab9 ${SOURCES}


again :
	gmake spotless deps ci all lis

