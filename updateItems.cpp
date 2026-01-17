/*
* PayNix - Billing Software
 * Copyright (c) 2026 Roshan Kumar Singh
 * GitHub: https://github.com/roshhellwett/PayNix
 * Licensed under MIT License
 */
#include <cstdio>
#include <cstring>
#include "updateItems.h"
#include "items.h"
#include "showItems.h"
#include "ui.h"

void updateItems() {

    clearScreen();
    printBoxTitle("UPDATE ITEM / PRICE");

    showItemsForBilling();

    FILE *fp, *tempfp;
    struct items item;
    int search_no, found = 0;

    fp = fopen("items.dat", "rb");
    if (fp == NULL) {
        setColor(RED);
        printf("NO ITEMS FOUND.\n");
        resetColor();
        return;
    }

    tempfp = fopen("temp.dat", "wb");
    if (tempfp == NULL) {
        setColor(RED);
        printf("TEMP FILE ERROR.\n");
        resetColor();
        fclose(fp);
        return;
    }

    printf("ENTER THE ITEM ID TO UPDATE: ");
    scanf("%d", &search_no);

    while (fread(&item, sizeof(struct items), 1, fp)) {

        if (item.item_number == search_no) {
            found = 1;

            setColor(YELLOW);
            printf("\nCURRENT NAME  : %s\n", item.item_name);
            printf("CURRENT PRICE : %.2f\n", item.item_price);
            resetColor();

            printf("ENTER NEW NAME  : ");
            getchar();
            fgets(item.item_name, sizeof(item.item_name), stdin);
            item.item_name[strcspn(item.item_name, "\n")] = '\0';

            printf("ENTER NEW PRICE : ");
            scanf("%f", &item.item_price);
        }

        fwrite(&item, sizeof(struct items), 1, tempfp);
    }

    fclose(fp);
    fclose(tempfp);

    remove("items.dat");
    rename("temp.dat", "items.dat");

    if (found) {
        setColor(GREEN);
        printf("ITEM UPDATED SUCCESSFULLY.\n");
        resetColor();
    } else {
        setColor(RED);
        printf("ITEM NOT FOUND.\n");
        resetColor();
    }
}
