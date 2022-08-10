# Create your own targets that compile the application
run:
	gcc -Wall -std=c11 -Werror main.c sensor_db.c -l sqlite3
	./a.out

# the files for ex2 will be ziped and are then ready to
# be submitted to labtools.groept.be
zip:
	zip lab6_ex2.zip sensor_db.c config.h
