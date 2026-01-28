/*
 * PayNix - Billing Software
 * Copyright (c) 2026 Roshan Kumar Singh
 * GitHub: https://github.com/roshhellwett/PayNix
 * Licensed under MIT License
 */
/*
 * PayNix - Billing Software
 */
#include <cstdio>
#include <cstring>
#include <ctime>
#include "employee.h"
#include "ui.h"
#include "dataPath.h"

static char currentEmployeeId[20] = "";
static char currentEmployeeName[50] = "";

static void getCurrentTime(char *buf) {
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    strftime(buf, 40, "%d-%m-%Y %H:%M:%S", t);
}

int employeeLogin() {

    FILE *fp = fopen(EMP_FILE, "rb");
    if (!fp) {
        setColor(RED);
        printf("NO EMPLOYEE DATA FOUND\n");
        resetColor();
        return 0;
    }

    char id[20], pass[20];
    struct Employee e;

    printBoxTitle("EMPLOYEE LOGIN");

    printf("EMPLOYEE ID : ");
    fgets(id, sizeof(id), stdin);
    id[strcspn(id, "\n")] = 0;

    printf("PASSWORD (DOB DD-MM-YYYY): ");
    inputHidden(pass, sizeof(pass));

    while (fread(&e, sizeof(e), 1, fp)) {
        if (strcmp(e.emp_id, id) == 0 &&
            strcmp(e.dob, pass) == 0) {

            strcpy(currentEmployeeId, e.emp_id);
            strcpy(currentEmployeeName, e.name);

            FILE *log = fopen(EMPLOYEE_LOG_FILE, "a");
            char timebuf[40];
            getCurrentTime(timebuf);
            fprintf(log, "LOGIN|%s|%s|%s\n",
                    e.emp_id, e.name, timebuf);
            fclose(log);

            fclose(fp);
            return 1;
            }
    }

    fclose(fp);

    setColor(RED);
    printf("\nINVALID EMPLOYEE ID OR PASSWORD\n");
    resetColor();

    return 0;
}

void employeeLogout() {

    if (currentEmployeeName[0] == '\0') return;

    FILE *log = fopen(EMPLOYEE_LOG_FILE, "a");
    char timebuf[40];
    getCurrentTime(timebuf);
    fprintf(log, "LOGOUT|%s|%s|%s\n",
            currentEmployeeId, currentEmployeeName, timebuf);
    fclose(log);

    currentEmployeeId[0] = 0;
    currentEmployeeName[0] = 0;
}
