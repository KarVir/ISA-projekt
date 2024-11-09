######################################
#
# ISA Makefile
# @autor Karolína Virágová
#
######################################

TARGET                 = isa-top
TARNAME = xvirag00
C                = g++
CPPFLAGS               = -std=c++20
EXTRA_CPPFLAGS         = -Wall -Wextra -pedantic -lpcap -lncurses
SRCS     = isa-top.c vector.c speed.c display.c sorting.c packet_handler.c 
${TARGET}: ${SRCS} 
	${C} ${CPPFLAGS} ${EXTRA_CPPFLAGS} -o ${TARGET} ${SRCS}
	
clean:
	rm isa-top *.tar

tar: 
	tar -cvf ${TARNAME}.tar *.h *.c Makefile isa-top.1
