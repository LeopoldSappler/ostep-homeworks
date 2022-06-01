#Kapitel 29 - Lock-Based Concurrent Data Structures 
###Homework (Code) :

1. We’ll start by redoing the measurements within this chapter. Use
the call gettimeofday() to measure time within your program.
How accurate is this timer? What is the smallest interval it can measure? Gain confidence in its workings, as we will need it in all subsequent questions. You can also look into other timers, such as the
cycle counter available on x86 via the rdtsc instruction.

Der Timer ist relativ genau, die kleinste Zeiteinheit die gettimeofday()
messen kann ist eine Mikrosekunde. **Implementierung -> time_of_day.c**

2. Now, build a simple concurrent counter and measure how long it
   takes to increment the counter many times as the number of threads
   increases. How many CPUs are available on the system you are
   using? Does this number impact your measurements at all?

