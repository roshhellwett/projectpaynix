/*
* PayNix - Billing Software
 * Copyright (c) 2026 Roshan Kumar Singh
 * GitHub: https://github.com/roshhellwett/PayNix
 * Licensed under MIT License
 */
#include <cstdio>
#include "ui.h"
#include "credits.h"

void showCreatorInfo() {

    setColor(CYAN);

    printf("  ____  _ _ _ _               ____        __ _      \n");
    printf(" | __ )(_) | (_)_ __   __ _  / ___|  ___ / _| |_ ___ \n");
    printf(" |  _ \\| | | | | '_ \\ / _` | \\___ \\ / _ \\ |_| __/ __|\n");
    printf(" | |_) | | | | | | | | (_| |  ___) |  __/  _| |_\\__ \\\n");
    printf(" |____/|_|_|_|_|_| |_|\\__, | |____/ \\___|_|  \\__|___/\n");
    printf("                        |___/                         \n");

    resetColor();

    printBoxTitle("BILLING SOFTWARE");

    printf(" Developed by : Roshan Kr Singh\n");
    printf(" Version      : 1.0\n");
    printf(" Year         : 2026\n");
    printf(" Platform     : Terminal Based (C/C++)\n");

    printf("\n");
}
