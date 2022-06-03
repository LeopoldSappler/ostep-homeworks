FLAGS = -Wall -pthread -g

all: time_of_day concurrent_counter sloppy_counter concurrent_linked_list handoverhand_linked_list concurrent_queue michael_and_scott_queue

clean:
	rm -f time_of_day concurrent_counter sloppy_counter concurrent_linked_list handoverhand_linked_list concurrent_queue michael_and_scott_queue

time_of_day: timeOfDay.c
	gcc -o time_of_day timeOfDay.c $(FLAGS)

concurrent_counter: concurrent_counter.c
	gcc -o concurrent_counter concurrent_counter.c $(FLAGS)

sloppy_counter: sloppy_counter.c
	gcc -o sloppy_counter sloppy_counter.c $(FLAGS)

concurrent_linked_list: concurrent_linked_list.c
	gcc -o concurrent_linked_list concurrent_linked_list.c $(FLAGS)

handoverhand_linked_list: handoverhand_linked_list.c
	gcc -o handoverhand_linked_list handoverhand_linked_list.c $(FLAGS)

concurrent_queue: concurrent_queue.c
	gcc -o concurrent_queue concurrent_queue.c $(FLAGS)

michael_and_scott_queue: michael_and_scott_queue.c
	gcc -o michael_and_scott_queue michael_and_scott_queue.c $(FLAGS)