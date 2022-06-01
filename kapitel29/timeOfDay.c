#include <bits/types/struct_timeval.h>
#include <stdio.h>
#include <sys/time.h>

int main() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    printf("Aktuelle Zeit: Sekunden: %ld, Mikrosekunden: %ld\n", tv.tv_sec, tv.tv_usec);
}
