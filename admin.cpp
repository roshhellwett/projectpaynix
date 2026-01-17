/*
* PayNix - Billing Software
 * Copyright (c) 2026 Roshan Kumar Singh
 * GitHub: https://github.com/roshhellwett/PayNix
 * Licensed under MIT License
 */
#include <cstdio>
#include "employee.h"
#include <cstring>
#include "admin.h"
#include "ui.h"

static void setupAdmin() {

    struct Admin a;
    FILE *fp = fopen("admin.dat", "wb");

    clearScreen();
    printBoxTitle("FIRST TIME ADMIN SETUP");

    printf("Example Admin ID  : 123456\n");
    printf("Example Password  : admin@123\n\n");

    printf("SET ADMIN ID : ");
    fgets(a.admin_id, sizeof(a.admin_id), stdin);
    a.admin_id[strcspn(a.admin_id, "\n")] = '\0';

    printf("SET PASSWORD : ");
    fgets(a.password, sizeof(a.password), stdin);
    a.password[strcspn(a.password, "\n")] = '\0';

    fwrite(&a, sizeof(a), 1, fp);
    fclose(fp);

    setColor(GREEN);
    printf("\nADMIN SETUP COMPLETED\n");
    resetColor();

    printf("PRESS ENTER...");
    getchar();
}

static int verifyAdmin() {

    struct Admin saved, input;
    FILE *fp = fopen("admin.dat", "rb");

    if (!fp) {
        setupAdmin();
        return 1;
    }

    fread(&saved, sizeof(saved), 1, fp);
    fclose(fp);

    clearScreen();
    printBoxTitle("ADMIN LOGIN");

    printf("ENTER ADMIN ID : ");
    fgets(input.admin_id, sizeof(input.admin_id), stdin);
    input.admin_id[strcspn(input.admin_id, "\n")] = '\0';

    printf("ENTER PASSWORD : ");
    fgets(input.password, sizeof(input.password), stdin);
    input.password[strcspn(input.password, "\n")] = '\0';

    if (strcmp(saved.admin_id, input.admin_id) == 0 &&
        strcmp(saved.password, input.password) == 0)
        return 1;

    setColor(RED);
    printf("\nINVALID ADMIN CREDENTIALS\n");
    resetColor();

    printf("PRESS ENTER...");
    getchar();
    return 0;
}

void adminPanel() {

    if (!verifyAdmin())
        return;

    char ch;

    while (1) {
        clearScreen();
        printBoxTitle("ADMIN PANEL");
        printf("1. ADD EMPLOYEE\n");
        printf("2. VIEW EMPLOYEES\n");
        printf("3. VIEW EMPLOYEE ACTIVITY\n");
        printf("4. EXIT ADMIN PANEL\n");
        printf("\nENTER CHOICE : ");
        ch = getchar();
        while (getchar() != '\n');

        if (ch == '1') {
            addEmployee();
        }
        else if (ch == '2') {
            viewEmployees();
        }
        else if (ch == '3') {
            showEmployeeActivity();
        }
        else if (ch == '4') {
            return;
        }

    }
}
void showEmployeeActivity() {

    FILE *fp = fopen("employee_log.dat", "r");
    if (!fp) {
        printf("\nNO EMPLOYEE ACTIVITY FOUND\n");
        printf("\nPRESS ENTER...");
        getchar();
        return;
    }

    clearScreen();
    printBoxTitle("EMPLOYEE LOGIN ACTIVITY");

    printf("%-6s | %-12s | %-25s | %s\n",
           "TYPE", "EMP ID", "EMPLOYEE NAME", "DATE & TIME");
    printf("-----------------------------------------------------------------------\n");

    char action[10], empid[20], name[30], datetime[40];

    while (fscanf(fp,
           "%9[^|]|%19[^|]|%29[^|]|%39[^\n]\n",
           action, empid, name, datetime) == 4) {

        printf("%-6s | %-12s | %-25s | %s\n",
               action, empid, name, datetime);
           }

    fclose(fp);

    printf("\nPRESS ENTER...");
    getchar();
}

