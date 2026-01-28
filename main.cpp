/*
* PayNix - Billing Software
 * Copyright (c) 2026 Roshan Kumar Singh
 * GitHub: https://github.com/roshhellwett/PayNix
 * Licensed under MIT License
 */

#include <cstdio>
#include "menu.h"
#include "employee.h"
#include "admin.h"
#include "ui.h"
#include <sys/stat.h>

#ifdef _WIN32
#include <direct.h>
#endif

static void ensureDataDir() {
#ifdef _WIN32
    _mkdir("data");
#else
    mkdir("data", 0777);
#endif
}

int main() {

    ensureDataDir();

    int choice;

    while (1) {
        clearScreen();
        printBoxTitle("PAYNIX - BILLING SOFTWARE");

        printf("1. EMPLOYEE LOGIN\n");
        printf("2. ADMIN PANEL\n");
        printf("3. EXIT\n");

        printf("\nENTER CHOICE : ");
        scanf("%d", &choice);
        getchar();

        /* ===== EMPLOYEE LOGIN ===== */
        if (choice == 1) {

            clearScreen();
            if (!employeeLogin()) {
                printf("\nPRESS ENTER TO CONTINUE...");
                getchar();
                continue;
            }

            /* ===== EMPLOYEE SESSION ===== */
            while (1) {
                menu();

                scanf("%d", &choice);
                getchar();

                if (choice == 9) {   // LOGOUT
                    employeeLogout();
                    break;
                }

                handleMenuChoice(choice);
            }
        }

        /* ===== ADMIN PANEL ===== */
        else if (choice == 2) {
            adminPanel();
        }

        /* ===== EXIT ===== */
        else if (choice == 3) {
            clearScreen();
            printBoxTitle("GOODBYE");
            return 0;
        }

        else {
            setColor(RED);
            printf("\nINVALID CHOICE\n");
            resetColor();
            printf("PRESS ENTER...");
            getchar();
        }
    }
}
