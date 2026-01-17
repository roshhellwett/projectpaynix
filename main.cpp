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

int main() {

    char choice;

    while (1) {

        clearScreen();
        printBoxTitle("BILLING SOFTWARE");

        printf("1. EMPLOYEE LOGIN\n");
        printf("2. ADMIN PANEL\n");
        printf("3. EXIT\n");

        printf("\nENTER CHOICE : ");
        choice = getchar();
        while (getchar() != '\n');

        if (choice == '1') {

            clearScreen();
            if (!employeeLogin()) {
                printf("\nPRESS ENTER TO CONTINUE...");
                getchar();
                continue;
            }

            while (1) {
                clearScreen();
                menu();

                printf("ENTER CHOICE : ");
                choice = getchar();
                while (getchar() != '\n');

                if (choice == '9') {
                    employeeLogout();
                    break;
                }

                handleMenuChoice(choice);
            }
        }

        else if (choice == '2') {
            adminPanel();
        }

        else if (choice == '3') {
            clearScreen();
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
