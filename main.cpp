#include <cstdio>
#include "menu.h"
#include "employee.h"
#include "admin.h"
#include "ui.h"

int main() {

    char ch[10];

    while (1) {

        clearScreen();
        printBoxTitle("BILLING SOFTWARE");

        printf("1. EMPLOYEE LOGIN\n");
        printf("2. ADMIN PANEL\n");
        printf("3. EXIT\n");

        printf("\nENTER CHOICE : ");
        fgets(ch, sizeof(ch), stdin);

        if (ch[0] == '1') {

            clearScreen();
            if (!employeeLogin()) {
                printf("\nPRESS ENTER...");
                getchar();
                continue;
            }

            while (1) {
                clearScreen();
                menu();
                printf("ENTER CHOICE : ");
                fgets(ch, sizeof(ch), stdin);

                if (ch[0] == 'I' || ch[0] == 'i') {
                    employeeLogout();
                    break;
                }
            }
        }

        else if (ch[0] == '2') {
            adminPanel();
        }

        else if (ch[0] == '3') {
            clearScreen();
            return 0;
        }
    }
}
