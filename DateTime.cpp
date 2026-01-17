/*
* PayNix - Billing Software
 * Copyright (c) 2026 Roshan Kumar Singh
 * GitHub: https://github.com/roshhellwett/PayNix
 * Licensed under MIT License
 */
#include <cstdio>
#include <ctime>
#include "DateTime.h"

void getDateTime(char *buffer) {
    time_t now;
    struct tm *t;
    time(&now);
    t = localtime(&now);

    strftime(buffer, 30, "%d-%m-%Y %H:%M:%S", t);
}
