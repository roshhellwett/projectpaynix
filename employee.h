
#ifndef BILLINGSOFTWARE_EMPLOYEE_H
#define BILLINGSOFTWARE_EMPLOYEE_H

struct Employee {
    char emp_id[20];
    char name[50];
    char phone[20];
    char dob[20];
    float salary;
    char join_date[30];
};

int employeeLogin();
void employeeLogout();
void addEmployee();
void viewEmployees();

#endif //BILLINGSOFTWARE_EMPLOYEE_H