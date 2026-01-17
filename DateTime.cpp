#include <cstdio>
#include <ctime>
#include "dateTime.h"

void getDateTime(char *buffer) {
    time_t now;
    struct tm *t;
    time(&now);
    t = localtime(&now);

    strftime(buffer, 30, "%d-%m-%Y %H:%M:%S", t);
}
