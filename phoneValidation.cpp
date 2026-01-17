/*
* PayNix - Billing Software
 * Copyright (c) 2026 Roshan Kumar Singh
 * GitHub: https://github.com/roshhellwett/PayNix
 * Licensed under MIT License
 */
#include <cstring>
#include <cctype>
#include "phoneValidation.h"

int isValidPhone(const char *phone) {

    // Check length (10 digits)
    if (strlen(phone) != 10)
        return 0;

    // Check digits only
    for (int i = 0; phone[i] != '\0'; i++) {
        if (!isdigit(phone[i]))
            return 0;
    }

    return 1; // valid
}
