/*
 * PayNix - Billing Software
 * Copyright (c) 2026 Roshan Kumar Singh
 * GitHub: https://github.com/roshhellwett/PayNix
 * Licensed under MIT License
 */

#include <cstdio>
#include <cstring>
#include "resumeOrder.h"
#include "billing.h"     // ✅ REQUIRED
#include "ui.h"
#include "dataPath.h"


void resumeOrder() {

    FILE *fp = fopen(BILL_FILE, "rb");
    if (!fp) {
        setColor(RED);
        printf("NO BILL DATA FOUND\n");
        resetColor();
        printf("PRESS ENTER...");
        getchar();
        return;
    }

    FILE *tempfp = fopen("data/tempBills.dat", "wb");
    if (!tempfp) {
        fclose(fp);
        setColor(RED);
        printf("TEMP FILE ERROR\n");
        resetColor();
        printf("PRESS ENTER...");
        getchar();
        return;
    }

    BillHeader bh;
    BillItem bi[50];
    BillItem temp;

    long billPos[50];
    int billCount = 0;

    char search4[5];
    int billChoice;

    clearScreen();
    printBoxTitle("RESUME / MODIFY BILL");

    printf("ENTER LAST 4 DIGITS OF PHONE : ");
    scanf("%4s", search4);
    getchar();

    /* ---------- FIND MATCHING BILLS ---------- */

    while (fread(&bh, sizeof(bh), 1, fp)) {

        long pos = ftell(fp) - sizeof(bh);

        for (int i = 0; i < bh.item_count; i++)
            fread(&temp, sizeof(temp), 1, fp);

        if (strcmp(bh.last4, search4) == 0) {
            billPos[billCount++] = pos;
            printf("%d. %s | %s | %.2f\n",
                   billCount,
                   bh.customer_name,
                   bh.datetime,
                   bh.grand_total);
        }
    }

    if (billCount == 0) {
        setColor(RED);
        printf("\nNO PURCHASE FOUND\n");
        resetColor();
        fclose(fp);
        fclose(tempfp);
        printf("PRESS ENTER...");
        getchar();
        return;
    }

    printf("\nSELECT BILL TO MODIFY (1-%d): ", billCount);
    scanf("%d", &billChoice);
    getchar();

    if (billChoice < 1 || billChoice > billCount) {
        setColor(RED);
        printf("INVALID SELECTION\n");
        resetColor();
        fclose(fp);
        fclose(tempfp);
        printf("PRESS ENTER...");
        getchar();
        return;
    }

    /* ---------- REWRITE FILE ---------- */

    rewind(fp);

    while (fread(&bh, sizeof(bh), 1, fp)) {

        long pos = ftell(fp) - sizeof(bh);

        for (int i = 0; i < bh.item_count; i++)
            fread(&bi[i], sizeof(bi[i]), 1, fp);

        if (pos == billPos[billChoice - 1]) {

            clearScreen();
            printBoxTitle("SELECTED BILL");

            printf("%-3s %-18s %-5s %-8s %-8s\n",
                   "NO", "ITEM", "QTY", "PRICE", "TOTAL");
            printf("--------------------------------------------\n");

            for (int i = 0; i < bh.item_count; i++) {
                printf("%-3d %-18s %-5d %-8.2f %-8.2f\n",
                       i + 1,
                       bi[i].item_name,
                       bi[i].quantity,
                       bi[i].price,
                       bi[i].total);
            }

            int index, newQty;
            printf("\nSELECT ITEM NO TO CHANGE (1-%d): ", bh.item_count);
            scanf("%d", &index);
            getchar();

            if (index >= 1 && index <= bh.item_count) {

                printf("ENTER NEW QUANTITY : ");
                scanf("%d", &newQty);
                getchar();

                if (newQty > 0) {
                    bi[index - 1].quantity = newQty;
                    bi[index - 1].total =
                            bi[index - 1].price * newQty;

                    bh.grand_total = 0;
                    for (int i = 0; i < bh.item_count; i++)
                        bh.grand_total += bi[i].total;
                }
            }
        }

        fwrite(&bh, sizeof(bh), 1, tempfp);
        for (int i = 0; i < bh.item_count; i++)
            fwrite(&bi[i], sizeof(bi[i]), 1, tempfp);
    }

    fclose(fp);
    fclose(tempfp);

    remove(BILL_FILE);
    rename("data/tempBills.dat", BILL_FILE);

    setColor(GREEN);
    printf("\nBILL UPDATED & SAVED SUCCESSFULLY\n");
    resetColor();
    printf("PRESS ENTER...");
    getchar();
}
