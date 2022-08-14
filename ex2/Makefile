# Create your own targets that compile the application
compileSensor:
	gcc lib/dplist.c -o libdplist.so -lm -fPIC -shared
	gcc lib/tcpsock.c -o libtcpsock.so -lm -fPIC -shared
	gcc sensor_node.c -o sensor_node -Wall -std=c11 -Werror -DLOOPS=5 -lm -L. -Wl,-rpath=. -ltcpsock

simulateSensors:
	./sensor_node 101 2 127.0.0.1 1234
#	./sensor_node 102 3 127.0.0.1 1234
#	./sensor_node 103 5 127.0.0.1 1234
#	sleep 30
#	pkill -f -e -c sensor_node

compileManager:
	gcc main.c connmgr.c -o conn_mgr_main -Wall -std=c11 -Werror -lm -L. -Wl,-rpath=. -ltcpsock -ldplist -DTIMEOUT=5

runManager:
	./conn_mgr_main 1234


# the files for ex2 will be ziped and are then ready to
# be submitted to labtools.groept.be
zip:
	zip lab7_ex2.zip connmgr.c connmgr.h config.h lib/dplist.c lib/dplist.h lib/tcpsock.c lib/tcpsock.h


run : conn_mgr_main sensor_node
			gnome-terminal -- valgrind --tool=memcheck --leak-check=yes --show-reachable=yes --num-callers=20 --track-fds=yes ./conn_mgr_main 1234
	sleep 2
			-- ./sensor_node 15 2 127.0.0.1 1234 &		#start the sensors
			-- ./sensor_node 16 3 127.0.0.1 1234 &
			-- ./sensor_node 20 7 127.0.0.1 1234 &
	@echo "Started the sensors"
	sleep 30
	pkill -f -e -c sensor_node													#Kill all the sensors