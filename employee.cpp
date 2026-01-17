/*
* PayNix - Billing Software
 * Copyright (c) 2026 Roshan Kumar Singh
 * GitHub: https://github.com/roshhellwett/PayNix
 * Licensed under MIT License
 */
#include <cstdio>
#include <cstring>
#include <ctime>
#include "employee.h"
#include "ui.h"

static char currentEmployeeId[20] = "";
static char currentEmployeeName[50] = "";

void getCurrentTime(char *buf) {
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    strftime(buf, 30, "%d-%m-%Y %H:%M:%S", t);
}

void addEmployee() {

    FILE *fp = fopen("employees.dat", "ab");
    if (!fp) return;

    struct Employee e;

    printBoxTitle("ADD EMPLOYEE");

    printf("NAME          : ");
    fgets(e.name, sizeof(e.name), stdin);
    e.name[strcspn(e.name, "\n")] = '\0';

    printf("PHONE NUMBER  : ");
    fgets(e.phone, sizeof(e.phone), stdin);
    e.phone[strcspn(e.phone, "\n")] = '\0';

    printf("DATE OF BIRTH (DD-MM-YYYY) : ");
    fgets(e.dob, sizeof(e.dob), stdin);
    e.dob[strcspn(e.dob, "\n")] = '\0';

    printf("SALARY        : ");
    scanf("%f", &e.salary);
    getchar();

    char last4[5], ddmm[5];
    strncpy(last4, &e.phone[strlen(e.phone) - 4], 4);
    last4[4] = '\0';

    ddmm[0] = e.dob[0];
    ddmm[1] = e.dob[1];
    ddmm[2] = e.dob[3];
    ddmm[3] = e.dob[4];
    ddmm[4] = '\0';

    sprintf(e.emp_id, "EMP%s%s", ddmm, last4);

    getCurrentTime(e.join_date);

    fwrite(&e, sizeof(e), 1, fp);
    fclose(fp);

    setColor(GREEN);
    printf("\nEMPLOYEE ADDED\nEMPLOYEE ID : %s\n", e.emp_id);
    resetColor();
}

void viewEmployees() {

    FILE *fp = fopen("employees.dat", "rb");
    if (!fp) {
        printf("\nNO EMPLOYEES FOUND\n");
        printf("\nPRESS ENTER TO CONTINUE...");
        getchar();
        return;
    }

    struct Employee e;

    printBoxTitle("EMPLOYEE LIST");

    printf("%-12s %-20s %-12s %-12s %-10s\n",
           "EMP ID", "NAME", "PHONE", "DOB", "SALARY");
    printf("------------------------------------------------------------\n");

    while (fread(&e, sizeof(e), 1, fp)) {
        printf("%-12s %-20s %-12s %-12s %.2f\n",
               e.emp_id, e.name, e.phone, e.dob, e.salary);
    }

    fclose(fp);

    printf("\nPRESS ENTER TO CONTINUE...");
    getchar();
}


int employeeLogin() {

    FILE *fp = fopen("employees.dat", "rb");
    if (!fp) return 0;

    char id[20], pass[20];
    struct Employee e;

    printBoxTitle("EMPLOYEE LOGIN");

    printf("EMPLOYEE ID : ");
    fgets(id, sizeof(id), stdin);
    id[strcspn(id, "\n")] = '\0';

    printf("PASSWORD (DOB DD-MM-YYYY) : ");
    fgets(pass, sizeof(pass), stdin);
    pass[strcspn(pass, "\n")] = '\0';

    while (fread(&e, sizeof(e), 1, fp)) {
        if (strcmp(e.emp_id, id) == 0 && strcmp(e.dob, pass) == 0) {

            strcpy(currentEmployeeId, e.emp_id);
            strcpy(currentEmployeeName, e.name);

            FILE *log = fopen("employee_log.dat", "ab");
            char timebuf[30];
            getCurrentTime(timebuf);
            fprintf(log, "LOGIN  | %s | %s | %s\n",
                    e.emp_id, e.name, timebuf);
            fclose(log);

            fclose(fp);
            setColor(GREEN);
            printf("\nLOGIN SUCCESSFUL : %s\n", e.name);
            resetColor();
            return 1;
        }
    }

    fclose(fp);
    setColor(RED);
    printf("\nINVALID ID OR PASSWORD\n");
    resetColor();
    return 0;
}

void employeeLogout() {

    if (strlen(currentEmployeeId) == 0) return;

    FILE *log = fopen("employee_log.dat", "ab");
    char timebuf[30];
    getCurrentTime(timebuf);
    fprintf(log, "LOGOUT | %s | %s | %s\n",
            currentEmployeeId, currentEmployeeName, timebuf);
    fclose(log);

    currentEmployeeId[0] = '\0';
    currentEmployeeName[0] = '\0';
}
