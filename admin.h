/*
* PayNix - Billing Software
 * Copyright (c) 2026 Roshan Kumar Singh
 * GitHub: https://github.com/roshhellwett/PayNix
 * Licensed under MIT License
 */
#ifndef BILLINGSOFTWARE_ADMIN_H
#define BILLINGSOFTWARE_ADMIN_H

struct Admin {
    char admin_id[20];
    char password[20];
};

void adminPanel();
void showEmployeeActivity();

#endif // BILLINGSOFTWARE_ADMIN_H
