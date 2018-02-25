halfnhalf: halfnhalf.o
	cc -o halfnhalf halfnhalf.o

halfnhalf.o: halfnhalf.c
	cc -O2 -c halfnhalf.c

clean:
	rm halfnhalf halfnhalf.o

