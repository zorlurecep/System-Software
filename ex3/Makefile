# Create your own targets that build the dynamic library for the list and then compile the application
generateFile:
	gcc file_creator.c -DSET_MAX_TEMP=30 -DSET_MIN_TEMP=15 -DDEBUG
	./a.out

debugStaticLib:
	gcc -g  main.c datamgr.c -L./lib -ldplist -DSET_MAX_TEMP=30 -DSET_MIN_TEMP=15 -DDEBUG
	./a.out

# First run debugger from CLion
debugDDD:
	ddd cmake-build-debug/lab5

valgrindCheck:
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose --log-file=valgrind-out.txt ./cmake-build-debug/lab5
# the files for ex3 will be ziped and are then ready to
# be submitted to labtools.groept.be
zip:
	zip lab5_ex3.zip datamgr.c datamgr.h config.h lib/dplist.c lib/dplist.h
