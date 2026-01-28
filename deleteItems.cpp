/*
* PayNix - Billing Software
 * Copyright (c) 2026 Roshan Kumar Singh
 * GitHub: https://github.com/roshhellwett/PayNix
 * Licensed under MIT License
 */

#include <cstdio>
#include <cstring>
#include "deleteItems.h"
#include "items.h"
#include "showItems.h"
#include "ui.h"
#include "dataPath.h"

void deleteItems() {

    clearScreen();
    printBoxTitle("DELETE ITEMS");

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
    char delete_id[30];
    int found = 0;

    printf("\nENTER THE ITEM ID TO DELETE: ");
    fgets(delete_id, sizeof(delete_id), stdin);
    delete_id[strcspn(delete_id, "\n")] = 0;

    while (fread(&item, sizeof(item), 1, fp)) {

        if (strcmp(item.item_number, delete_id) == 0) {
            found = 1;
            continue;   // skip writing -> delete
        }

        fwrite(&item, sizeof(item), 1, tempfp);
    }

    fclose(fp);
    fclose(tempfp);

    remove(ITEMS_FILE);
    rename("data/temp.dat", ITEMS_FILE);

    if (found) {
        setColor(GREEN);
        printf("\nITEM DELETED SUCCESSFULLY.\n");
        resetColor();
    } else {
        setColor(RED);
        printf("\nITEM NOT FOUND.\n");
        resetColor();
    }

    printf("\nPRESS ENTER TO CONTINUE...");
    getchar();
}
