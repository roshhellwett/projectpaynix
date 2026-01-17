/*
* PayNix - Billing Software
 * Copyright (c) 2026 Roshan Kumar Singh
 * GitHub: https://github.com/roshhellwett/PayNix
 * Licensed under MIT License
 */
#include <cstdio>
#include <cstdlib>
#include "ui.h"

#define RED     31
#define GREEN   32
#define YELLOW  33
#define BLUE    34
#define CYAN    36
#define WHITE   37

void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void setColor(int color) {
    printf("\033[1;%dm", color);
}

void resetColor() {
    printf("\033[0m");
}

void printBoxTitle(const char *title) {
    setColor(CYAN);
    printf("+--------------------------------------+\n");
    printf("| %-36s |\n", title);
    printf("+--------------------------------------+\n");
    resetColor();
}
