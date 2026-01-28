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

    // Must be exactly 10 digits
    if (!phone || strlen(phone) != 10)
        return 0;

    // Must contain digits only
    for (int i = 0; phone[i] != '\0'; i++) {
        if (!isdigit((unsigned char)phone[i]))
            return 0;
    }

    // Optional: realistic Indian mobile number check (6–9)
    if (phone[0] < '6' || phone[0] > '9')
        return 0;

    return 1; // valid
}
