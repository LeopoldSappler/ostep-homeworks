## Kapitel 29 - Lock-Based Concurrent Data Structures 
#### Gruppe 6: Niels Hensler, Leopold Sappler

1. We’ll start by redoing the measurements within this chapter. Use
the call gettimeofday() to measure time within your program.
How accurate is this timer? What is the smallest interval it can measure? Gain confidence in its workings, as we will need it in all subsequent questions. You can also look into other timers, such as the
cycle counter available on x86 via the rdtsc instruction.

_Implementierung -> time_of_day.c_  
Der Timer ist relativ genau (jedoch nicht so genau wie z.B clock_gettime!),die kleinste Zeiteinheit die gettimeofday()
messen kann ist eine Mikrosekunde.

2. Now, build a simple concurrent counter and measure how long it
   takes to increment the counter many times as the number of threads
   increases. How many CPUs are available on the system you are
   using? Does this number impact your measurements at all?

_Implementierung -> concurrent_counter.c_ (Quelle: OSTEP Chapter 29)  
Mit höherer Threadanzahl steigt auch die gemessene Zeit beim Concurrent Counter, wie in Figure 1 zu sehen ist. Hier wurden 1-4 Threads getestet, die jeweils bis 10.000.000 hochzählen.
Das System auf dem getestet wurde hat 6 CPUs. 

![Concurrent Counter](image-sources/concurrent_counter_diagramm.png){width=400}

3. Next, build a version of the sloppy counter. Once again, measure its
   performance as the number of threads varies, as well as the threshold. Do the numbers match what you see in the chapter?

_Implementierung -> sloppy_counter.c_ (Quelle: OSTEP Chapter 29)  
Mit höherer Threadanzahl sollte eigentlich beim Sloppy Counter die Zeit sehr gering sein, jedoch funktioniert das bei unserem Code nicht, wie man in Figure 2 sehen kann. Grund ???

![Concurrent Counter vs Sloppy Counter](image-sources/concurrent_vs_sloppy_diagramm.png){width=400}

4. Build a version of a linked list that uses hand-over-hand locking
    [MS04], as cited in the chapter. You should read the paper first
    to understand how it works, and then implement it. Measure its
    performance. When does a hand-over-hand list work better than a
    standard list as shown in the chapter?

_Implementierung -> sloppy_counter.c_ (Quelle: OSTEP Chapter 29, https://www.geeksforgeeks.org/linked-list-set-1-introduction)  
In Figure 3 sieht man, wie lange 1-8 Threads brauchen, um jeweils 1.000.000 Elemente in die Concurrent und die Hand-Over-Hand Liste einzufügen. 

![Lists: Insert Time](image-sources/list_inserttime_diagramm.png){width=400}


![Lists: Lookup Time](image-sources/list_lookuptime_diagramm.png){width=400}

5. Pick your favorite data structure, such as a B-tree or other slightly
   more interesting structure. Implement it, and start with a simple
   locking strategy such as a single lock. Measure its performance as
   the number of concurrent threads increases.

6. Finally, think of a more interesting locking strategy for this favorite
   data structure of yours. Implement it, and measure its performance.
   How does it compare to the straightforward locking approach?