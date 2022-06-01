# Kapitel 29
# Lock-Based Concurrent Data Structures 
### Gruppe 6: Niels Hensler, Leopold Sappler

1. We’ll start by redoing the measurements within this chapter. Use
the call gettimeofday() to measure time within your program.
How accurate is this timer? What is the smallest interval it can measure? Gain confidence in its workings, as we will need it in all subsequent questions. You can also look into other timers, such as the
cycle counter available on x86 via the rdtsc instruction.

Der Timer ist relativ genau (jedoch nicht so genau wie z.B clock_gettime!), die kleinste Zeiteinheit die gettimeofday()
messen kann ist eine Mikrosekunde. _Implementierung -> time_of_day.c_

2. Now, build a simple concurrent counter and measure how long it
   takes to increment the counter many times as the number of threads
   increases. How many CPUs are available on the system you are
   using? Does this number impact your measurements at all?

_Implementierung -> concurrent_counter.c_

![Concurrent Counter](image-sources/concurrent_counter_diagramm.png){width=350}

3. Next, build a version of the sloppy counter. Once again, measure its
   performance as the number of threads varies, as well as the threshold. Do the numbers match what you see in the chapter?

