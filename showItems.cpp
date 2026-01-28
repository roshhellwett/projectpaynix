/*
* PayNix - Billing Software
 * Copyright (c) 2026 Roshan Kumar Singh
 * GitHub: https://github.com/roshhellwett/PayNix
 * Licensed under MIT License
 */

#include <cstdio>
#include "showItems.h"
#include "items.h"
#include "ui.h"
#include "dataPath.h"

void showItemsForBilling() {

    FILE *fp = fopen(ITEMS_FILE, "rb");
    items temp;

    if (!fp) {
        setColor(RED);
        printf("NO ITEMS AVAILABLE\n");
        resetColor();
        return;
    }

    printBoxTitle("AVAILABLE ITEMS");
    printf("ITEM ID       ITEM NAME               PRICE\n");
    printf("--------------------------------------------\n");

    while (fread(&temp, sizeof(temp), 1, fp)) {
        printf("%-12s %-22s %8.2f\n",
               temp.item_number,
               temp.item_name,
               temp.item_price);
    }

    printf("--------------------------------------------\n");
    fclose(fp);
}

