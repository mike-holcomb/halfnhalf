halfnhalf: halfnhalf.o splaysort.o
	cc -o halfnhalf halfnhalf.o splaysort.o

testSplay: testSplay.o splaysort.o
	cc -o testSplay testSplay.o splaysort.o

halfnhalf.o: halfnhalf.c
	cc -g -c halfnhalf.c

splaysort.o: splaysort.c
	cc -g -c splaysort.c

testsplay.o: testSplay.c
	cc -g -c testSplay.c

clean:
	rm halfnhalf testSplay *.o 

