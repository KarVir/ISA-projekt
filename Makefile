######################################
#
# ISA Makefile
# @autor Karolína Virágová
#
######################################

TARGET                 = isa-top
ZIPNAME = xvirag00.zip
C                = g++
CPPFLAGS               = -std=c++20
EXTRA_CPPFLAGS         = -Wall -Wextra -pedantic -lpcap -lncurses
SRCS     = isa-top.c vector.c speed.c display.c sorting.c packet_handler.c 
${TARGET}: ${SRCS} 
	${C} ${CPPFLAGS} ${EXTRA_CPPFLAGS} -o ${TARGET} ${SRCS}
	
clean:
	rm isa-top

zip: 
	zip -v -r ${ZIPNAME} .* 