######################################
#
# IPK Makefile
# @autor KV
#
######################################

TARGET                 = isa-top
ZIPNAME = xvirag00.zip
C                = g++
CPPFLAGS               = -std=c++20
EXTRA_CPPFLAGS         = -Wall -Wextra -pedantic -lpcap
SRCS     = main.c
${TARGET}: ${SRCS} 
	${C} ${CPPFLAGS} ${EXTRA_CPPFLAGS} -o ${TARGET} ${SRCS}
	
clean:
	rm ipk-sniffer

zip: 
	zip -v -r ${ZIPNAME} .* 