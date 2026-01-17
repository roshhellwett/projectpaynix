/*
* PayNix - Billing Software
 * Copyright (c) 2026 Roshan Kumar Singh
 * GitHub: https://github.com/roshhellwett/PayNix
 * Licensed under MIT License
 */
#ifndef BILLINGSOFTWARE_ADMIN_H
#define BILLINGSOFTWARE_ADMIN_H
void adminPanel();
void showEmployeeActivity();
struct Admin {
    char admin_id[30];
    char password[30];
};
#endif //BILLINGSOFTWARE_ADMIN_H