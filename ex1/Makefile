# Create your own targets that compile the application
runSensor:
	gcc sensor_node.c lib/tcpsock.c -o sensor -Wall -Werror -DLOOPS=5
	./sensor 101 60 127.0.0.1 1234

# the files for ex2 will be ziped and are then ready to
# be submitted to labtools.groept.be
zip:
	zip lab7_ex2.zip connmgr.c connmgr.h config.h lib/dplist.c lib/dplist.h lib/tcpsock.c lib/tcpsock.h
