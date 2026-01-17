#include <cstdio>
#include "showItems.h"
#include "items.h"
#include "ui.h"

void showItemsForBilling() {

    struct items temp;
    FILE *fp = fopen("items.dat", "rb");

    if (fp == NULL) {
        setColor(RED);
        printf("NO ITEMS AVAILABLE.\n");
        resetColor();
        return;
    }


    setColor(YELLOW);
    printf("%-10s %-20s %10s\n", "ITEM ID", "ITEM NAME", "PRICE");
    printf("--------------------------------------------\n");
    resetColor();

    while (fread(&temp, sizeof(struct items), 1, fp)) {
        printf("%-10d %-20s %10.2f\n",
               temp.item_number,
               temp.item_name,
               temp.item_price);
    }

    printf("--------------------------------------------\n");

    fclose(fp);
}
