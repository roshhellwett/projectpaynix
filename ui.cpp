/*
* PayNix - Billing Software
 * Copyright (c) 2026 Roshan Kumar Singh
 * GitHub: https://github.com/roshhellwett/PayNix
 * Licensed under MIT License
 */

#include <cstdio>
#include <cstdlib>

#ifdef _WIN32
#include <conio.h>
#else
#include <termios.h>
#include <unistd.h>
#endif

#include "ui.h"

/* ---------- COLORS ---------- */

void setColor(int c) {
    printf("\033[%dm", c);
    fflush(stdout);
}

void resetColor() {
    printf("\033[0m");
    fflush(stdout);
}

/* ---------- SCREEN ---------- */

void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

/* ---------- BOX TITLE ---------- */

void printBoxTitle(const char *t) {
    setColor(CYAN);
    printf("+--------------------------------------+\n");
    printf("| %-36s |\n", t);
    printf("+--------------------------------------+\n");
    resetColor();
}

/* ---------- HIDDEN INPUT ---------- */

void inputHidden(char *buf, int size) {

    int i = 0;
    char ch = 0;

#ifdef _WIN32

    while ((ch = getch()) != '\r' && i < size - 1) {

        if ((ch == 8 || ch == 127) && i > 0) {   // backspace
            i--;
            printf("\b \b");
            fflush(stdout);
        }
        else if (ch >= 32) {
            buf[i++] = ch;
            printf("*");
            fflush(stdout);
        }
    }

#else
    struct termios oldt{}, newt{};
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    while ((ch = getchar()) != '\n' && i < size - 1) {

        if ((ch == 8 || ch == 127) && i > 0) {   // backspace
            i--;
            printf("\b \b");
            fflush(stdout);
        }
        else if (ch >= 32) {
            buf[i++] = ch;
            printf("*");
            fflush(stdout);
        }
    }

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
#endif

    buf[i] = '\0';
    printf("\n");
}
