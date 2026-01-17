/*
* PayNix - Billing Software
 * Copyright (c) 2026 Roshan Kumar Singh
 * GitHub: https://github.com/roshhellwett/PayNix
 * Licensed under MIT License
 */
#include <cstdio>
#include <cstring>
#include "resumeOrder.h"
#include "items.h"

void resumeOrder() {

    FILE *fp = fopen("bills.dat", "rb");
    FILE *tempfp = fopen("tempBills.dat", "wb");

    struct BillHeader bh;
    struct BillItem items[50];
    struct BillItem temp;

    long billPos[50];
    int billCount = 0;

    char search4[5];
    int billChoice;

    if (!fp || !tempfp) {
        printf("FILE ERROR\n");
        return;
    }

    printf("ENTER LAST 4 DIGITS OF PHONE : ");
    scanf("%4s", search4);

    while (fread(&bh, sizeof(struct BillHeader), 1, fp)) {

        long pos = ftell(fp) - sizeof(struct BillHeader);

        for (int i = 0; i < bh.item_count; i++)
            fread(&temp, sizeof(struct BillItem), 1, fp);

        if (strcmp(bh.last4, search4) == 0) {
            billPos[billCount++] = pos;
            printf("%d. %s  |  %s\n",
                   billCount,
                   bh.customer_name,
                   bh.datetime);
        }
    }

    if (billCount == 0) {
        printf("NO PURCHASE FOUND\n");
        fclose(fp);
        fclose(tempfp);
        return;
    }

    printf("SELECT BILL TO MODIFY : ");
    scanf("%d", &billChoice);

    if (billChoice < 1 || billChoice > billCount) {
        printf("INVALID SELECTION\n");
        fclose(fp);
        fclose(tempfp);
        return;
    }

    rewind(fp);

    while (fread(&bh, sizeof(struct BillHeader), 1, fp)) {

        long pos = ftell(fp) - sizeof(struct BillHeader);

        for (int i = 0; i < bh.item_count; i++)
            fread(&items[i], sizeof(struct BillItem), 1, fp);

        if (pos == billPos[billChoice - 1]) {

            int index, newQty;

            printf("\nITEMS IN SELECTED BILL:\n");
            printf("NO  ITEM ID   ITEM NAME        PRICE   QTY   TOTAL\n");
            printf("---------------------------------------------------\n");

            for (int i = 0; i < bh.item_count; i++) {
                printf("%-3d %-9d %-15s %-7.2f %-5d %-7.2f\n",
                       i + 1,
                       items[i].item_number,
                       items[i].item_name,
                       items[i].price,
                       items[i].quantity,
                       items[i].total);
            }

            printf("\nSELECT ITEM NO TO CHANGE (1-%d): ", bh.item_count);
            scanf("%d", &index);

            if (index >= 1 && index <= bh.item_count) {

                printf("ENTER NEW QUANTITY : ");
                scanf("%d", &newQty);

                if (newQty > 0) {

                    items[index - 1].quantity = newQty;
                    items[index - 1].total =
                            items[index - 1].price * newQty;

                    bh.grand_total = 0;
                    for (int i = 0; i < bh.item_count; i++)
                        bh.grand_total += items[i].total;

                    printf("\n===== UPDATED BILL =====\n");
                    for (int i = 0; i < bh.item_count; i++) {
                        printf("%d. %s  Qty:%d  Total:%.2f\n",
                               i + 1,
                               items[i].item_name,
                               items[i].quantity,
                               items[i].total);
                    }
                    printf("GRAND TOTAL: %.2f\n", bh.grand_total);

                } else {
                    printf("INVALID QUANTITY\n");
                }
            } else {
                printf("INVALID ITEM SELECTION\n");
            }
        }

        fwrite(&bh, sizeof(struct BillHeader), 1, tempfp);
        for (int i = 0; i < bh.item_count; i++)
            fwrite(&items[i], sizeof(struct BillItem), 1, tempfp);
    }

    fclose(fp);
    fclose(tempfp);

    remove("bills.dat");
    rename("tempBills.dat", "bills.dat");

    printf("\nBILL UPDATED & SAVED SUCCESSFULLY\n");
}
