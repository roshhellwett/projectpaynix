/*
* PayNix - Billing Software
 * Copyright (c) 2026 Roshan Kumar Singh
 * GitHub: https://github.com/roshhellwett/PayNix
 * Licensed under MIT License
 */

#ifndef BILLINGSOFTWARE_BILLING_H
#define BILLINGSOFTWARE_BILLING_H

/* ---------- BILL HEADER ---------- */
struct BillHeader {
    char customer_name[50];
    char phone[20];
    char last4[5];
    char datetime[30];
    int item_count;
    float grand_total;
};

/* ---------- BILL ITEM ---------- */
struct BillItem {
    char item_id[20];
    char item_name[50];
    float price;
    int quantity;
    float total;
};

/* ---------- BILLING FUNCTION ---------- */
void billing();

#endif // BILLINGSOFTWARE_BILLING_H
