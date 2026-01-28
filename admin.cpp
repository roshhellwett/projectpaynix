/*
* PayNix - Billing Software
 * Copyright (c) 2026 Roshan Kumar Singh
 * GitHub: https://github.com/roshhellwett/PayNix
 * Licensed under MIT License
 */
#include <cstdio>
#include <cstring>
#include "admin.h"
#include "employee.h"
#include "ui.h"
#include "dataPath.h"

/* ---------- ADMIN SETUP ---------- */
static void setupAdmin() {
    Admin a;
    FILE *fp = fopen(ADMIN_FILE, "wb");

    clearScreen();
    printBoxTitle("FIRST TIME ADMIN SETUP");

    printf("SET ADMIN ID : ");
    fgets(a.admin_id, sizeof(a.admin_id), stdin);
    a.admin_id[strcspn(a.admin_id, "\n")] = 0;

    printf("SET PASSWORD : ");
    fgets(a.password, sizeof(a.password), stdin);
    a.password[strcspn(a.password, "\n")] = 0;

    fwrite(&a, sizeof(a), 1, fp);
    fclose(fp);

    setColor(GREEN);
    printf("\nADMIN CREATED SUCCESSFULLY\n");
    resetColor();

    printf("PRESS ENTER...");
    getchar();
}

static int verifyAdmin() {
    Admin saved, input;
    FILE *fp = fopen(ADMIN_FILE, "rb");

    if (!fp) {
        setupAdmin();
        return 1;
    }

    fread(&saved, sizeof(saved), 1, fp);
    fclose(fp);

    clearScreen();
    printBoxTitle("ADMIN LOGIN");

    printf("ADMIN ID : ");
    fgets(input.admin_id, sizeof(input.admin_id), stdin);
    input.admin_id[strcspn(input.admin_id, "\n")] = 0;

    printf("PASSWORD : ");
    fgets(input.password, sizeof(input.password), stdin);
    input.password[strcspn(input.password, "\n")] = 0;

    if (strcmp(saved.admin_id, input.admin_id) == 0 &&
        strcmp(saved.password, input.password) == 0)
        return 1;

    setColor(RED);
    printf("\nINVALID ADMIN LOGIN\n");
    resetColor();

    printf("PRESS ENTER...");
    getchar();
    return 0;
}

/* ---------- ADD EMPLOYEE ---------- */
static void addEmployee() {
    Employee e;
    FILE *fp = fopen(EMP_FILE, "ab");

    clearScreen();
    printBoxTitle("ADD EMPLOYEE");

    printf("EMPLOYEE ID : ");
    fgets(e.emp_id, sizeof(e.emp_id), stdin);
    e.emp_id[strcspn(e.emp_id, "\n")] = 0;

    printf("NAME : ");
    fgets(e.name, sizeof(e.name), stdin);
    e.name[strcspn(e.name, "\n")] = 0;

    printf("DOB (DD-MM-YYYY) : ");
    fgets(e.dob, sizeof(e.dob), stdin);
    e.dob[strcspn(e.dob, "\n")] = 0;

    fwrite(&e, sizeof(e), 1, fp);
    fclose(fp);

    setColor(GREEN);
    printf("\nEMPLOYEE ADDED SUCCESSFULLY\n");
    resetColor();

    printf("PRESS ENTER...");
    getchar();
}

/* ---------- VIEW EMPLOYEES ---------- */

static void viewEmployees() {
    Employee e;
    FILE *fp = fopen(EMP_FILE, "rb");

    clearScreen();
    printBoxTitle("EMPLOYEE LIST");

    if (!fp) {
        printf("NO EMPLOYEES FOUND\n");
        printf("PRESS ENTER...");
        getchar();
        return;
    }

    printf("%-15s %-25s %-15s\n",
           "EMP ID", "NAME", "DOB");
    printf("-----------------------------------------------------\n");

    while (fread(&e, sizeof(e), 1, fp)) {
        printf("%-15s %-25s %-15s\n",
               e.emp_id, e.name, e.dob);
    }

    fclose(fp);
    printf("\nPRESS ENTER...");
    getchar();
}

/* ---------- ADMIN PANEL ---------- */

void adminPanel() {

    if (!verifyAdmin())
        return;

    int choice;
    do {
        clearScreen();
        printBoxTitle("ADMIN PANEL");

        printf("1. ADD EMPLOYEE\n");
        printf("2. VIEW EMPLOYEES\n");
        printf("3. VIEW EMPLOYEE ACTIVITY\n");
        printf("4. LOGOUT\n");
        printf("\nENTER CHOICE : ");
        scanf("%d", &choice);
        getchar();

        switch (choice) {
            case 1:
                addEmployee();
                break;
            case 2:
                viewEmployees();
                break;
            case 3:
                showEmployeeActivity();
                break;
        }
    } while (choice != 4);
}

/* ---------- EMPLOYEE ACTIVITY ---------- */

void showEmployeeActivity() {

    FILE *fp = fopen(EMPLOYEE_LOG_FILE, "r");
    if (!fp) {
        printf("\nNO EMPLOYEE ACTIVITY FOUND\n");
        printf("PRESS ENTER...");
        getchar();
        return;
    }

    clearScreen();
    printBoxTitle("EMPLOYEE ACTIVITY");

    printf("%-8s %-12s %-20s %s\n",
           "ACTION", "EMP ID", "NAME", "DATE & TIME");
    printf("-------------------------------------------------------------\n");

    char action[10], empid[20], name[30], datetime[40];

    while (fscanf(fp,
           "%9[^|]|%19[^|]|%29[^|]|%39[^\n]\n",
           action, empid, name, datetime) == 4) {

        printf("%-8s %-12s %-20s %s\n",
               action, empid, name, datetime);
    }

    fclose(fp);
    printf("\nPRESS ENTER...");
    getchar();
}
