/*
* PayNix - Billing Software
 * Copyright (c) 2026 Roshan Kumar Singh
 * GitHub: https://github.com/roshhellwett/PayNix
 * Licensed under MIT License
 */

#ifndef BILLINGSOFTWARE_ITEMS_H
#define BILLINGSOFTWARE_ITEMS_H

struct items {
    char item_name[50];
    int item_number;
    float item_price;
};
struct BillHeader {
    char customer_name[50];
    char phone[15];
    char last4[5];
    int item_count;
    char datetime[30];
    float grand_total;
};

struct BillItem {
    int item_number;
    char item_name[50];
    float price;
    int quantity;
    float total;
};

#endif //BILLINGSOFTWARE_ITEMS_H