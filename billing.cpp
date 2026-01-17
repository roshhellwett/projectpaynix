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
#include "ui.h"

void billing() {

    clearScreen();
    printBoxTitle("BILLING SECTION");

    struct BillHeader bh;
    struct BillItem itemsArr[50];
    struct items temp;

    FILE *fpItems = NULL;
    FILE *fpBill  = NULL;

    int search_no, qty;
    int found;
    char more;


    setColor(YELLOW);
    printBoxTitle("CUSTOMER DETAILS");
    resetColor();

    printf("ENTER CUSTOMER NAME : ");
    fgets(bh.customer_name, sizeof(bh.customer_name), stdin);
    bh.customer_name[strcspn(bh.customer_name, "\n")] = '\0';

    printf("ENTER CUSTOMER PHONE NUMBER : ");
    fgets(bh.phone, sizeof(bh.phone), stdin);
    bh.phone[strcspn(bh.phone, "\n")] = '\0';

    strcpy(bh.last4, &bh.phone[strlen(bh.phone) - 4]);
    getDateTime(bh.datetime);

    bh.item_count = 0;
    bh.grand_total = 0;


    showItemsForBilling();

    fpItems = fopen("items.dat", "rb");
    fpBill  = fopen("bills.dat", "ab");

    if (!fpItems || !fpBill) {
        setColor(RED);
        printf("File error!\n");
        resetColor();
        return;
    }

    setColor(GREEN);
    printBoxTitle("BILLING STARTED");
    resetColor();

    do {
        found = 0;

        printf("ENTER ITEM ID : ");
        scanf("%d", &search_no);

        printf("ENTER QUANTITY : ");
        scanf("%d", &qty);

        rewind(fpItems);

        while (fread(&temp, sizeof(struct items), 1, fpItems)) {
            if (temp.item_number == search_no) {

                itemsArr[bh.item_count].item_number = temp.item_number;
                strcpy(itemsArr[bh.item_count].item_name, temp.item_name);
                itemsArr[bh.item_count].price = temp.item_price;
                itemsArr[bh.item_count].quantity = qty;
                itemsArr[bh.item_count].total = temp.item_price * qty;

                setColor(GREEN);
                printf("\nITEM  : %s", itemsArr[bh.item_count].item_name);
                printf("\nQTY   : %d", qty);
                printf("\nTOTAL : %.2f\n", itemsArr[bh.item_count].total);
                resetColor();

                bh.grand_total += itemsArr[bh.item_count].total;
                bh.item_count++;

                found = 1;
                break;
            }
        }

        if (!found) {
            setColor(RED);
            printf("Item not found!\n");
            resetColor();
        }

        printf("ADD MORE ITEMS (Y/N): ");
        scanf(" %c", &more);

    } while (more == 'Y' || more == 'y');

    fwrite(&bh, sizeof(struct BillHeader), 1, fpBill);

    for (int i = 0; i < bh.item_count; i++) {
        fwrite(&itemsArr[i], sizeof(struct BillItem), 1, fpBill);
    }

    fclose(fpItems);
    fclose(fpBill);

    setColor(CYAN);
    printBoxTitle("BILL RECEIPT");
    resetColor();

    printf("CUSTOMER : %s\n", bh.customer_name);
    printf("PHONE    : %s\n", bh.phone);
    printf("DATETIME : %s\n", bh.datetime);

    printf("--------------------------------------\n");
    for (int i = 0; i < bh.item_count; i++) {
        printf("%-15s Qty:%-4d Total:%7.2f\n",
               itemsArr[i].item_name,
               itemsArr[i].quantity,
               itemsArr[i].total);
    }
    printf("--------------------------------------\n");

    setColor(GREEN);
    printf("GRAND TOTAL: %.2f\n", bh.grand_total);
    resetColor();
}
