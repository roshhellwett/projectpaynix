/*
 * PayNix - Billing Software
 * Copyright (c) 2026 Roshan Kumar Singh
 * GitHub: https://github.com/roshhellwett/PayNix
 * Licensed under MIT License
 */

#include <cstdio>
#include <cstring>
#include "billing.h"
#include "items.h"
#include "showItems.h"
#include "DateTime.h"
#include "dataPath.h"
#include "ui.h"

/* ---------- BILL SUMMARY ---------- */

static void printBillSummary(BillHeader &bh, BillItem bi[]) {

    printBoxTitle("BILL SUMMARY");

    printf("%-18s %-5s %-8s %-8s\n",
           "ITEM", "QTY", "PRICE", "TOTAL");
    printf("--------------------------------------------\n");

    for (int i = 0; i < bh.item_count; i++) {
        printf("%-18s %-5d %-8.2f %-8.2f\n",
               bi[i].item_name,
               bi[i].quantity,
               bi[i].price,
               bi[i].total);
    }

    printf("--------------------------------------------\n");
    printf("GRAND TOTAL : %.2f\n", bh.grand_total);
}

/* ---------- BILLING ---------- */

void billing() {

    items temp;
    BillHeader bh;
    BillItem bi[50];

    FILE *fpI = fopen(ITEMS_FILE, "rb");
    FILE *fpB = fopen(BILL_FILE, "ab");

    if (!fpI || !fpB) {
        setColor(RED);
        printf("ERROR OPENING DATA FILES\n");
        resetColor();
        if (fpI) fclose(fpI);
        if (fpB) fclose(fpB);
        return;
    }

    clearScreen();
    printBoxTitle("BILLING");

    printf("CUSTOMER NAME : ");
    fgets(bh.customer_name, sizeof(bh.customer_name), stdin);
    bh.customer_name[strcspn(bh.customer_name, "\n")] = 0;

    printf("PHONE : ");
    fgets(bh.phone, sizeof(bh.phone), stdin);
    bh.phone[strcspn(bh.phone, "\n")] = 0;

    if (strlen(bh.phone) >= 4)
        strcpy(bh.last4, &bh.phone[strlen(bh.phone) - 4]);
    else
        strcpy(bh.last4, "XXXX");

    getDateTime(bh.datetime);

    bh.item_count = 0;
    bh.grand_total = 0;

    showItemsForBilling();

    char id[30];
    int qty;
    char more = 'y';

    while ((more == 'y' || more == 'Y') && bh.item_count < 50) {

        printf("ITEM ID : ");
        fgets(id, sizeof(id), stdin);
        id[strcspn(id, "\n")] = 0;

        printf("QTY : ");
        scanf("%d", &qty);
        getchar();

        int found = 0;
        rewind(fpI);

        while (fread(&temp, sizeof(temp), 1, fpI)) {
            if (strcmp(temp.item_number, id) == 0) {

                strcpy(bi[bh.item_count].item_id, id);
                strcpy(bi[bh.item_count].item_name, temp.item_name);
                bi[bh.item_count].price = temp.item_price;
                bi[bh.item_count].quantity = qty;
                bi[bh.item_count].total = qty * temp.item_price;

                bh.grand_total += bi[bh.item_count].total;
                bh.item_count++;

                found = 1;
                break;
            }
        }

        if (!found) {
            setColor(RED);
            printf("INVALID ITEM ID\n");
            resetColor();
        }

        printf("MORE (Y/N): ");
        scanf(" %c", &more);
        getchar();
    }

    /* ---------- SAVE BILL ---------- */

    fwrite(&bh, sizeof(bh), 1, fpB);
    for (int i = 0; i < bh.item_count; i++) {
        fwrite(&bi[i], sizeof(BillItem), 1, fpB);
    }

    fclose(fpI);
    fclose(fpB);

    /* ---------- SHOW SUMMARY ---------- */

    clearScreen();
    printBillSummary(bh, bi);

}
