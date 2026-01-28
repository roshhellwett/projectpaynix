/*
* PayNix - Billing Software
 * Copyright (c) 2026 Roshan Kumar Singh
 * GitHub: https://github.com/roshhellwett/PayNix
 * Licensed under MIT License
 */

#include <cstdio>
#include "viewCustomers.h"
#include "billing.h"     // ✅ REQUIRED
#include "ui.h"
#include "dataPath.h"


void viewCustomers() {

    clearScreen();
    printBoxTitle("CUSTOMER BILL HISTORY");

    FILE *fp = fopen(BILL_FILE, "rb");
    if (!fp) {
        setColor(RED);
        printf("NO CUSTOMER DATA FOUND.\n");
        resetColor();
        printf("PRESS ENTER...");
        getchar();
        return;
    }

    BillHeader bh;
    BillItem bi;

    while (fread(&bh, sizeof(bh), 1, fp)) {

        setColor(YELLOW);
        printf("\nCUSTOMER NAME : %s\n", bh.customer_name);
        printf("PHONE NUMBER  : %s\n", bh.phone);
        printf("DATE & TIME   : %s\n", bh.datetime);
        resetColor();

        printf("ITEMS:\n");
        printf("%-18s %-5s %-8s %-8s\n",
               "ITEM", "QTY", "PRICE", "TOTAL");
        printf("--------------------------------------------\n");

        for (int i = 0; i < bh.item_count; i++) {
            fread(&bi, sizeof(bi), 1, fp);
            printf("%-18s %-5d %-8.2f %-8.2f\n",
                   bi.item_name,
                   bi.quantity,
                   bi.price,
                   bi.total);
        }

        printf("--------------------------------------------\n");
        printf("GRAND TOTAL : %.2f\n", bh.grand_total);
        printf("============================================\n");
    }

    fclose(fp);

}
