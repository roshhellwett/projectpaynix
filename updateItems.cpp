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
#include "dataPath.h"

void updateItems() {

    clearScreen();
    printBoxTitle("UPDATE ITEM / PRICE");

    showItemsForBilling();

    FILE *fp = fopen(ITEMS_FILE, "rb");
    if (!fp) {
        setColor(RED);
        printf("NO ITEMS FOUND.\n");
        resetColor();
        printf("PRESS ENTER...");
        getchar();
        return;
    }

    FILE *tempfp = fopen("data/temp.dat", "wb");
    if (!tempfp) {
        setColor(RED);
        printf("TEMP FILE ERROR.\n");
        resetColor();
        fclose(fp);
        printf("PRESS ENTER...");
        getchar();
        return;
    }

    items item;
    char search_id[30];
    int found = 0;

    printf("\nENTER THE ITEM ID TO UPDATE: ");
    fgets(search_id, sizeof(search_id), stdin);
    search_id[strcspn(search_id, "\n")] = 0;

    while (fread(&item, sizeof(item), 1, fp)) {

        if (strcmp(item.item_number, search_id) == 0) {
            found = 1;

            setColor(YELLOW);
            printf("\nCURRENT NAME  : %s\n", item.item_name);
            printf("CURRENT PRICE : %.2f\n", item.item_price);
            resetColor();

            printf("\nENTER NEW NAME  : ");
            fgets(item.item_name, sizeof(item.item_name), stdin);
            item.item_name[strcspn(item.item_name, "\n")] = 0;

            printf("ENTER NEW PRICE : ");
            scanf("%f", &item.item_price);
            getchar();
        }

        fwrite(&item, sizeof(item), 1, tempfp);
    }

    fclose(fp);
    fclose(tempfp);

    remove(ITEMS_FILE);
    rename("data/temp.dat", ITEMS_FILE);

    if (found) {
        setColor(GREEN);
        printf("\nITEM UPDATED SUCCESSFULLY.\n");
        resetColor();
    } else {
        setColor(RED);
        printf("\nITEM NOT FOUND.\n");
        resetColor();
    }

    printf("\nPRESS ENTER TO CONTINUE...");
    getchar();
}
