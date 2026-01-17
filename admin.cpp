#include <cstdio>
#include "admin.h"
#include "employee.h"
#include "ui.h"

void showEmployeeActivity() {

    FILE *fp = fopen("employee_log.dat", "rb");
    if (!fp) return;

    char line[200];

    printBoxTitle("EMPLOYEE ACTIVITY");

    printf("%-8s %-12s %-20s %-20s\n",
           "ACTION", "EMP ID", "NAME", "DATE & TIME");
    printf("--------------------------------------------------------------\n");

    while (fgets(line, sizeof(line), fp)) {

        char act[10], id[20], name[50], dt[40];

        if (sscanf(line, "%[^|]| %[^|]| %[^|]| %[^\n]",
                   act, id, name, dt) == 4) {

            printf("%-8s %-12s %-20s %-20s\n",
                   act, id, name, dt);
                   }
    }

    fclose(fp);
}

void adminPanel() {

    char ch[10];

    while (1) {

        clearScreen();
        printBoxTitle("ADMIN PANEL");

        printf("1. ADD EMPLOYEE\n");
        printf("2. VIEW EMPLOYEES\n");
        printf("3. EMPLOYEE ACTIVITY\n");
        printf("4. EXIT\n");

        printf("\nENTER CHOICE : ");
        fgets(ch, sizeof(ch), stdin);

        clearScreen();

        if (ch[0] == '1') addEmployee();
        else if (ch[0] == '2') viewEmployees();
        else if (ch[0] == '3') showEmployeeActivity();
        else if (ch[0] == '4') return;

        printf("\nPRESS ENTER...");
        getchar();
    }
}
