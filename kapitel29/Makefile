FLAGS = -Wall -pthread -g

all: time_of_day concurrent_counter sloppy_counter

clean:
	rm -f time_of_day concurrent_counter

time_of_day:
	gcc -o time_of_day timeOfDay.c $(FLAGS)

concurrent_counter:
	gcc -o concurrent_counter concurrent_counter.c $(FLAGS)

slopppy_counter:
	gcc -o sloppy_counter sloppy_counter.c $(FLAGS)